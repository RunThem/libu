#include "chibicc.h"

pub type_mut_t ty_int = &me(type_t, TY_INT);

pub bool is_integer(type_mut_t ty) {
  return ty->kind == TY_INT;
}

pub type_mut_t pointer_to(type_mut_t base) {
  return new(type_t, .kind = TY_PTR, .base = base);
}

pub type_mut_t func_type(type_mut_t return_ty) {
  return new(type_t, .kind = TY_FUNC, .return_ty = return_ty);
}

pub void add_type(node_mut_t node) {
  if (!node || node->ty) {
    return;
  }

  add_type(node->lhs);
  add_type(node->rhs);
  add_type(node->cond);
  add_type(node->then);
  add_type(node->els);
  add_type(node->init);
  add_type(node->inc);

  for (node_mut_t n = node->body; n; n = n->next) {
    add_type(n);
  }

  switch (node->kind) {
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
    case ND_NEG:
    case ND_ASSIGN: node->ty = node->lhs->ty; return;

    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
    case ND_NUM:
    case ND_FUNCALL: node->ty = ty_int; return;

    case ND_VAR: node->ty = node->var->ty; return;

    case ND_ADDR: node->ty = pointer_to(node->lhs->ty); return;

    case ND_DEREF:
      if (node->lhs->ty->kind != TY_PTR) {
        error_tok(node->tok, "invalid pointer dereference");
      }

      node->ty = node->lhs->ty->base;
      return;

    default: break;
  }
}
