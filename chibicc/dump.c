#include "chibicc.h"

pri int space = 0;

pri void fmt(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  printf("%*s", space, "");
  vprintf(fmt, ap);
  printf("\n");
}

pri void dump_ast(node_ref_t node) {
  switch (node->kind) {
    case ND_EXPR_STMT:
      fmt("ND_EXPR_STMT");
      space++;
      dump_ast(node->lhs);
      space--;
      break;

    case ND_ASSIGN:
      fmt("ND_ASSIGN");
      space++;
      dump_ast(node->lhs);
      dump_ast(node->rhs);
      space--;
      break;

    case ND_VAR:
      fmt("ND_VAR");
      space++;
      fmt("var: %s, offset: %d", node->var->name, node->var->offset);
      space--;
      break;

    case ND_NUM:
      fmt("ND_NUM");
      space++;
      fmt("num: %d", node->val);
      space--;
      break;

    default: break;
  }
}

pub void dump(function_ref_t prog) {
  fmt("stack-size: %d", prog->stack_size);

  int count = 0;
  for (obj_ref_t var = prog->locals; var; var = var->next) {
    fmt("lvar[%d]: %s, offset: %d", count, var->name, var->offset);
    count++;
  }

  for (node_ref_t n = prog->body; n; n = n->next) {
    dump_ast(n);
  }
}