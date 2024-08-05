#include <linenoise.h>
#include <mpc.h>

#define LIBU_VA_EVAL
#include <u/u.h>

#define lisp_parse(name) mpc_parser_t* name = mpc_new(#name);
#define LISP_DEF_MAIN    lisp
#define LISP_DEF         LISP_DEF_MAIN

int main(int argc, const char* argv[]) {
  mpc_result_t result = {};

  u_va_mapof(lisp_parse, LISP_DEF);

  char* lisp_ =

      "                                                                        "
      "lisp    : /^/ /$/                                                     ;";

  mpca_lang(MPCA_LANG_DEFAULT, lisp_, LISP_DEF);

  puts("Lisp version 0.0.1");
  puts("Press Ctrl+c to exit\n");

  while (true) {
    char* input = linenoise("lisp> ");
    u_brk_if(input == nullptr || strcmp(input, "quit") == 0);

    linenoiseHistoryAdd(input);

    if (mpc_parse("<stdin>", input, LISP_DEF_MAIN, &result)) {
      mpc_ast_print(result.output);
      mpc_ast_delete(result.output);
    } else {
      mpc_err_print(result.error);
      mpc_err_delete(result.error);
    }

    linenoiseFree(input);
  }

  mpc_cleanup(u_va_cnt(LISP_DEF), LISP_DEF);

  return 0;
}
