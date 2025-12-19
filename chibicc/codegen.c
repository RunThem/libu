#include "chibicc.h"

pri FILE* output_file;

pri int depth;

pri char* argreg8[]  = {"%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b"};
pri char* argreg64[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

pri ObjMut_t current_fn = nullptr;

pri void gen_expr(NodeMut_t node);
pri void gen_stmt(NodeMut_t node);

pri void println(char* fmt, ...) {
  va_list ap = {};
  va_start(ap, fmt);
  vfprintf(output_file, fmt, ap);
  va_end(ap);

  fprintf(output_file, "\n");
}
pri int count() {
  pri int i = 1;
  return i++;
}

pri void push() {
  println("  push %%rax");
  depth++;
}

pri void pop(char* arg) {
  println("  pop %s", arg);
  depth--;
}

/// 计算给定节点的绝对地址, 如果给定的节点不在内存中, 则会产生错误
pri void gen_addr(NodeRef_t node) {
  switch (node->kind) {
    case ND_VAR:
      if (node->var->is_local) {
        // 局部变量
        println("  lea %d(%%rbp), %%rax", node->var->offset);
      } else {
        // 全局变量
        println("  lea %s(%%rip), %%rax", node->var->name);
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
    println("  movsbq (%%rax), %%rax");
  } else {
    println("  mov (%%rax), %%rax");
  }
}

/// 将 %rax 寄存器中的值存储到栈顶所指向的地址中
pri void store(TypeMut_t ty) {
  pop("%rdi");

  if (ty->size == 1) {
    println("  mov %%al, (%%rdi)");
  } else {
    println("  mov %%rax, (%%rdi)");
  }
}

/// 为给定节点生成代码
pri void gen_expr(NodeMut_t node) {
  switch (node->kind) {
    case ND_NUM: println("  mov $%d, %%rax", node->val); return;
    case ND_NEG:
      gen_expr(node->lhs);
      println("  neg %%rax");
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
    case ND_STMT_EXPR:
      for (NodeMut_t n = node->body; n; n = n->next) {
        gen_stmt(n);
      }
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

      println("  mov $0, %%rax");
      println("  call %s", node->funcname);
      return;
    }

    default: break;
  }

  gen_expr(node->rhs);
  push();
  gen_expr(node->lhs);
  pop("%rdi");

  switch (node->kind) {
    case ND_ADD: println("  add %%rdi, %%rax"); return;
    case ND_SUB: println("  sub %%rdi, %%rax"); return;
    case ND_MUL: println("  imul %%rdi, %%rax"); return;
    case ND_DIV:
      println("  cqo");
      println("  idiv %%rdi");
      return;
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
      println("  cmp %%rdi, %%rax");

      if (node->kind == ND_EQ) {
        println("  sete %%al");
      } else if (node->kind == ND_NE) {
        println("  setne %%al");
      } else if (node->kind == ND_LT) {
        println("  setl %%al");
      } else if (node->kind == ND_LE) {
        println("  setle %%al");
      }

      println("  movzb %%al, %%rax");
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
      println("  cmp $0, %%rax");
      println("  je .L.else.%d", c);
      gen_stmt(node->then);
      println("  jmp .L.end.%d", c);
      println(".L.else.%d:", c);
      if (node->els) {
        gen_stmt(node->els);
      }
      println(".L.end.%d:", c);
      return;
    }
    case ND_FOR: {
      int c = count();
      if (node->init) {
        gen_stmt(node->init);
      }

      println(".L.begin.%d:", c);
      if (node->cond) {
        gen_expr(node->cond);
        println("  cmp $0, %%rax");
        println("  je .L.end.%d", c);
      }
      gen_stmt(node->then);
      if (node->inc) {
        gen_expr(node->inc);
      }
      println("  jmp .L.begin.%d", c);
      println(".L.end.%d:", c);
      return;
    }
    case ND_BLOCK:
      for (NodeMut_t n = node->body; n; n = n->next) {
        gen_stmt(n);
      }
      return;
    case ND_RETURN:
      gen_expr(node->lhs);
      println("  jmp .L.return.%s", current_fn->name);
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

    println("  .data");
    println("  .globl %s", var->name);
    println("%s:", var->name);

    if (var->init_data) {
      for (int i = 0; i < var->ty->size; i++) {
        println("  .byte %d", var->init_data[i]);
      }
    } else {
      println("  .zero %d", var->ty->size);
    }
  }
}

pri void emit_text(ObjMut_t prog) {
  for (ObjMut_t fn = prog; fn; fn = fn->next) {
    u_cnt_if(!fn->is_function);

    println("  .globl %s", fn->name);
    println("  .text");
    println("%s:", fn->name);
    current_fn = fn;

    // Prologue
    println("  push %%rbp");
    println("  mov %%rsp, %%rbp");
    println("  sub $%d, %%rsp", fn->stack_size);

    int i = 0;
    for (ObjMut_t var = fn->params; var; var = var->next) {
      if (var->ty->size == 1) {
        println("  mov %s, %d(%%rbp)", argreg8[i++], var->offset);
      } else {
        println("  mov %s, %d(%%rbp)", argreg64[i++], var->offset);
      }
    }

    // Emit code
    gen_stmt(fn->body);
    assert(depth == 0);

    // Epilogue
    println(".L.return.%s:", fn->name);
    println("  mov %%rbp, %%rsp");
    println("  pop %%rbp");
    println("  ret");
  }
}

pub void codegen(ObjMut_t prog, FILE* out) {
  output_file = out;

  assign_lvar_offsets(prog);
  emit_data(prog);
  emit_text(prog);
}