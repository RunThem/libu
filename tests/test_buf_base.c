#include <u/u.h>

test() {
  u_buf_ref_t v = nullptr;

  /* alloc buffer */
  v = u_buf_new(nullptr, 32);
  mut_n(v);

  mut_e(0, u_buf_len(v));
  mut_t(u_buf_is_empty(v));

  u_buf_put(v, me(int, 1));
  u_buf_put(v, me(int, 2));
  u_buf_put(v, me(int, 3));
  u_buf_put(v, me(int, 4));
  u_buf_put(v, me(int, 5));
  u_buf_put(v, me(int, 6));

  mut_e(6 * sizeof(int), u_buf_len(v));
  mut_f(u_buf_is_empty(v));

  int num = 0;

  mut_e(1, u_buf_pop(v, i32_t));
  mut_e(2, u_buf_pop(v, i32_t));
  mut_e(3, u_buf_pop(v, i32_t));
  mut_e(4, u_buf_pop(v, i32_t));
  mut_e(5, u_buf_pop(v, i32_t));
  mut_e(6, u_buf_pop(v, i32_t));

  mut_e(0, u_buf_len(v));
  mut_t(u_buf_is_empty(v));

  u_buf_put(v, me(int, 0xa3'12'86'53));
  mut_e(1 * sizeof(int), u_buf_len(v));

  // u_dbg("0x%x", u_buf_pop(v, u8_t));
  // u_dbg("0x%x", u_buf_pop(v, u8_t));
  // u_dbg("0x%x", u_buf_pop(v, u8_t));
  // u_dbg("0x%x", u_buf_pop(v, u8_t));
  mut_e(0x53, u_buf_pop(v, u8_t));
  mut_e(0x86, u_buf_pop(v, u8_t));
  mut_e(0x12, u_buf_pop(v, u8_t));
  mut_e(0xa3, u_buf_pop(v, u8_t));

  mut_e(0, u_buf_len(v));
  mut_t(u_buf_is_empty(v));

  u_buf_put(v, me(int, 0xa3'12'86'53));
  mut_e(1 * sizeof(int), u_buf_len(v));

  u_buf_clear(v);
  mut_e(0, u_buf_len(v));
  mut_t(u_buf_is_empty(v));

  u_buf_cleanup(v);
}
