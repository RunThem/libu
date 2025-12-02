#include "chibicc.h"

/// 解析过程中创建的所有局部变量实例都是追加到这个列表中.
obj_mut_t locals;

pri type_mut_t declspec(token_mut_t* rest, token_mut_t tok);
pri type_mut_t declarator(token_mut_t* rest, token_mut_t tok, type_mut_t ty);
pri node_mut_t declaration(token_mut_t* rest, token_mut_t tok);
pri node_mut_t compound_stmt(token_mut_t* rest, token_mut_t tok);
pri node_mut_t stmt(token_mut_t* rest, token_mut_t tok);
pri node_mut_t expr_stmt(token_mut_t* rest, token_mut_t tok);
pri node_mut_t expr(token_mut_t* rest, token_mut_t tok);
pri node_mut_t assign(token_mut_t* rest, token_mut_t tok);
pri node_mut_t equality(token_mut_t* rest, token_mut_t tok);
pri node_mut_t relational(token_mut_t* rest, token_mut_t tok);
pri node_mut_t add(token_mut_t* rest, token_mut_t tok);
pri node_mut_t mul(token_mut_t* rest, token_mut_t tok);
pri node_mut_t unary(token_mut_t* rest, token_mut_t tok);
pri node_mut_t primary(token_mut_t* rest, token_mut_t tok);

/// 按名称查找局部变量
pri obj_mut_t find_var(token_ref_t tok) {
  for (obj_mut_t var = locals; var; var = var->next) {
    if (strlen(var->name) == tok->len && !strncmp(tok->loc, var->name, tok->len)) {
      return var;
    }
  }

  return nullptr;
}

pri node_mut_t new_node(node_kind_e kind, token_mut_t tok) {
  return new(node_t, .kind = kind, .tok = tok);
}

pri node_mut_t new_binary(node_kind_e kind, node_mut_t lhs, node_mut_t rhs, token_mut_t tok) {
  return new(node_t, .kind = kind, .lhs = lhs, .rhs = rhs, .tok = tok);
}

pri node_mut_t new_unary(node_kind_e kind, node_mut_t expr, token_mut_t tok) {
  return new(node_t, .kind = kind, .lhs = expr, .tok = tok);
}

pri node_mut_t new_number(int val, token_mut_t tok) {
  return new(node_t, .kind = ND_NUM, .val = val, .tok = tok);
}

pri node_mut_t new_var(obj_mut_t var, token_mut_t tok) {
  return new(node_t, .kind = ND_VAR, .var = var, .tok = tok);
}

pri obj_mut_t new_lvar(char* name, type_mut_t ty) {
  obj_mut_t var = new(obj_t, .name = name, .next = locals, .ty = ty);
  locals        = var;

  return var;
}

pri char* get_ident(token_mut_t tok) {
  if (tok->kind != TK_IDENT) {
    error_tok(tok, "expected an identifier");
  }

  return strndup(tok->loc, tok->len);
}

pri int get_number(token_mut_t tok) {
  if (tok->kind != TK_NUM) {
    error_tok(tok, "expected a number");
  }

  return tok->val;
}

/// declspec = "int"
pri type_mut_t declspec(token_mut_t* rest, token_mut_t tok) {
  *rest = skip(tok, "int");
  return ty_int;
}

/// func-params = (param ("," param)* ")
/// param       = declspec declarator
pri type_mut_t func_params(token_mut_t* rest, token_mut_t tok, type_mut_t ty) {
  type_t head    = {};
  type_mut_t cur = &head;

  while (!equal(tok, ")")) {
    if (cur != &head) {
      tok = skip(tok, ",");
    }

    type_mut_t basety = declspec(&tok, tok);
    type_mut_t ty     = declarator(&tok, tok, basety);
    cur = cur->next = copy_type(ty);
  }

  ty         = func_type(ty);
  ty->params = head.next;
  *rest      = tok->next;

  return ty;
}

