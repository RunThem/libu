#include "chibicc.h"

/// 解析过程中创建的所有局部变量实例都是追加到这个列表中.
pri ObjMut_t locals;
pri ObjMut_t globals;

pri TypeMut_t declspec(TokenMut_t* rest, TokenMut_t tok);
pri TypeMut_t declarator(TokenMut_t* rest, TokenMut_t tok, TypeMut_t ty);
pri NodeMut_t declaration(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t compound_stmt(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t stmt(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t expr_stmt(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t expr(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t assign(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t equality(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t relational(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t add(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t mul(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t postfix(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t unary(TokenMut_t* rest, TokenMut_t tok);
pri NodeMut_t primary(TokenMut_t* rest, TokenMut_t tok);

/// 按名称查找局部变量
pri ObjMut_t find_var(TokenRef_t tok) {
  for (ObjMut_t var = locals; var; var = var->next) {
    if (strlen(var->name) == tok->len && !strncmp(tok->loc, var->name, tok->len)) {
      return var;
    }
  }

  for (ObjMut_t var = globals; var; var = var->next) {
    if (strlen(var->name) == tok->len && !strncmp(tok->loc, var->name, tok->len)) {
      return var;
    }
  }

  return nullptr;
}

pri NodeMut_t new_node(NodeKind_e kind, TokenMut_t tok) {
  return new(Node_t, .kind = kind, .tok = tok);
}

pri NodeMut_t new_binary(NodeKind_e kind, NodeMut_t lhs, NodeMut_t rhs, TokenMut_t tok) {
  return new(Node_t, .kind = kind, .lhs = lhs, .rhs = rhs, .tok = tok);
}

pri NodeMut_t new_unary(NodeKind_e kind, NodeMut_t expr, TokenMut_t tok) {
  return new(Node_t, .kind = kind, .lhs = expr, .tok = tok);
}

pri NodeMut_t new_number(int val, TokenMut_t tok) {
  return new(Node_t, .kind = ND_NUM, .val = val, .tok = tok);
}

pri NodeMut_t new_var_node(ObjMut_t var, TokenMut_t tok) {
  return new(Node_t, .kind = ND_VAR, .var = var, .tok = tok);
}

pri ObjMut_t new_var(char* name, TypeMut_t ty) {
  return new(Obj_t, .name = name, .ty = ty);
}

pri ObjMut_t new_lvar(char* name, TypeMut_t ty) {
  ObjMut_t var = new(Obj_t, .name = name, .is_local = true, .next = locals, .ty = ty);
  locals       = var;

  return var;
}

pri ObjMut_t new_gvar(char* name, TypeMut_t ty) {
  ObjMut_t var = new(Obj_t, .name = name, .next = globals, .ty = ty);
  globals      = var;

  return var;
}

pri char* new_unique_name() {
  pri int id = 0;

  return format(".L..%d", id++);
}

pri ObjMut_t new_anon_gvar(TypeMut_t ty) {
  return new_gvar(new_unique_name(), ty);
}

pri ObjMut_t new_string_literal(char* p, TypeMut_t ty) {
  ObjMut_t var   = new_anon_gvar(ty);
  var->init_data = p;

  return var;
}

pri char* get_ident(TokenMut_t tok) {
  if (tok->kind != TK_IDENT) {
    error_tok(tok, "expected an identifier");
  }

  return strndup(tok->loc, tok->len);
}

pri int get_number(TokenMut_t tok) {
  if (tok->kind != TK_NUM) {
    error_tok(tok, "expected a number");
  }

  return tok->val;
}

/// declspec = "char" | "int"
pri TypeMut_t declspec(TokenMut_t* rest, TokenMut_t tok) {
  if (equal(tok, "char")) {
    *rest = tok->next;
    return ty_char;
  }

  *rest = skip(tok, "int");
  return ty_int;
}

/// func-params = (param ("," param)* ")
/// param       = declspec declarator
pri TypeMut_t func_params(TokenMut_t* rest, TokenMut_t tok, TypeMut_t ty) {
  Type_t head   = {};
  TypeMut_t cur = &head;

  while (!equal(tok, ")")) {
    if (cur != &head) {
      tok = skip(tok, ",");
    }

    TypeMut_t basety = declspec(&tok, tok);
    TypeMut_t ty     = declarator(&tok, tok, basety);
    cur = cur->next = copy_type(ty);
  }

  ty         = func_type(ty);
  ty->params = head.next;
  *rest      = tok->next;

  return ty;
}

/// type-suffix = ("(" func-params? ")")?
///             | "[" num "]" type-suffix
///             | ε
pri TypeMut_t type_suffix(TokenMut_t* rest, TokenMut_t tok, TypeMut_t ty) {
  if (equal(tok, "(")) {
    return func_params(rest, tok->next, ty);
  }

  if (equal(tok, "[")) {
    int sz = get_number(tok->next);
    tok    = skip(tok->next->next, "]");
    ty     = type_suffix(rest, tok, ty);
    return array_of(ty, sz);
  }

  *rest = tok;
  return ty;
}

/// declarator = "*"* ident type-suffix
pri TypeMut_t declarator(TokenMut_t* rest, TokenMut_t tok, TypeMut_t ty) {
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
pri NodeMut_t declaration(TokenMut_t* rest, TokenMut_t tok) {
  TypeMut_t basety = declspec(&tok, tok);

  Node_t head   = {};
  NodeMut_t cur = &head;
  int i         = 0;

  while (!equal(tok, ";")) {
    if (i++ > 0) {
      tok = skip(tok, ",");
    }

    TypeMut_t ty = declarator(&tok, tok, basety);
    ObjMut_t var = new_lvar(get_ident(ty->name), ty);

    if (!equal(tok, "=")) {
      continue;
    }

    NodeMut_t lhs  = new_var_node(var, ty->name);
    NodeMut_t rhs  = assign(&tok, tok->next);
    NodeMut_t node = new_binary(ND_ASSIGN, lhs, rhs, tok);
    cur = cur->next = new_unary(ND_EXPR_STMT, node, tok);
  }

  NodeMut_t node = new_node(ND_BLOCK, tok);
  node->body     = head.next;
  *rest          = tok->next;

  return node;
}

// 如果是给定的类型则返回真
pri bool is_typename(TokenMut_t tok) {
  return equal(tok, "char") || equal(tok, "int");
}

/// stmt = "return" expr ";"
///      | "if" "(" expr ")" stmt ("else" stmt)?
///      | "for" "(" expr-stmt expr? ";" expr? ")" stmt
///      | "while" "(" expr ")" stmt
///      | "{" compound-stmt
///      | expr-stmt
pri NodeMut_t stmt(TokenMut_t* rest, TokenMut_t tok) {
  if (equal(tok, "return")) {
    NodeMut_t node = new_node(ND_RETURN, tok);
    node->lhs      = expr(&tok, tok->next);
    *rest          = skip(tok, ";");
    return node;
  }

  if (equal(tok, "for")) {
    NodeMut_t node = new_node(ND_FOR, tok);
    tok            = skip(tok->next, "(");

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
    NodeMut_t node = new_node(ND_IF, tok);
    tok            = skip(tok->next, "(");
    node->cond     = expr(&tok, tok);
    tok            = skip(tok, ")");
    node->then     = stmt(&tok, tok);
    if (equal(tok, "else")) {
      node->els = stmt(&tok, tok->next);
    }
    *rest = tok;

    return node;
  }

  if (equal(tok, "while")) {
    NodeMut_t node = new_node(ND_FOR, tok);
    tok            = skip(tok->next, "(");
    node->cond     = expr(&tok, tok);
    tok            = skip(tok, ")");
    node->then     = stmt(rest, tok);

    return node;
  }

  if (equal(tok, "{")) {
    return compound_stmt(rest, tok->next);
  }

  return expr_stmt(rest, tok);
}

/// compoound-stmt = (declaration | stmt)* "}"
pri NodeMut_t compound_stmt(TokenMut_t* rest, TokenMut_t tok) {
  Node_t head   = {};
  NodeMut_t cur = &head;

  while (!equal(tok, "}")) {
    if (is_typename(tok)) {
      cur = cur->next = declaration(&tok, tok);
    } else {
      cur = cur->next = stmt(&tok, tok);
    }

    add_type(cur);
  }

  NodeMut_t node = new(Node_t, .kind = ND_BLOCK, .body = head.next, .tok = tok);
  *rest          = tok->next;

  return node;
}

/// expr-stmt = expr? ";"
pri NodeMut_t expr_stmt(TokenMut_t* rest, TokenMut_t tok) {
  if (equal(tok, ";")) {
    *rest = tok->next;
    return new_node(ND_BLOCK, tok);
  }

  NodeMut_t node = new_node(ND_EXPR_STMT, tok);
  node->lhs      = expr(&tok, tok);
  *rest          = skip(tok, ";");

  return node;
}

/// expr = assign
pri NodeMut_t expr(TokenMut_t* rest, TokenMut_t tok) {
  return assign(rest, tok);
}

/// assign = equality ("=" assign)?
pri NodeMut_t assign(TokenMut_t* rest, TokenMut_t tok) {
  NodeMut_t node = equality(&tok, tok);

  if (equal(tok, "=")) {
    return new_binary(ND_ASSIGN, node, assign(rest, tok->next), tok);
  }

  *rest = tok;

  return node;
}

/// equality = relational ("==" relational | "!=" relational)*
pri NodeMut_t equality(TokenMut_t* rest, TokenMut_t tok) {
  NodeMut_t node = relational(&tok, tok);

  for (;;) {
    TokenMut_t start = tok;

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
pri NodeMut_t relational(TokenMut_t* rest, TokenMut_t tok) {
  NodeMut_t node = add(&tok, tok);

  for (;;) {
    TokenMut_t start = tok;

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

pri NodeMut_t new_add(NodeMut_t lhs, NodeMut_t rhs, TokenMut_t tok) {
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

pri NodeMut_t new_sub(NodeMut_t lhs, NodeMut_t rhs, TokenMut_t tok) {
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
    NodeMut_t node = new_binary(ND_SUB, lhs, rhs, tok);
    node->ty       = lhs->ty;
    return node;
  }

  // ptr - ptr
  if (lhs->ty->base && rhs->ty->base) {
    NodeMut_t node = new_binary(ND_SUB, lhs, rhs, tok);
    node->ty       = ty_int;
    return new_binary(ND_DIV, node, new_number(lhs->ty->base->size, tok), tok);
  }

  error_tok(tok, "invalid operands");

  return nullptr;
}

/// add = mul ("+" mul | "-" mul)*
pri NodeMut_t add(TokenMut_t* rest, TokenMut_t tok) {
  NodeMut_t node = mul(&tok, tok);

  for (;;) {
    TokenMut_t start = tok;

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
pri NodeMut_t mul(TokenMut_t* rest, TokenMut_t tok) {
  NodeMut_t node = unary(&tok, tok);

  for (;;) {
    TokenMut_t start = tok;

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
///       | postfix
pri NodeMut_t unary(TokenMut_t* rest, TokenMut_t tok) {
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

  return postfix(rest, tok);
}

/// postfix = primary ("[" expr "]")
pri NodeMut_t postfix(TokenMut_t* rest, TokenMut_t tok) {
  NodeMut_t node = primary(&tok, tok);

  while (equal(tok, "[")) {
    // x[y] is short for *(x+y)
    TokenMut_t start = tok;
    NodeMut_t idx    = expr(&tok, tok->next);
    tok              = skip(tok, "]");
    node             = new_unary(ND_DEREF, new_add(node, idx, start), start);
  }

  *rest = tok;

  return node;
}

/// funcall = ident "(" (assign "," assign)*)? ")"
pri NodeMut_t funcall(TokenMut_t* rest, TokenMut_t tok) {
  TokenMut_t start = tok;
  tok              = tok->next->next;

  Node_t head   = {};
  NodeMut_t cur = &head;

  while (!equal(tok, ")")) {
    if (cur != &head) {
      tok = skip(tok, ",");
    }

    cur = cur->next = assign(&tok, tok);
  }

  *rest = skip(tok, ")");

  NodeMut_t node = new_node(ND_FUNCALL, start);
  node->funcname = strndup(start->loc, start->len);
  node->args     = head.next;

  return node;
}

/// primary = "(" expr ")" | "sizeof" unary | ident func-args? | str | num
pri NodeMut_t primary(TokenMut_t* rest, TokenMut_t tok) {
  if (equal(tok, "(")) {
    NodeMut_t node = expr(&tok, tok->next);
    *rest          = skip(tok, ")");
    return node;
  }

  if (equal(tok, "sizeof")) {
    NodeMut_t node = unary(rest, tok->next);
    add_type(node);
    return new_number(node->ty->size, tok);
  }

  if (tok->kind == TK_IDENT) {
    // 函数调用
    if (equal(tok->next, "(")) {
      return funcall(rest, tok);
    }

    // 变量
    ObjMut_t var = find_var(tok);
    if (!var) {
      error_tok(tok, "undefined variable");
    }
    *rest = tok->next;

    return new_var_node(var, tok);
  }

  if (tok->kind == TK_STR) {
    ObjMut_t var = new_string_literal(tok->str, tok->ty);
    *rest        = tok->next;
    return new_var_node(var, tok);
  }

  if (tok->kind == TK_NUM) {
    NodeMut_t node = new_number(tok->val, tok);
    *rest          = tok->next;
    return node;
  }

  error_tok(tok, "expected an expression");

  return nullptr;
}

pri void create_param_lvars(TypeMut_t param) {
  if (param) {
    create_param_lvars(param->next);
    new_lvar(get_ident(param->name), param);
  }
}

pri TokenMut_t function(TokenMut_t tok, TypeMut_t basety) {
  TypeMut_t ty = declarator(&tok, tok, basety);

  ObjMut_t fn     = new_gvar(get_ident(ty->name), ty);
  fn->is_function = true;

  locals = nullptr;
  create_param_lvars(ty->params);
  fn->params = locals;

  tok        = skip(tok, "{");
  fn->body   = compound_stmt(&tok, tok);
  fn->locals = locals;

  return tok;
}

pri TokenMut_t global_variable(TokenMut_t tok, TypeMut_t basety) {
  bool first = true;

  while (!consume(&tok, tok, ";")) {
    if (!first) {
      tok = skip(tok, ",");
    }

    first        = false;
    TypeMut_t ty = declarator(&tok, tok, basety);
    new_gvar(get_ident(ty->name), ty);
  }

  return tok;
}

pri bool is_function(TokenMut_t tok) {
  if (equal(tok, ";")) {
    return false;
  }

  Type_t dummy = {};
  TypeMut_t ty = declarator(&tok, tok, &dummy);

  return ty->kind == TY_FUNC;
}

/// program = function-definition*
pub ObjMut_t parse(TokenMut_t tok) {
  globals = nullptr;

  while (tok->kind != TK_EOF) {
    TypeMut_t basety = declspec(&tok, tok);

    // 函数
    if (is_function(tok)) {
      tok = function(tok, basety);
      continue;
    }

    // 全局变量
    tok = global_variable(tok, basety);
  }

  return globals;
}