#include <u/u.h>

int main(int argc, const char* argv[]) {
  u_chk_if(argc != 2, 1, "%s: invalid number of arguments\n", argv[0]);

  char* p = (char*)argv[1];

  printf("  .globl main\n");
  printf("main:\n");
  printf("  mov $%ld, %%rax\n", strtol(p, &p, 10));

  while (*p) {
    if (*p == '+') {
      p++;
      printf("  add $%ld, %%rax\n", strtol(p, &p, 10));
      continue;
    }

    if (*p == '-') {
      p++;
      printf("  sub $%ld, %%rax\n", strtol(p, &p, 10));
      continue;
    }

    u_err("unexpected character: '%c'", *p);
  }

  printf("  ret\n");

  return 0;
}
