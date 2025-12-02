#include "chibicc.h"

pri int depth;

pri char* argreg[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

pri function_mut_t current_fn = nullptr;

pri void gen_expr(node_ref_t node);

pri int count() {
  pri int i = 1;
  return i++;
}

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
  switch (node->kind) {
    case ND_VAR: printf("  lea %d(%%rbp), %%rax\n", node->var->offset); return;
    case ND_DEREF: gen_expr(node->lhs); return;

    default: break;
  }

  error_tok(node->tok, "not an lvalue");
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
    case ND_DEREF:
      gen_expr(node->lhs);
      printf("  mov (%%rax), %%rax\n");
      return;
    case ND_ADDR: gen_addr(node->lhs); return;
    case ND_ASSIGN:
      gen_addr(node->lhs);
      push();
      gen_expr(node->rhs);
      pop("%rdi");
      printf("  mov %%rax, (%%rdi)\n");
      return;
    case ND_FUNCALL: {

      int nargs = 0;
      for (node_mut_t arg = node->args; arg; arg = arg->next) {
        gen_expr(arg);
        push();
        nargs++;
      }

      for (int i = nargs - 1; i >= 0; i--) {
        pop(argreg[i]);
      }

      printf("  mov $0, %%rax\n");
      printf("  call %s\n", node->funcname);
      return;
    }

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

  error_tok(node->tok, "invalid expression");
}

pri void gen_stmt(node_mut_t node) {
  switch (node->kind) {
    case ND_IF: {

      int c = count();
      gen_expr(node->cond);
      printf("  cmp $0, %%rax\n");
      printf("  je .L.else.%d\n", c);
      gen_stmt(node->then);
      printf("  jmp .L.end.%d\n", c);
      printf(".L.else.%d:\n", c);
      if (node->els) {
        gen_stmt(node->els);
      }
      printf(".L.end.%d:\n", c);
      return;
    }
    case ND_FOR: {
      int c = count();
      if (node->init) {
        gen_stmt(node->init);
      }

      printf(".L.begin.%d:\n", c);
      if (node->cond) {
        gen_expr(node->cond);
        printf("  cmp $0, %%rax\n");
        printf("  je .L.end.%d\n", c);
      }
      gen_stmt(node->then);
      if (node->inc) {
        gen_expr(node->inc);
      }
      printf("  jmp .L.begin.%d\n", c);
      printf(".L.end.%d:\n", c);
      return;
    }
    case ND_BLOCK:
      for (node_mut_t n = node->body; n; n = n->next) {
        gen_stmt(n);
      }
      return;
    case ND_RETURN:
      gen_expr(node->lhs);
      printf("  jmp .L.return.%s\n", current_fn->name);
      return;
    case ND_EXPR_STMT: gen_expr(node->lhs); return;

    default: break;
  }

  error_tok(node->tok, "invalid statement");
}

pri void assign_lvar_offsets(function_mut_t prog) {
  for (function_mut_t fn = prog; fn; fn = fn->next) {
    int offset = 0;

    for (obj_mut_t var = fn->locals; var; var = var->next) {
      offset += 8;
      var->offset = -offset;
    }

    fn->stack_size = u_align_of(offset, 16);
  }
}

pub void codegen(function_mut_t prog) {
  assign_lvar_offsets(prog);

  for (function_mut_t fn = prog; fn; fn = fn->next) {
    printf("  .globl %s\n", fn->name);
    printf("%s:\n", fn->name);
    current_fn = fn;

    // Prologue
    printf("  push %%rbp\n");
    printf("  mov %%rsp, %%rbp\n");
    printf("  sub $%d, %%rsp\n", fn->stack_size);

    int i = 0;
    for (obj_mut_t var = fn->params; var; var = var->next) {
      printf("  mov %s, %d(%%rbp)\n", argreg[i++], var->offset);
    }

    // Emit code
    gen_stmt(fn->body);
    assert(depth == 0);

    // Epilogue
    printf(".L.return.%s:\n", fn->name);
    printf("  mov %%rbp, %%rsp\n");
    printf("  pop %%rbp\n");
    printf("  ret\n");
  }
}