#include <u/u.h>

typedef struct type* type_mut_t;
typedef struct node* node_mut_t;

///
/// Tokenizer
///

typedef enum {
  TK_IDENT,    // 标识符
  TK_PUNCT,    // 符号
  TK_KEYWORD,  // 关键字
  TK_NUM,      // 数字
  TK_EOF,      // 文件结束符标志
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
pub bool consume(token_mut_t* rest, token_mut_t tok, char* str);
pub token_mut_t tokenize(char* p);

///
/// Parser
///

/// 局部变量
u_struct_def(obj) {
  obj_mut_t next;
  char* name;     // 变量名
  type_mut_t ty;  // 类型
  int offset;     // 栈偏移量
};

u_struct_def(function) {
  node_mut_t body;
  obj_mut_t locals;
  int stack_size;
};

typedef enum {
  ND_ADD,        // +
  ND_SUB,        // -
  ND_MUL,        // *
  ND_DIV,        // /
  ND_NEG,        // unary -
  ND_EQ,         // ==
  ND_NE,         // !=
  ND_LT,         // <
  ND_LE,         // <=
  ND_ASSIGN,     // =
  ND_ADDR,       // unary &
  ND_DEREF,      // unary *
  ND_RETURN,     // return
  ND_IF,         // if
  ND_FOR,        // for, while
  ND_BLOCK,      // { ... }
  ND_EXPR_STMT,  // 表达式语句
  ND_VAR,        // 变量
  ND_NUM,        // 数字
} node_kind_e;

/// Ast 节点
u_struct_def(node) {
  node_kind_e kind;  // 节点类型
  node_mut_t next;   // 下一个节点
  type_mut_t ty;     // 类型, 如 int 或 int*
  token_mut_t tok;   // 代表 Token

  node_mut_t lhs;  // 左子节点
  node_mut_t rhs;  // 右子节点

  // if 或 for 语句
  node_mut_t cond;
  node_mut_t then;
  node_mut_t els;
  node_mut_t init;
  node_mut_t inc;

  // 块
  node_mut_t body;

  obj_mut_t var;  // 如果 .kind == ND_VAR
  int val;        // 如果 .kind == ND_NUM
};

pub function_mut_t parse(token_mut_t tok);

///
/// Type checker
///

typedef enum {
  TY_INT,
  TY_PTR,
} type_kind_e;

u_struct_def(type) {
  type_kind_e kind;

  // 指针
  type_mut_t base;

  // 声明
  token_mut_t name;
};

extern pub type_mut_t ty_int;

pub bool is_integer(type_mut_t ty);
pub type_mut_t pointer_to(type_mut_t base);
pub void add_type(node_mut_t node);

///
/// Code generator
///

pub void codegen(function_mut_t prog);

///
/// Dump
///

pub void dump(function_ref_t prog);