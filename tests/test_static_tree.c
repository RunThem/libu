#include <stdio.h>
#include "mut.h"

#define U_PRI
#include <u/pri.h>

TREE_DEFINE(int, int, k > n->k ? 1 : (k == n->k ? 0 : -1))

test() {
  auto m = tree_new();
  mut_n(m);

  mut_e(0, tree_len(m));
  mut_t(tree_is_empty(m));

  mut_f(tree_is_exist(m, 0));
  mut_f(tree_is_exist(m, 1));
  mut_f(tree_is_exist(m, 2));

  tree_put(m, 1, -1);
  tree_put(m, 2, -2);
  tree_put(m, 3, -3);

  mut_e(3, tree_len(m));
  mut_f(tree_is_empty(m));

  mut_t(tree_is_exist(m, 1));
  mut_t(tree_is_exist(m, 2));
  mut_t(tree_is_exist(m, 3));

  mut_f(tree_is_exist(m, 4));
  mut_f(tree_is_exist(m, 5));
  mut_f(tree_is_exist(m, -1));
  mut_f(tree_is_exist(m, -2));

  tree_re(m, 1, -101);
  tree_re(m, 2, -102);
  tree_re(m, 3, -103);

  mut_e(3, tree_len(m));
  mut_f(tree_is_empty(m));

  mut_t(tree_is_exist(m, 1));
  mut_t(tree_is_exist(m, 2));
  mut_t(tree_is_exist(m, 3));

  mut_f(tree_is_exist(m, 4));
  mut_f(tree_is_exist(m, 5));
  mut_f(tree_is_exist(m, -1));
  mut_f(tree_is_exist(m, -2));

  mut_e(-101, tree_at(m, 1));
  mut_e(-102, tree_at(m, 2));
  mut_e(-103, tree_at(m, 3));

  // tree_try(m, 1) {
  //   mut_e(-101, *it);
  //   *it = *it * 2;
  // }

  auto v = tree_at(m, 1);
  tree_re(m, 1, v * 2);

  mut_e(3, tree_len(m));
  mut_t(tree_is_exist(m, 1));
  mut_e(-202, tree_at(m, 1));

  tree_clear(m);

  mut_e(0, tree_len(m));
  mut_t(tree_is_empty(m));

  tree_cleanup(m);

  /* init */
  m = tree_new();
  mut_n(m);

  mut_e(0, tree_len(m));
  mut_t(tree_is_empty(m));

  mut_f(tree_is_exist(m, 0));
  mut_f(tree_is_exist(m, 1));
  mut_f(tree_is_exist(m, 2));

  tree_put(m, 1, -1);
  tree_put(m, 2, -2);
  tree_put(m, 3, -3);

  mut_e(3, tree_len(m));
  mut_f(tree_is_empty(m));

  mut_t(tree_is_exist(m, 1));
  mut_t(tree_is_exist(m, 2));
  mut_t(tree_is_exist(m, 3));

  mut_f(tree_is_exist(m, 4));
  mut_f(tree_is_exist(m, 5));
  mut_f(tree_is_exist(m, -1));
  mut_f(tree_is_exist(m, -2));

  tree_clear(m);

  mut_e(0, tree_len(m));
  mut_t(tree_is_empty(m));

  tree_cleanup(m);
}
