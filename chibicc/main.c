#include "chibicc.h"

int main(int argc, const char* argv[]) {
  u_chk_if(argc != 2, 1, "%s: invalid number of arguments\n", argv[0]);

  token_mut_t tok     = tokenize((char*)argv[1]);
  function_mut_t prog = parse(tok);

  // dump(prog);

  // 遍历 AST 以写入汇编
  codegen(prog);

  return 0;
}
