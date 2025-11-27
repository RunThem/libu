#include <u/u.h>

///
/// Tokenizer
///

typedef enum {
  TK_PUNCT,  // 符号
  TK_NUM,    // 数字
  TK_EOF,    // 文件结束符标志
} token_kind_e;

u_struct_def(token) {
  token_kind_e kind;  // Token 类型
  token_mut_t next;   // 指向下一个 Token

  int val;    // 如果 .kind == TK_NUM
  char* loc;  // Token 的起始位置
  int len;    // Token 的长度
};

/// 输入字符串
pri char* current_input;

/// 报错并退出程序
pri void error(char* fmt, ...) {
  va_list ap = {};
  va_start(ap, fmt);

  vfprintf(stderr, fmt, ap);

  va_end(ap);
  exit(1);
}

// 报告错误位置并退出程序
pri void verror_at(char* loc, char* fmt, va_list ap) {
  int pos = loc - current_input;
  fprintf(stderr, "%s\n", current_input);
  fprintf(stderr, "%*s", pos, "");  // 打印多个空格
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

pri void error_at(char* loc, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  verror_at(loc, fmt, ap);
}

pri void error_tok(token_ref_t tok, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  verror_at(tok->loc, fmt, ap);
}

/// 如果 op 与当前 Token 匹配, 则消耗该 Token
pri bool equal(token_ref_t tok, char* op) {
  return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

/// 确保当前 Token 与 s 匹配
pri token_mut_t skip(token_ref_t tok, char* s) {
  if (!equal(tok, s)) {
    error_tok(tok, "expected '%s'", s);
  }

  return tok->next;
}

/// 确保当前 Token 类型为 TK_NUM
pri int get_number(token_ref_t tok) {
  if (tok->kind != TK_NUM) {
    error_tok(tok, "expected a number");
  }

  return tok->val;
}

/// 创建 Token 实例
pri token_mut_t new_token(token_kind_e kind, char* start, char* end) {
  return new(token_t, .kind = kind, .loc = start, .len = end - start);
}

pri bool startswitch(char* p, char* q) {
  return strncmp(p, q, strlen(q)) == 0;
}

/// 从 `p` 读取一个标点符号并返回其长度
pri int read_punct(char* p) {
  if (startswitch(p, "==") || startswitch(p, "!=") || startswitch(p, "<=") ||
      startswitch(p, ">=")) {
    return 2;
  }

  return ispunct(*p) ? 1 : 0;
}

/// 对 `current_input` 进行分词并返回新的 Token
pri token_mut_t tokenize() {
  char* p         = current_input;
  token_t head    = {};
  token_mut_t cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (isdigit(*p)) {
      cur = cur->next = new_token(TK_NUM, p, p);
      char* q         = p;
      cur->val        = (int)strtoul(p, &p, 10);
      cur->len        = (int)(p - q);
      continue;
    }

    int punct_len = read_punct(p);
    if (punct_len) {
      cur = cur->next = new_token(TK_PUNCT, p, p + punct_len);
      p += cur->len;
      continue;
    }

    error_at(p, "invalid token");
  }

  cur = cur->next = new_token(TK_EOF, p, p);

  return head.next;
}

///
/// Parser
///

typedef enum {
  ND_ADD,  // +
  ND_SUB,  // -
  ND_MUL,  // *
  ND_DIV,  // /
  ND_NEG,  // unary -
  ND_EQ,   // ==
  ND_NE,   // !=
  ND_LT,   // <
  ND_LE,   // <=
  ND_NUM,  // 数字
} node_kind_e;

/// Ast 节点
u_struct_def(node) {
  node_kind_e kind;  // 节点类型
  node_mut_t lhs;    // 左子节点
  node_mut_t rhs;    // 右子节点
  int val;           // 如果 .kind == ND_NUM
};

pri node_mut_t new_node(node_kind_e kind) {
  return new(node_t, .kind = kind);
}

pri node_mut_t new_binary(node_kind_e kind, node_mut_t lhs, node_mut_t rhs) {
  return new(node_t, .kind = kind, .lhs = lhs, .rhs = rhs);
}

pri node_mut_t new_unary(node_kind_e kind, node_mut_t expr) {
  return new(node_t, .kind = kind, .lhs = expr);
}

pri node_mut_t new_number(int val) {
  return new(node_t, .kind = ND_NUM, .val = val);
}

pri node_mut_t expr(token_mut_t* rest, token_mut_t tok);
pri node_mut_t equality(token_mut_t* rest, token_mut_t tok);
pri node_mut_t relational(token_mut_t* rest, token_mut_t tok);
pri node_mut_t add(token_mut_t* rest, token_mut_t tok);
pri node_mut_t mul(token_mut_t* rest, token_mut_t tok);
pri node_mut_t unary(token_mut_t* rest, token_mut_t tok);
pri node_mut_t primary(token_mut_t* rest, token_mut_t tok);

/// expr = equality
pri node_mut_t expr(token_mut_t* rest, token_mut_t tok) {
  return equality(rest, tok);
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

/// primary = "(" expr ")" | num
pri node_mut_t primary(token_mut_t* rest, token_mut_t tok) {
  if (equal(tok, "(")) {
    node_mut_t node = expr(&tok, tok->next);
    *rest           = skip(tok, ")");
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

///
/// Code generator
///

pri int depth;

pri void push() {
  printf("  push %%rax\n");
  depth++;
}

pri void pop(char* arg) {
  printf("  pop %s\n", arg);
  depth--;
}

pri void gen_expr(node_ref_t node) {
  switch (node->kind) {
    case ND_NUM: printf("  mov $%d, %%rax\n", node->val); return;
    case ND_NEG:
      gen_expr(node->lhs);
      printf("  neg %%rax\n");
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

int main(int argc, const char* argv[]) {
  u_chk_if(argc != 2, 1, "%s: invalid number of arguments\n", argv[0]);

  current_input   = (char*)argv[1];
  token_mut_t tok = tokenize();
  node_ref_t node = expr(&tok, tok);

  if (tok->kind != TK_EOF) {
    error_tok(tok, "extra token");
  }

  printf("  .globl main\n");
  printf("main:\n");

  gen_expr(node);
  printf("  ret\n");

  assert(depth == 0);

  return 0;
}
