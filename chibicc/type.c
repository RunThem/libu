#include "chibicc.h"

pub TypeMut_t ty_int  = &me(Type_t, TY_INT, 8);
pub TypeMut_t ty_char = &me(Type_t, TY_CHAR, 1);

pub bool is_integer(TypeMut_t ty) {
  return ty->kind == TY_CHAR || ty->kind == TY_INT;
}

pub TypeMut_t copy_type(TypeMut_t ty) {
  TypeMut_t ret = u_talloc(Type_t);
  *ret          = *ty;
  return ret;
}

pub TypeMut_t pointer_to(TypeMut_t base) {
  return new(Type_t, .kind = TY_PTR, .size = 8, .base = base);
}

pub TypeMut_t func_type(TypeMut_t return_ty) {
  return new(Type_t, .kind = TY_FUNC, .return_ty = return_ty);
}

pub TypeMut_t array_of(TypeMut_t base, int len) {
  return new(Type_t, .kind = TY_ARRAY, .size = base->size * len, .base = base, .array_len = len);
}

pub void add_type(NodeMut_t node) {
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

  for (NodeMut_t n = node->body; n; n = n->next) {
    add_type(n);
  }

  for (NodeMut_t n = node->args; n; n = n->next) {
    add_type(n);
  }

  switch (node->kind) {
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
    case ND_NEG: node->ty = node->lhs->ty; return;
    case ND_ASSIGN:
      if (node->lhs->ty->kind == TY_ARRAY) {
        error_tok(node->lhs->tok, "not an lvalue");
      }

      node->ty = node->lhs->ty;
      return;

    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
    case ND_NUM:
    case ND_FUNCALL: node->ty = ty_int; return;

    case ND_VAR: node->ty = node->var->ty; return;

    case ND_ADDR:
      if (node->lhs->ty->kind == TY_ARRAY) {
        node->ty = pointer_to(node->lhs->ty->base);
      } else {
        node->ty = pointer_to(node->lhs->ty);
      }

      return;

    case ND_DEREF:
      if (!node->lhs->ty->base) {
        error_tok(node->tok, "invalid pointer dereference");
      }

      node->ty = node->lhs->ty->base;
      return;

    default: break;
  }
}
