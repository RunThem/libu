#include "chibicc.h"

/// 输入文件名
pri char* current_filename;

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

// 报告一下格式的错误消息并退出程序
//
// foo.c:10: x = y + 1;
//               ^ <error message here>
pri void verror_at(char* loc, char* fmt, va_list ap) {
  // 查找行内容位置
  char* line = loc;
  while (current_input < line && line[-1] != '\n') {
    line--;
  }

  char* end = loc;
  while (*end != '\n') {
    end++;
  }

  // 获取行号
  int line_no = 1;
  for (char* p = current_input; p < line; p++) {
    if (*p == '\n') {
      line_no++;
    }
  }

  // 打印行号
  int ident = fprintf(stderr, "%s:%d: ", current_filename, line_no);
  fprintf(stderr, "%.*s\n", (int)(end - line), line);

  // 打印错误信息
  int pos = loc - line + ident;

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
  return new (Token_t, .kind = kind, .loc = start, .len = end - start);
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

pri int from_hex(char c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  }

  if ('a' <= c && c <= 'f') {
    return c - 'a' + 10;
  }

  return c - 'A' + 10;
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

pri int read_escaped_char(char** new_pos, char* p) {
  if ('0' <= *p && *p <= '7') {
    int c = *p++ - '0';
    if ('0' <= *p && *p <= '7') {
      c = (c << 3) + (*p++ - '0');
      if ('0' <= *p && *p <= '7') {
        c = (c << 3) + (*p++ - '0');
      }
    }

    *new_pos = p;
    return c;
  }

  if (*p == 'x') {
    p++;
    if (!isxdigit(*p)) {
      error_at(p, "invalid hex escape sequence");
    }

    int c = 0;
    for (; isxdigit(*p); p++) {
      c = (c << 4) + from_hex(*p);
    }

    *new_pos = p;
    return c;
  }

  *new_pos = p + 1;

  switch (*p) {
    case 'a': return '\a';
    case 'b': return '\b';
    case 't': return '\t';
    case 'n': return '\n';
    case 'v': return '\v';
    case 'f': return '\f';
    case 'r': return '\r';
    case 'e': return 27;

    default: return *p;
  }
}

// 寻找反双引号
pri char* string_literal_end(char* p) {
  char* start = p;

  for (; *p != '"'; p++) {
    if (*p == '\n' || *p == '\0') {
      error_at(start, "unclosed string literal");
    }

    if (*p == '\\') {
      p++;
    }
  }

  return p;
}

pri TokenMut_t read_string_literal(char* start) {
  char* end = string_literal_end(start + 1);
  char* buf = u_zalloc(end - start);
  int len   = 0;

  for (char* p = start + 1; p < end;) {
    if (*p == '\\') {
      buf[len++] = (char)read_escaped_char(&p, p + 1);
    } else {
      buf[len++] = *p++;
    }
  }

  TokenMut_t tok = new_token(TK_STR, start, end + 1);
  tok->ty        = array_of(ty_char, len + 1);
  tok->str       = buf;
  return tok;
}

pri void convert_keywords(TokenMut_t tok) {
  for (TokenMut_t t = tok; tok->kind != TK_EOF; t = t->next) {
    if (is_keyword(tok)) {
      t->kind = TK_KEYWORD;
    }
  }
}

/// 对给定字符串进行标记并返回新的 Token
pub TokenMut_t tokenize(char* filename, char* p) {
  current_filename = filename;
  current_input    = p;
  Token_t head     = {};
  TokenMut_t cur   = &head;

  while (*p) {

    // 跳过行注释
    if (startswitch(p, "//")) {
      p += 2;
      while (*p != '\n') {
        p++;
      }

      continue;
    }

    // 跳过块注释
    if (startswitch(p, "/*")) {
      char* q = strstr(p + 2, "*/");
      if (!q) {
        error_at(p, "unclosed block comment");
      }

      p = q + 2;
      continue;
    }

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

    if (*p == '"') {
      cur = cur->next = read_string_literal(p);
      p += cur->len;
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

/// 返回给出的文件内容
pri char* read_file(char* path) {
  FILE* fp = {};

  if (strcmp(path, "-") == 0) {
    fp = stdin;
  } else {
    fp = fopen(path, "r");
    if (!fp) {
      error("cannot open %s: %s", path, strerror(errno));
    }
  }

  char* buf     = {};
  size_t buflen = {};
  FILE* out     = open_memstream(&buf, &buflen);

  for (;;) {
    char buf2[4096] = {};
    int n           = fread(buf2, 1, sizeof(buf2), fp);
    u_brk_if(n == 0);

    fwrite(buf2, 1, n, out);
  }

  if (fp != stdin) {
    fclose(fp);
  }

  fflush(out);
  if (buflen == 0 || buf[buflen - 1] != '\n') {
    fputc('\n', out);
  }
  fputc('\0', out);
  fclose(out);

  return buf;
}

pub TokenMut_t tokenize_file(char* path) {
  return tokenize(path, read_file(path));
}