/// type-suffix = ("(" func-params? ")")?
///             | "[" num "]"
///             | ε
pri type_mut_t type_suffix(token_mut_t* rest, token_mut_t tok, type_mut_t ty) {
  if (equal(tok, "(")) {
    return func_params(rest, tok->next, ty);
  }

  if (equal(tok, "[")) {
    int sz = get_number(tok->next);
    *rest  = skip(tok->next->next, "]");
    return array_of(ty, sz);
  }

  *rest = tok;
  return ty;
}

/// declarator = "*"* ident type-suffix
pri type_mut_t declarator(token_mut_t* rest, token_mut_t tok, type_mut_t ty) {
  while (consume(&tok, tok, "*")) {
    ty = pointer_to(ty);
  }

  if (tok->kind != TK_IDENT) {
    error_tok(tok, "expected a variable name");
  }

  ty       = type_suffix(rest, tok->next, ty);
  ty->name = tok;

  return ty;
}

/// declaration = declspec (declarator ("=" expr)? ("," declarator ("=" expr)?)*)? ";"
pri node_mut_t declaration(token_mut_t* rest, token_mut_t tok) {
  type_mut_t basety = declspec(&tok, tok);

  node_t head    = {};
  node_mut_t cur = &head;
  int i          = 0;

  while (!equal(tok, ";")) {
    if (i++ > 0) {
      tok = skip(tok, ",");
    }

    type_mut_t ty = declarator(&tok, tok, basety);
    obj_mut_t var = new_lvar(get_ident(ty->name), ty);

    if (!equal(tok, "=")) {
      continue;
    }

    node_mut_t lhs  = new_var(var, ty->name);
    node_mut_t rhs  = assign(&tok, tok->next);
    node_mut_t node = new_binary(ND_ASSIGN, lhs, rhs, tok);
    cur = cur->next = new_unary(ND_EXPR_STMT, node, tok);
  }

  node_mut_t node = new_node(ND_BLOCK, tok);
  node->body      = head.next;
  *rest           = tok->next;

  return node;
}

/// stmt = "return" expr ";"
///      | "if" "(" expr ")" stmt ("else" stmt)?
///      | "for" "(" expr-stmt expr? ";" expr? ")" stmt
///      | "while" "(" expr ")" stmt
///      | "{" compound-stmt
///      | expr-stmt
pri node_mut_t stmt(token_mut_t* rest, token_mut_t tok) {
  if (equal(tok, "return")) {
    node_mut_t node = new_node(ND_RETURN, tok);
    node->lhs       = expr(&tok, tok->next);
    *rest           = skip(tok, ";");
    return node;
  }

  if (equal(tok, "for")) {
    node_mut_t node = new_node(ND_FOR, tok);
    tok             = skip(tok->next, "(");

    node->init = expr_stmt(&tok, tok);

    if (!equal(tok, ";")) {
      node->cond = expr(&tok, tok);
    }

    tok = skip(tok, ";");

    if (!equal(tok, ")")) {
      node->inc = expr(&tok, tok);
    }
    tok = skip(tok, ")");

    node->then = stmt(rest, tok);

    return node;
  }

  if (equal(tok, "if")) {
    node_mut_t node = new_node(ND_IF, tok);
    tok             = skip(tok->next, "(");
    node->cond      = expr(&tok, tok);
    tok             = skip(tok, ")");
    node->then      = stmt(&tok, tok);
    if (equal(tok, "else")) {
      node->els = stmt(&tok, tok->next);
    }
    *rest = tok;

    return node;
  }

  if (equal(tok, "while")) {
    node_mut_t node = new_node(ND_FOR, tok);
    tok             = skip(tok->next, "(");
    node->cond      = expr(&tok, tok);
    tok             = skip(tok, ")");
    node->then      = stmt(rest, tok);

    return node;
  }

  if (equal(tok, "{")) {
    return compound_stmt(rest, tok->next);
  }

  return expr_stmt(rest, tok);
}

/// compoound-stmt = (declaration | stmt)* "}"
pri node_mut_t compound_stmt(token_mut_t* rest, token_mut_t tok) {
  node_t head    = {};
  node_mut_t cur = &head;

  while (!equal(tok, "}")) {
    if (equal(tok, "int")) {
      cur = cur->next = declaration(&tok, tok);
    } else {
      cur = cur->next = stmt(&tok, tok);
    }

    add_type(cur);
  }

  node_mut_t node = new(node_t, .kind = ND_BLOCK, .body = head.next, .tok = tok);
  *rest           = tok->next;

  return node;
}

