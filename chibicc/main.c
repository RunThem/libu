#include <u/u.h>

int main(int argc, const char* argv[]) {
  u_chk_if(argc != 2, 1, "%s: invalid number of arguments\n", argv[0]);

  printf("  .globl main\n");
  printf("main:\n");
  printf("  mov $%d, %%rax\n", atoi(argv[1]));
  printf("  ret\n");

  return 0;
}
