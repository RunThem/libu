#include "chibicc.h"

int main(int argc, const char* argv[]) {
  u_chk_if(argc != 2, 1, "%s: invalid number of arguments\n", argv[0]);

  token_mut_t tok = tokenize((char*)argv[1]);
  node_mut_t node = parse(tok);
  codegen(node);

  return 0;
}