/// expr-stmt = expr? ";"
pri node_mut_t expr_stmt(token_mut_t* rest, token_mut_t tok) {
  if (equal(tok, ";")) {
    *rest = tok->next;
    return new_node(ND_BLOCK, tok);
  }

  node_mut_t node = new_node(ND_EXPR_STMT, tok);
  node->lhs       = expr(&tok, tok);
  *rest           = skip(tok, ";");

  return node;
}

/// expr = assign
pri node_mut_t expr(token_mut_t* rest, token_mut_t tok) {
  return assign(rest, tok);
}

/// assign = equality ("=" assign)?
pri node_mut_t assign(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = equality(&tok, tok);

  if (equal(tok, "=")) {
    return new_binary(ND_ASSIGN, node, assign(rest, tok->next), tok);
  }

  *rest = tok;

  return node;
}

/// equality = relational ("==" relational | "!=" relational)*
pri node_mut_t equality(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = relational(&tok, tok);

  for (;;) {
    token_mut_t start = tok;

    if (equal(tok, "==")) {
      node = new_binary(ND_EQ, node, relational(&tok, tok->next), start);
      continue;
    }

    if (equal(tok, "!=")) {
      node = new_binary(ND_NE, node, relational(&tok, tok->next), start);
      continue;
    }

    *rest = tok;

    return node;
  }
}

/// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
pri node_mut_t relational(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = add(&tok, tok);

  for (;;) {
    token_mut_t start = tok;

    if (equal(tok, "<")) {
      node = new_binary(ND_LT, node, add(&tok, tok->next), start);
      continue;
    }

    if (equal(tok, "<=")) {
      node = new_binary(ND_LE, node, add(&tok, tok->next), start);
      continue;
    }

    if (equal(tok, ">")) {
      node = new_binary(ND_LT, add(&tok, tok->next), node, start);
      continue;
    }

    if (equal(tok, ">=")) {
      node = new_binary(ND_LE, add(&tok, tok->next), node, start);
      continue;
    }

    *rest = tok;

    return node;
  }
}

pri node_mut_t new_add(node_mut_t lhs, node_mut_t rhs, token_mut_t tok) {
  add_type(lhs);
  add_type(rhs);

  // 数字 + 数字
  if (is_integer(lhs->ty) && is_integer(rhs->ty)) {
    return new_binary(ND_ADD, lhs, rhs, tok);
  }

  if (lhs->ty->base && rhs->ty->base) {
    error_tok(tok, "invalid operands");
  }

  // 调整 `num + ptr` 为 `ptr + num`
  if (!lhs->ty->base && rhs->ty->base) {
    u_swap(lhs, rhs);
  }

  // 指针 + 数字
  rhs = new_binary(ND_MUL, rhs, new_number(lhs->ty->base->size, tok), tok);

  return new_binary(ND_ADD, lhs, rhs, tok);
}

pri node_mut_t new_sub(node_mut_t lhs, node_mut_t rhs, token_mut_t tok) {
  add_type(lhs);
  add_type(rhs);

  // 数字 - 数字
  if (is_integer(lhs->ty) && is_integer(rhs->ty)) {
    return new_binary(ND_SUB, lhs, rhs, tok);
  }

  // ptr - num
  if (lhs->ty->base && is_integer(rhs->ty)) {
    rhs = new_binary(ND_MUL, rhs, new_number(lhs->ty->base->size, tok), tok);
    add_type(rhs);
    node_mut_t node = new_binary(ND_SUB, lhs, rhs, tok);
    node->ty        = lhs->ty;
    return node;
  }

  // ptr - ptr
  if (lhs->ty->base && rhs->ty->base) {
    node_mut_t node = new_binary(ND_SUB, lhs, rhs, tok);
    node->ty        = ty_int;
    return new_binary(ND_DIV, node, new_number(lhs->ty->base->size, tok), tok);
  }

  error_tok(tok, "invalid operands");

  return nullptr;
}

