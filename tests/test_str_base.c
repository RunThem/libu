#include <u/u.h>

test() {
  u_str_t s = nullptr;

  /*  */
  s = u_str_t();

  mut_n(s);
  mut_n(s->ptr);
  mut_e(0, s->len);

  u_str_cleanup(s);
  mut_nn(s);

  s = u_str_t('h');
  mut_n(s);
  mut_n(s->ptr);
  mut_e(1, s->len);
  mut_t(0 == strcmp(s->ptr, "h"));

  /*  */
  s = u_str_t("hello");
  mut_n(s);
  mut_n(s->ptr);
  mut_e(5, s->len);
  mut_t(0 == strcmp(s->ptr, "hello"));

  s = u_str_t("l");
  u_str_cat(s, 'o');

  s = u_str_t("hello", 2);
  mut_e(2, s->len);
  mut_t(0 == strcmp(s->ptr, "he"));

  u_string_t s1 = u_str_t("hello");
}
