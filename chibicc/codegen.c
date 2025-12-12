#include "chibicc.h"

pri int depth;

pri char* argreg8[]  = {"%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b"};
pri char* argreg64[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

pri ObjMut_t current_fn = nullptr;

pri void gen_expr(NodeRef_t node);

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
pri void gen_addr(NodeRef_t node) {
  switch (node->kind) {
    case ND_VAR:
      if (node->var->is_local) {
        // 局部变量
        printf("  lea %d(%%rbp), %%rax\n", node->var->offset);
      } else {
        // 全局变量
        printf("  lea %s(%%rip), %%rax\n", node->var->name);
      }
      return;

    case ND_DEREF: gen_expr(node->lhs); return;

    default: break;
  }

  error_tok(node->tok, "not an lvalue");
}

struct Api;
typedef struct Api Api_t;
typedef struct Api* ApiMut_t;
typedef const struct Api* ApiRef_t;
struct Api { };

/// 从 %rax 所指向的位置加载一个值
pri void load(TypeMut_t ty) {
  if (ty->kind == TY_ARRAY) {
    // 如果是一个数组, 不要尝试将值加载到寄存器中, 因为通常我们无法将整个数组加载到寄存器中.
    // 因此, 对数组求值的结果不是数组本身, 而是数组的地址.
    // 这就是 C 语言中 '数组会自动转换为指向数组第一个元素的指针' 的情况.
    return;
  }

  if (ty->size == 1) {
    printf("  movsbq (%%rax), %%rax\n");
  } else {
    printf("  mov (%%rax), %%rax\n");
  }
}

/// 将 %rax 寄存器中的值存储到栈顶所指向的地址中
pri void store(TypeMut_t ty) {
  pop("%rdi");

  if (ty->size == 1) {
    printf("  mov %%al, (%%rdi)\n");
  } else {
    printf("  mov %%rax, (%%rdi)\n");
  }
}

/// 为给定节点生成代码
pri void gen_expr(NodeRef_t node) {
  switch (node->kind) {
    case ND_NUM: printf("  mov $%d, %%rax\n", node->val); return;
    case ND_NEG:
      gen_expr(node->lhs);
      printf("  neg %%rax\n");
      return;
    case ND_VAR:
      gen_addr(node);
      load(node->ty);
      return;
    case ND_DEREF:
      gen_expr(node->lhs);
      load(node->ty);
      return;
    case ND_ADDR: gen_addr(node->lhs); return;
    case ND_ASSIGN:
      gen_addr(node->lhs);
      push();
      gen_expr(node->rhs);
      store(node->ty);
      return;
    case ND_FUNCALL: {

      int nargs = 0;
      for (NodeMut_t arg = node->args; arg; arg = arg->next) {
        gen_expr(arg);
        push();
        nargs++;
      }

      for (int i = nargs - 1; i >= 0; i--) {
        pop(argreg64[i]);
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

pri void gen_stmt(NodeMut_t node) {
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
      for (NodeMut_t n = node->body; n; n = n->next) {
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

pri void assign_lvar_offsets(ObjMut_t prog) {
  for (ObjMut_t fn = prog; fn; fn = fn->next) {
    u_cnt_if(!fn->is_function);

    int offset = 0;

    for (ObjMut_t var = fn->locals; var; var = var->next) {
      offset += var->ty->size;
      var->offset = -offset;
    }

    fn->stack_size = u_align_of(offset, 16);
  }
}

pri void emit_data(ObjMut_t prog) {
  for (ObjMut_t var = prog; var; var = var->next) {
    u_cnt_if(var->is_function);

    printf("  .data\n");
    printf("  .globl %s\n", var->name);
    printf("%s:\n", var->name);
    printf("  .zero %d\n", var->ty->size);
  }
}

pri void emit_text(ObjMut_t prog) {
  for (ObjMut_t fn = prog; fn; fn = fn->next) {
    u_cnt_if(!fn->is_function);

    printf("  .globl %s\n", fn->name);
    printf("  .text\n");
    printf("%s:\n", fn->name);
    current_fn = fn;

    // Prologue
    printf("  push %%rbp\n");
    printf("  mov %%rsp, %%rbp\n");
    printf("  sub $%d, %%rsp\n", fn->stack_size);

    int i = 0;
    for (ObjMut_t var = fn->params; var; var = var->next) {
      if (var->ty->size == 1) {
        printf("  mov %s, %d(%%rbp)\n", argreg8[i++], var->offset);
      } else {
        printf("  mov %s, %d(%%rbp)\n", argreg64[i++], var->offset);
      }
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

pub void codegen(ObjMut_t prog) {
  assign_lvar_offsets(prog);
  emit_data(prog);
  emit_text(prog);
}