/// add = mul ("+" mul | "-" mul)*
pri node_mut_t add(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = mul(&tok, tok);

  for (;;) {
    token_mut_t start = tok;

    if (equal(tok, "+")) {
      node = new_add(node, mul(&tok, tok->next), start);
      continue;
    }

    if (equal(tok, "-")) {
      node = new_sub(node, mul(&tok, tok->next), start);
      continue;
    }

    *rest = tok;

    return node;
  }
}

/// mul = unary ("*" unary | "/" primary)*
pri node_mut_t mul(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = unary(&tok, tok);

  for (;;) {
    token_mut_t start = tok;

    if (equal(tok, "*")) {
      node = new_binary(ND_MUL, node, unary(&tok, tok->next), start);
      continue;
    }

    if (equal(tok, "/")) {
      node = new_binary(ND_DIV, node, unary(&tok, tok->next), start);
      continue;
    }

    *rest = tok;

    return node;
  }
}

/// unary = ("+" | "-" | "*" | "&") unary
///       | primary
pri node_mut_t unary(token_mut_t* rest, token_mut_t tok) {
  if (equal(tok, "+")) {
    return unary(rest, tok->next);
  }

  if (equal(tok, "-")) {
    return new_unary(ND_NEG, unary(rest, tok->next), tok);
  }

  if (equal(tok, "&")) {
    return new_unary(ND_ADDR, unary(rest, tok->next), tok);
  }

  if (equal(tok, "*")) {
    return new_unary(ND_DEREF, unary(rest, tok->next), tok);
  }

  return primary(rest, tok);
}

/// funcall = ident "(" (assign "," assign)*)? ")"
pri node_mut_t funcall(token_mut_t* rest, token_mut_t tok) {
  token_mut_t start = tok;
  tok               = tok->next->next;

  node_t head    = {};
  node_mut_t cur = &head;

  while (!equal(tok, ")")) {
    if (cur != &head) {
      tok = skip(tok, ",");
    }

    cur = cur->next = assign(&tok, tok);
  }

  *rest = skip(tok, ")");

  node_mut_t node = new_node(ND_FUNCALL, start);
  node->funcname  = strndup(start->loc, start->len);
  node->args      = head.next;

  return node;
}

/// primary = "(" expr ")" | ident func-args? | num
pri node_mut_t primary(token_mut_t* rest, token_mut_t tok) {
  if (equal(tok, "(")) {
    node_mut_t node = expr(&tok, tok->next);
    *rest           = skip(tok, ")");
    return node;
  }

  if (tok->kind == TK_IDENT) {
    // 函数调用
    if (equal(tok->next, "(")) {
      return funcall(rest, tok);
    }

    // 变量
    obj_mut_t var = find_var(tok);
    if (!var) {
      error_tok(tok, "undefined variable");
    }
    *rest = tok->next;

    return new_var(var, tok);
  }

  if (tok->kind == TK_NUM) {
    node_mut_t node = new_number(tok->val, tok);
    *rest           = tok->next;
    return node;
  }

  error_tok(tok, "expected an expression");

  return nullptr;
}

pri void create_param_lvars(type_mut_t param) {
  if (param) {
    create_param_lvars(param->next);
    new_lvar(get_ident(param->name), param);
  }
}

pri function_mut_t function(token_mut_t* rest, token_mut_t tok) {
  type_mut_t ty = declspec(&tok, tok);
  ty            = declarator(&tok, tok, ty);

  locals = nullptr;

  function_mut_t fn = u_talloc(function_t);
  fn->name          = get_ident(ty->name);
  create_param_lvars(ty->params);
  fn->params = locals;

  tok        = skip(tok, "{");
  fn->body   = compound_stmt(rest, tok);
  fn->locals = locals;

  return fn;
}

/// program = function-definition*
pub function_mut_t parse(token_mut_t tok) {
  function_t head    = {};
  function_mut_t cur = &head;

  while (tok->kind != TK_EOF) {
    cur = cur->next = function(&tok, tok);
  }

  return head.next;
}