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

pub void error_tok(TokenRef_t tok, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  verror_at(tok->loc, fmt, ap);
}

/// 如果 op 与当前 Token 匹配, 则消耗该 Token
pub bool equal(TokenRef_t tok, char* op) {
  return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

/// 确保当前 Token 与 s 匹配
pub TokenMut_t skip(TokenRef_t tok, char* s) {
  if (!equal(tok, s)) {
    error_tok(tok, "expected '%s'", s);
  }

  return tok->next;
}

/// 确保当前 Token 类型为 TK_NUM
pri int get_number(TokenRef_t tok) {
  if (tok->kind != TK_NUM) {
    error_tok(tok, "expected a number");
  }

  return tok->val;
}

pub bool consume(TokenMut_t* rest, TokenMut_t tok, char* str) {
  if (equal(tok, str)) {
    *rest = tok->next;
    return true;
  }

  *rest = tok;
  return false;
}

/// 创建 Token 实例
pri TokenMut_t new_token(TokenKind_e kind, char* start, char* end) {
  return new(Token_t, .kind = kind, .loc = start, .len = end - start);
}

pri bool startswitch(char* p, char* q) {
  return strncmp(p, q, strlen(q)) == 0;
}

/// 如果 `c` 作为标识符的第一个字符有效, 则返回true
pri bool is_ident1(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

/// 如果 `c` 作为标识符的非第一个字符有效, 则返回true
pri bool is_ident2(char c) {
  return is_ident1(c) || ('0' <= c && c <= '9');
}

/// 从 `p` 读取一个标点符号并返回其长度
pri int read_punct(char* p) {
  if (startswitch(p, "==") || startswitch(p, "!=") || startswitch(p, "<=") ||
      startswitch(p, ">=")) {
    return 2;
  }

  return ispunct(*p) ? 1 : 0;
}

pri bool is_keyword(TokenMut_t tok) {
  pri char* kw[] = {"return", "if", "else", "for", "while", "int", "sizeof", "char"};

  u_arr_each (kw, it) {
    if (equal(tok, it)) {
      return true;
    }
  }

  return false;
}

pri void convert_keywords(TokenMut_t tok) {
  for (TokenMut_t t = tok; tok->kind != TK_EOF; t = t->next) {
    if (is_keyword(tok)) {
      t->kind = TK_KEYWORD;
    }
  }
}

/// 对给定字符串进行标记并返回新的 Token
pub TokenMut_t tokenize(char* p) {
  current_input  = p;
  Token_t head   = {};
  TokenMut_t cur = &head;

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

    if (is_ident1(*p)) {
      char* start = p;
      do {
        p++;
      } while (is_ident2(*p));
      cur = cur->next = new_token(TK_IDENT, start, p);
      continue;
    }

    // 标识符与关键词
    int punct_len = read_punct(p);
    if (punct_len) {
      cur = cur->next = new_token(TK_PUNCT, p, p + punct_len);
      p += cur->len;
      continue;
    }

    error_at(p, "invalid token");
  }

  cur = cur->next = new_token(TK_EOF, p, p);
  convert_keywords(cur);

  return head.next;
}
