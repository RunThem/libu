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

/// 报错并退出程序
pub void error(char* fmt, ...);
pub void error_at(char* loc, char* fmt, ...);
pub void error_tok(token_ref_t tok, char* fmt, ...);
pub bool equal(token_ref_t tok, char* op);
pub token_mut_t skip(token_ref_t tok, char* s);
pub token_mut_t tokenize(char* p);

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

pub node_mut_t parse(token_mut_t tok);

///
/// Code generator
///

pub void codegen(node_mut_t node);
