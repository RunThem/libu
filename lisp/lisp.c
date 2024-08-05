#include <linenoise.h>
#include <mpc.h>

#define LIBU_VA_EVAL
#include <u/u.h>

#define lisp_parse(name) mpc_parser_t* name = mpc_new(#name);
#define LISP_DEF_MAIN    lisp
#define LISP_DEF         number, symbol, sexpr, expr, LISP_DEF_MAIN

enum {
  LVAL_ERR,
  LVAL_NUM,
  LVAL_SYM,
  LVAL_SEXPR,
};

typedef struct lval_t lval_t, *lval_ref_t;
struct lval_t {
  int type;

  i64_t num;
  char* err;
  char* sym;

  /* #[[vec<lval_ref_t>]] */
  u_vec_t(lval_ref_t) cell;
};

pri lval_ref_t lval_num(i64_t x) {
  lval_ref_t v = u_talloc(lval_t);
  v->type      = LVAL_NUM;
  v->num       = x;
  return v;
}

pri lval_ref_t lval_err(char* m) {
  lval_ref_t v = u_talloc(lval_t);
  v->type      = LVAL_ERR;
  v->err       = strdup(m);
  return v;
}

pri lval_ref_t lval_sym(char* s) {
  lval_ref_t v = u_talloc(lval_t);
  v->type      = LVAL_SYM;
  v->sym       = strdup(s);
  return v;
}

pri lval_ref_t lval_sexpr() {
  lval_ref_t v = u_talloc(lval_t);
  v->type      = LVAL_SEXPR;
  v->cell      = u_vec_new(lval_ref_t);
  return v;
}

pri void lval_del(lval_ref_t v) {
  switch (v->type) {
    case LVAL_ERR: free(v->err); break;
    case LVAL_SYM: free(v->sym); break;

    case LVAL_SEXPR: {
      u_vec_for (v->cell, i, it) {
        u_free(it);
      }

      u_vec_cleanup(v->cell);
    } break;
  }

  u_free(v);
}

lval_ref_t lval_read_num(mpc_ast_t* t) {
  errno   = 0;
  i64_t x = strtol(t->contents, nullptr, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}

lval_ref_t lval_read(mpc_ast_t* t) {
  lval_ref_t x = nullptr;

  if (strstr(t->tag, "number")) {
    return lval_read_num(t);
  }

  if (strstr(t->tag, "symbol")) {
    return lval_sym(t->contents);
  }

  if (strcmp(t->tag, ">") == 0) {
    x = lval_sexpr();
  }

  if (strstr(t->tag, "sexpr")) {
    x = lval_sexpr();
  }

  u_each (i, t->children_num) {
    u_cnt_if(strcmp(t->children[i]->contents, "(") == 0);
    u_cnt_if(strcmp(t->children[i]->contents, ")") == 0);
    u_cnt_if(strcmp(t->children[i]->tag, "regex") == 0);

    u_vec_put(x->cell, lval_read(t->children[i]));
  }

  return x;
}

pri void lval_expr_print(lval_ref_t v, char open, char close);
pri void lval_print(lval_ref_t v) {
  switch (v->type) {
    case LVAL_NUM: printf("%li", v->num); break;
    case LVAL_ERR: printf("Error: %s", v->err); break;
    case LVAL_SYM: printf("%s", v->sym); break;
    case LVAL_SEXPR: lval_expr_print(v, '(', ')'); break;
  }
}

pri void lval_expr_print(lval_ref_t v, char open, char close) {
  putchar(open);

  u_vec_for (v->cell, i, it) {
    lval_print(it);
    if (i != u_vec_len(v->cell)) {
      putchar(' ');
    }
  }

  putchar(close);
}

pri void lval_println(lval_ref_t v) {
  lval_print(v);
  putchar('\n');
}

int main(int argc, const char* argv[]) {
  mpc_result_t result = {};

  u_va_mapof(lisp_parse, LISP_DEF);

  char* lisp_ =

      "                                                                                           "
      "number      : /-?[0-9]+/                                                                  ;"
      "symbol      : '+' | '-' | '*' | '/'                                                       ;"
      "sexpr       : '(' <expr>* ')'                                                             ;"
      "expr        : <number> | <symbol> | <sexpr>                                               ;"
      "lisp        : /^/ <expr>* /$/                                                             ;";

  mpca_lang(MPCA_LANG_DEFAULT, lisp_, LISP_DEF);

  puts("Lisp version 0.0.1");
  puts("Press Ctrl+c to exit\n");

#ifdef LINENOISE
  while (true) {
    char* input = linenoise("lisp> ");
    u_brk_if(input == nullptr || strcmp(input, "quit") == 0);

    linenoiseHistoryAdd(input);
#else
  char input[1024];
  fgets(input, sizeof(input), stdin);
#endif

    if (mpc_parse("<stdin>", input, LISP_DEF_MAIN, &result)) {

      lval_ref_t x = lval_read(result.output);
      lval_println(x);
      lval_del(x);

      mpc_ast_print(result.output);
      mpc_ast_delete(result.output);
    } else {
      mpc_err_print(result.error);
      mpc_err_delete(result.error);
    }

#ifdef LINENOISE
    linenoiseFree(input);
  }
#endif

  mpc_cleanup(u_va_cnt(LISP_DEF), LISP_DEF);

  return 0;
}
