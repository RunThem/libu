#include "chibicc.h"

int main(int argc, const char* argv[]) {
  u_chk_if(argc != 2, 1, "%s: invalid number of arguments\n", argv[0]);

  // 词法分析与语法解析
  TokenMut_t tok = tokenize_file((char*)argv[1]);
  ObjMut_t prog  = parse(tok);

  // dump(prog);

  // 遍历 AST 以写入汇编
  codegen(prog);

  return 0;
}
