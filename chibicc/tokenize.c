#include "chibicc.h"

/// 输入字符串
pri char* current_input;

/// 报错并退出程序
pub void error(char* fmt, ...) {
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

pub void error_at(char* loc, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  verror_at(loc, fmt, ap);
}

pub void error_tok(token_ref_t tok, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  verror_at(tok->loc, fmt, ap);
}

/// 如果 op 与当前 Token 匹配, 则消耗该 Token
pub bool equal(token_ref_t tok, char* op) {
  return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

/// 确保当前 Token 与 s 匹配
pub token_mut_t skip(token_ref_t tok, char* s) {
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
pub token_mut_t tokenize(char* p) {
  current_input   = p;
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
