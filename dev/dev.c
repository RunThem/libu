#include <u/u.h>

/*
 * #[[flag]]
 *
 * */

/*
 * namespace
 *
 * ua: arr
 * uv: vec
 * um: map
 * ut: avl
 * ul: lst
 * us: str
 * ub: buf
 * uf: file
 * ug: log
 * un: net
 * */

/* 全新版本的字符串原始实现 */
// typedef char* u_string_t[2]; /* {raw string pointer, string data pointer} */

int main(int argc, const u_cstr_t argv[]) {

  u_str_t s = u_str_t("hello");
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_cat(s, ',');
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_cat(s, "%s!", "world");
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_ins(s, 6, " ");
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_dbg("s == %d", u_str_cmp(s, "hello, world!"));
  u_dbg("s == %d", u_str_cmp(s, "hello, world! "));
  u_dbg("s == %d", u_str_cmp(s, "hello, world"));

  u_str_t s1 = u_str_t("%s", s->ptr);
  u_dbg("s == %d", u_str_cmp(s, s1));

  u_str_clear(s);
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_cleanup(s);
  u_dbg("%p", s);

  s = s1;
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_slen(s, 5);
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_2upper(s);
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_2lower(s);
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_cat(s, " ");
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_rtrim(s);
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_ins(s, 0, "  ");
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_str_ltrim(s);
  u_dbg("%p, '%s', %d", s, s->ptr, s->len);

  u_dbg("prefix = %d", u_str_prefix(s, 'h'));
  u_dbg("prefix = %d", u_str_prefix(s, "h"));
  u_dbg("prefix = %d", u_str_prefix(s, "he"));
  u_dbg("prefix = %d", u_str_prefix(s, "hel"));
  u_dbg("prefix = %d", u_str_prefix(s, "hell"));
  u_dbg("prefix = %d", u_str_prefix(s, "hello"));
  u_dbg("prefix = %d", u_str_prefix(s, "hello,"));
  u_dbg("prefix = %d", u_str_prefix(s, "hell,"));

  u_dbg("suffix = %d", u_str_suffix(s, 'o'));
  u_dbg("suffix = %d", u_str_suffix(s, "o"));
  u_dbg("suffix = %d", u_str_suffix(s, "lo"));
  u_dbg("suffix = %d", u_str_suffix(s, "llo"));
  u_dbg("suffix = %d", u_str_suffix(s, "ello"));
  u_dbg("suffix = %d", u_str_suffix(s, "hello"));
  u_dbg("suffix = %d", u_str_suffix(s, ",hello"));
  u_dbg("suffix = %d", u_str_suffix(s, ",ello"));

  return EXIT_SUCCESS;
}
