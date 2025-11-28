#include "chibicc.h"

pri int depth;

pri void push() {
  printf("  push %%rax\n");
  depth++;
}

pri void pop(char* arg) {
  printf("  pop %s\n", arg);
  depth--;
}

/// 计算给定节点的绝对地址, 如果给定的节点不在内存中, 则会产生错误
pri void gen_addr(node_ref_t node) {
  if (node->kind == ND_VAR) {
    printf("  lea %d(%%rbp), %%rax\n", node->var->offset);
    return;
  }

  error("not an lvalue");
}

/// 为给定节点生成代码
pri void gen_expr(node_ref_t node) {
  switch (node->kind) {
    case ND_NUM: printf("  mov $%d, %%rax\n", node->val); return;
    case ND_NEG:
      gen_expr(node->lhs);
      printf("  neg %%rax\n");
      return;
    case ND_VAR:
      gen_addr(node);
      printf("  mov (%%rax), %%rax\n");
      return;
    case ND_ASSIGN:
      gen_addr(node->lhs);
      push();
      gen_expr(node->rhs);
      pop("%rdi");
      printf("  mov %%rax, (%%rdi)\n");
      return;

    default: break;
  }

  gen_expr(node->rhs);
  push();
  gen_expr(node->lhs);
  pop("%rdi");

  switch (node->kind) {
    case ND_ADD: printf("  add %%rdi, %%rax\n"); return;
    case ND_SUB: printf("  sub %%rdi, %%rax\n"); return;
    case ND_MUL: printf("  imul %%rdi, %%rax\n"); return;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv %%rdi\n");
      return;
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
      printf("  cmp %%rdi, %%rax\n");

      if (node->kind == ND_EQ) {
        printf("  sete %%al\n");
      } else if (node->kind == ND_NE) {
        printf("  setne %%al\n");
      } else if (node->kind == ND_LT) {
        printf("  setl %%al\n");
      } else if (node->kind == ND_LE) {
        printf("  setle %%al\n");
      }

      printf("  movzb %%al, %%rax\n");
      return;

    default: break;
  }

  error("invalid expression");
}

pri void gen_stmt(node_mut_t node) {
  if (node->kind == ND_EXPR_STMT) {
    gen_expr(node->lhs);
    return;
  }

  error("invalid statement");
}

pri void assign_lvar_offsets(function_mut_t prog) {
  int offset = 0;

  for (obj_mut_t var = prog->locals; var; var = var->next) {
    offset += 8;
    var->offset = -offset;
  }

  prog->stack_size = u_align_of(offset, 16);
}

pub void codegen(function_mut_t prog) {
  assign_lvar_offsets(prog);

  printf("  .globl main\n");
  printf("main:\n");

  // 开始段
  printf("  push %%rbp\n");
  printf("  mov %%rsp, %%rbp\n");
  printf("  sub $%d, %%rsp\n", prog->stack_size);

  for (node_mut_t n = prog->body; n; n = n->next) {
    gen_stmt(n);
    assert(depth == 0);
  }

  printf("  mov %%rbp, %%rsp\n");
  printf("  pop %%rbp\n");
  printf("  ret\n");
}