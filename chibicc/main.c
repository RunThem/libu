#include <u/u.h>

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

/// 报错并退出程序
pri void error(char* fmt, ...) {
  va_list ap = {};
  va_start(ap, fmt);

  vfprintf(stderr, fmt, ap);

  va_end(ap);
  exit(1);
}

/// 如果 op 与当前 Token 匹配, 则消耗该 Token
pri bool equal(token_ref_t tok, char* op) {
  return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

/// 确保当前 Token 与 s 匹配
pri token_mut_t skip(token_ref_t tok, char* s) {
  if (!equal(tok, s)) {
    error("expected '%s'", s);
  }

  return tok->next;
}

/// 确保当前 Token 类型为 TK_NUM
pri int get_number(token_ref_t tok) {
  if (tok->kind != TK_NUM) {
    error("expected a number");
  }

  return tok->val;
}

/// 创建 Token 实例
pri token_mut_t new_token(token_kind_e kind, char* start, char* end) {
  return new (token_t, .kind = kind, .loc = start, .len = end - start);
}

/// 对 `p` 进行分词并返回新的 Token
pri token_mut_t tokenize(char* p) {
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

    if (*p == '+' || *p == '-') {
      cur = cur->next = new_token(TK_PUNCT, p, p + 1);
      p++;
      continue;
    }

    error("invalid token");
  }

  cur = cur->next = new_token(TK_EOF, p, p);

  return head.next;
}

int main(int argc, const char* argv[]) {
  u_chk_if(argc != 2, 1, "%s: invalid number of arguments\n", argv[0]);

  token_ref_t tok = tokenize((char*)argv[1]);

  printf("  .globl main\n");
  printf("main:\n");

  /// 第一个 Token 必须是数字
  printf("  mov $%d, %%rax\n", get_number(tok));
  tok = tok->next;

  while (tok->kind != TK_EOF) {
    if (equal(tok, "+")) {
      printf("  add $%d, %%rax\n", get_number(tok->next));
      tok = tok->next->next;
      continue;
    }

    tok = skip(tok, "-");
    printf("  sub $%d, %%rax\n", get_number(tok));

    tok = tok->next;
  }

  printf("  ret\n");

  return 0;
}
