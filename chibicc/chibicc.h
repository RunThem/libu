#include <u/u.h>

#undef u_struct_def
#define u_struct_def(T, ...)                                                                       \
  struct T;                                                                                        \
  typedef typeof(struct T) T##_t;                                                                  \
  typedef typeof(struct T*) T##Mut_t;                                                              \
  typedef typeof(const struct T*) T##Ref_t;                                                        \
  struct __VA_ARGS__ T

u_struct_def(Type);
u_struct_def(Node);

///
/// String
///

pub char* format(char* fmt, ...);

///
/// Tokenizer
///

typedef enum {
  TK_IDENT,    // 标识符
  TK_PUNCT,    // 符号
  TK_KEYWORD,  // 关键字
  TK_STR,      // 字符串
  TK_NUM,      // 数字
  TK_EOF,      // 文件结束符标志
} TokenKind_e;

u_struct_def(Token) {
  TokenKind_e kind;  // Token 类型
  TokenMut_t next;   // 指向下一个 Token

  int val;       // 如果 .kind == TK_NUM
  char* loc;     // Token 的起始位置
  int len;       // Token 的长度
  TypeMut_t ty;  // 如果是 TK_STR
  char* str;     // 字符串字面量, 包括括终止 '\0'
};

/// 报错并退出程序
pub void error(char* fmt, ...);
pub void error_at(char* loc, char* fmt, ...);
pub void error_tok(TokenRef_t tok, char* fmt, ...);
pub bool equal(TokenRef_t tok, char* op);
pub TokenMut_t skip(TokenRef_t tok, char* s);
pub bool consume(TokenMut_t* rest, TokenMut_t tok, char* str);
pub TokenMut_t tokenize(char* p);

///
/// Parser
///

/// 变量或函数
u_struct_def(Obj) {
  ObjMut_t next;
  char* name;     // 变量名
  TypeMut_t ty;   // 类型
  bool is_local;  // 局部或全局/函数

  // 局部变量
  int offset;

  // 全局变脸或函数
  bool is_function;

  // 全局变量
  char* init_data;

  // 函数
  ObjMut_t params;
  NodeMut_t body;
  ObjMut_t locals;
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
  ND_FUNCALL,    // 函数调用
  ND_EXPR_STMT,  // 表达式语句
  ND_STMT_EXPR,  // 语句表达式
  ND_VAR,        // 变量
  ND_NUM,        // 数字
} NodeKind_e;

/// Ast 节点
u_struct_def(Node) {
  NodeKind_e kind;  // 节点类型
  NodeMut_t next;   // 下一个节点
  TypeMut_t ty;     // 类型, 如 int 或 int*
  TokenMut_t tok;   // 代表 Token

  NodeMut_t lhs;  // 左子节点
  NodeMut_t rhs;  // 右子节点

  // if 或 for 语句
  NodeMut_t cond;
  NodeMut_t then;
  NodeMut_t els;
  NodeMut_t init;
  NodeMut_t inc;

  // 块或语句表达式
  NodeMut_t body;

  // 函数名
  char* funcname;
  NodeMut_t args;

  ObjMut_t var;  // 如果 .kind == ND_VAR
  int val;       // 如果 .kind == ND_NUM
};

pub ObjMut_t parse(TokenMut_t tok);

///
/// Type checker
///

typedef enum {
  TY_CHAR,
  TY_INT,
  TY_PTR,
  TY_FUNC,
  TY_ARRAY,
} TypeKind_e;

u_struct_def(Type) {
  TypeKind_e kind;
  int size;  // sizeof() 值

  // 指针类型或数组类型, 我们有意使用相同的成员来表示 C 语言中指针/数组的二元性.
  // 在许多期望指针的上下文中, 我们检查此成员而非 .kind 成员来确定某个类型是否为指针.
  // 这意味着在许多上下文中, 'T 类型的数组' 会自然地被当作 '指向 T 的指针' 来处理,
  // 这符合 C 语言规范的要求.
  TypeMut_t base;

  // 声明
  TokenMut_t name;

  // 数组
  int array_len;

  // 函数类型
  TypeMut_t return_ty;
  TypeMut_t params;
  TypeMut_t next;
};

extern pub TypeMut_t ty_char;
extern pub TypeMut_t ty_int;

pub bool is_integer(TypeMut_t ty);
pub TypeMut_t copy_type(TypeMut_t ty);
pub TypeMut_t pointer_to(TypeMut_t base);
pub TypeMut_t func_type(TypeMut_t return_ty);
pub TypeMut_t array_of(TypeMut_t base, int size);
pub void add_type(NodeMut_t node);

///
/// Code generator
///

pub void codegen(ObjMut_t prog);

///
/// Dump
///

pub void dump(ObjMut_t prog);