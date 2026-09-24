/* clang-format off */
#ifndef U_TEST_PUB_H__
#  define U_TEST_PUB_H__

/* 用例一律用 assert 判失败: 即使 -DNDEBUG (release) 也要保留, 否则单元测试会被静默关掉 */
#  ifdef NDEBUG
#    undef NDEBUG
#  endif

#  include <assert.h>
#  include <stdio.h>

#  include <tbox/tbox.h>
#  include <u/u.h>

/***************************************************************************************************
 * 用例公用的比较 / 哈希函数 (tree, dict 用例需要)
 **************************************************************************************************/
typedef struct {
  int id;
  int score;
} rec_t;
static int cmp_int(cany_t a, cany_t b) {
  int x = *(const int*)a;
  int y = *(const int*)b;
  return (x > y) - (x < y);
}
static int cmp_rec(cany_t a, cany_t b) {
  const rec_t* x = (const rec_t*)a;
  const rec_t* y = (const rec_t*)b;
  return (x->id > y->id) - (x->id < y->id);
}
/* djb2 风格哈希, 用于自定义 hash_fn 路径 */
static u_hash_t hash_int(const u8_t* a, size_t len) {
  u_hash_t hash = 5381;
  for (size_t i = 0; i < len; i++) {
    hash = hash * 33 + a[i];
  }
  return hash;
}

#endif /* !U_TEST_PUB_H__ */
