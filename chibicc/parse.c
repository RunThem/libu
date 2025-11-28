#include "chibicc.h"

pri node_mut_t expr(token_mut_t* rest, token_mut_t tok);
pri node_mut_t expr_stmt(token_mut_t* rest, token_mut_t tok);
pri node_mut_t assign(token_mut_t* rest, token_mut_t tok);
pri node_mut_t equality(token_mut_t* rest, token_mut_t tok);
pri node_mut_t relational(token_mut_t* rest, token_mut_t tok);
pri node_mut_t add(token_mut_t* rest, token_mut_t tok);
pri node_mut_t mul(token_mut_t* rest, token_mut_t tok);
pri node_mut_t unary(token_mut_t* rest, token_mut_t tok);
pri node_mut_t primary(token_mut_t* rest, token_mut_t tok);

pri node_mut_t new_node(node_kind_e kind) {
  return new (node_t, .kind = kind);
}

pri node_mut_t new_binary(node_kind_e kind, node_mut_t lhs, node_mut_t rhs) {
  return new (node_t, .kind = kind, .lhs = lhs, .rhs = rhs);
}

pri node_mut_t new_unary(node_kind_e kind, node_mut_t expr) {
  return new (node_t, .kind = kind, .lhs = expr);
}

pri node_mut_t new_number(int val) {
  return new (node_t, .kind = ND_NUM, .val = val);
}

pri node_mut_t new_var(char name) {
  return new (node_t, .kind = ND_VAR, .name = name);
}

/// stmt = expr-stmt
pri node_mut_t stmt(token_mut_t* rest, token_mut_t tok) {
  return expr_stmt(rest, tok);
}

/// expr-stmt = expr ";"
pri node_mut_t expr_stmt(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = new_unary(ND_EXPR_STMT, expr(&tok, tok));
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
    node = new_binary(ND_ASSIGN, node, assign(&tok, tok->next));
  }

  *rest = tok;

  return node;
}

/// equality = relational ("==" relational | "!=" relational)*
pri node_mut_t equality(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = relational(&tok, tok);

  for (;;) {
    if (equal(tok, "==")) {
      node = new_binary(ND_EQ, node, relational(&tok, tok->next));
      continue;
    }

    if (equal(tok, "!=")) {
      node = new_binary(ND_NE, node, relational(&tok, tok->next));
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
    if (equal(tok, "<")) {
      node = new_binary(ND_LT, node, add(&tok, tok->next));
      continue;
    }

    if (equal(tok, "<=")) {
      node = new_binary(ND_LE, node, add(&tok, tok->next));
      continue;
    }

    if (equal(tok, ">")) {
      node = new_binary(ND_LT, add(&tok, tok->next), node);
      continue;
    }

    if (equal(tok, ">=")) {
      node = new_binary(ND_LE, add(&tok, tok->next), node);
      continue;
    }

    *rest = tok;

    return node;
  }
}

/// add = mul ("+" mul | "-" mul)*
pri node_mut_t add(token_mut_t* rest, token_mut_t tok) {
  node_mut_t node = mul(&tok, tok);

  for (;;) {
    if (equal(tok, "+")) {
      node = new_binary(ND_ADD, node, mul(&tok, tok->next));
      continue;
    }

    if (equal(tok, "-")) {
      node = new_binary(ND_SUB, node, mul(&tok, tok->next));
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
    if (equal(tok, "*")) {
      node = new_binary(ND_MUL, node, unary(&tok, tok->next));
      continue;
    }

    if (equal(tok, "/")) {
      node = new_binary(ND_DIV, node, unary(&tok, tok->next));
      continue;
    }

    *rest = tok;

    return node;
  }
}

/// unary = ("+" | "-") unary
///       | primary
pri node_mut_t unary(token_mut_t* rest, token_mut_t tok) {
  if (equal(tok, "+")) {
    return unary(rest, tok->next);
  }

  if (equal(tok, "-")) {
    return new_unary(ND_NEG, unary(rest, tok->next));
  }

  return primary(rest, tok);
}

/// primary = "(" expr ")" | ident | num
pri node_mut_t primary(token_mut_t* rest, token_mut_t tok) {
  if (equal(tok, "(")) {
    node_mut_t node = expr(&tok, tok->next);
    *rest           = skip(tok, ")");
    return node;
  }

  if (tok->kind == TK_IDENT) {
    node_mut_t node = new_var(*tok->loc);
    *rest           = tok->next;
    return node;
  }

  if (tok->kind == TK_NUM) {
    node_mut_t node = new_number(tok->val);
    *rest           = tok->next;
    return node;
  }

  error_tok(tok, "expected an expression");

  return nullptr;
}

pub node_mut_t parse(token_mut_t tok) {
  node_t head    = {};
  node_mut_t cur = &head;

  while (tok->kind != TK_EOF) {
    cur = cur->next = stmt(&tok, tok);
  }

  return head.next;
}