#include <Block.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <tbox/tbox.h>
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

/*
 * network
 *
 * tcp4://0.0.0.0:8080
 * udp4://0.0.0.0:8080
 * tcp6://[::1]:8080
 * udp6://[::1]:8080
 * tcpu:///mnt/tcp.sock
 * udpu:///mnt/tcp.sock
 * */

#define lambda(...) ^(__VA_ARGS__)

u_struct_def(ApiArgs) {
  int a;
  int b;
  int c;
};

/*
 * json = string : (null | true | false| string | number | array | object)
 **/

typedef struct json_t json_t, *json_ref_t;
struct json_t {
  u64_t type: 4;  /* {0: null, 1: true, 2: false, 3: string, 4: array, 5: object} */
  u64_t klen: 30; /* key len */

  /*
   * If type == 3, len is the string length.
   * If type == 4, len is the number of elements in the array.
   * If type == 5, len is the number of elements in the object.
   * */
  u64_t vlen: 30;

  char* key;

  union {
    char* s;
    f64_t n;
    u_vec_t(json_ref_t) a;
    u_vec_t(json_ref_t) o;
  };
};

#define json_n(_key)                                                                               \
  char auto json = new (json_ref_t, .type = 0, .key = _key, .klen = strlen(_key));                 \
  NULL_t json;                                                                                     \
  })

#define json_t(_key)                                                                               \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 1, .key = _key, .klen = strlen(_key));                    \
                                                                                                   \
    json;                                                                                          \
  })

#define json_f(_key)                                                                               \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 2, .key = _key, .klen = strlen(_key));                    \
                                                                                                   \
    json;                                                                                          \
  })

#define json_s(_key, _str, ...)                                                                    \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 3, .key = _key, .klen = strlen(_key), );                  \
                                                                                                   \
    json->s    = _str;                                                                             \
    json->vlen = strlen(_str);                                                                     \
                                                                                                   \
    json;                                                                                          \
  })

#define json_a(_key, ...)                                                                          \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 4, .key = _key, .klen = strlen(_key), );                  \
                                                                                                   \
    json->a    = u_vec_new(json_ref_t);                                                            \
    json->vlen = 0;                                                                                \
                                                                                                   \
    json;                                                                                          \
  })

#define json_o(_key, ...)                                                                          \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 5, .key = _key, .klen = strlen(_key), );                  \
                                                                                                   \
    json->o    = u_vec_new(json_ref_t);                                                            \
    json->vlen = 0;                                                                                \
                                                                                                   \
    json;                                                                                          \
  })

int main(int argc, const cstr_t argv[]) {
  tb_init(NULL, NULL);

  extern void libu_vec();

  libu_vec();

  extern void libu_tree();

  libu_tree();

  extern void libu_dict();

  libu_dict();

  return EXIT_SUCCESS;

end:
  return EXIT_FAILURE;
}

void libu_vec() {
  printf("================ Vec<T> ================\n");

  /* ---- 1. new / cleanup: 默认容量, 指定容量, ref 自引用 ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    assert(v);
    assert(v->len == 0);
    assert(v->cap == 16);       /* 默认 cap */
    assert(v->ref == (any_t)v); /* ref 自引用 */

    u_vec_t(int) w = u_vec_new(w, 8);
    assert(w);
    assert(w->len == 0);
    assert(w->cap == 8);

    u_vec_cleanup(w);
    assert(w == NULL); /* cleanup 后置 NULL */

    u_vec_cleanup(v);
    assert(v == NULL);
  }
  printf("  new / cleanup         ok\n");

  /* ---- 2. insert / insert_front / insert_back ---- */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert_back(v, 2);  /* [2]        */
    u_vec_insert_front(v, 0); /* [0, 2]     */
    u_vec_insert(v, 1, 1);    /* [0, 1, 2]  */
    u_vec_insert_back(v, 4);  /* [0, 1, 2, 4] */
    u_vec_insert(v, 3, 3);    /* [0, 1, 2, 3, 4] */

    assert(v->len == 5);
    assert(u_vec_at(v, 0) == 0);
    assert(u_vec_at(v, 1) == 1);
    assert(u_vec_at(v, 2) == 2);
    assert(u_vec_at(v, 3) == 3);
    assert(u_vec_at(v, 4) == 4);

    /* 空 vec 上 idx 0 / idx == len */
    u_vec_t(int) e = u_vec_new(e);
    u_vec_insert(e, 0, 7);
    assert(e->len == 1 && u_vec_at(e, 0) == 7);
    u_vec_insert(e, 1, 8);
    assert(e->len == 2 && u_vec_at(e, 1) == 8);

    u_vec_cleanup(e);
    u_vec_cleanup(v);
  }
  printf("  insert family         ok\n");

  /* ---- 3. 自动扩容: 默认 cap 16, 插入超过容量触发隐式扩容 ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    assert(v->cap == 16);

    for (int i = 0; i < 100; i++) {
      u_vec_insert_back(v, i);
    }

    assert(v->len == 100);
    assert(v->cap >= 100);
    for (int i = 0; i < 100; i++) {
      assert(u_vec_at(v, i) == i);
    }

    u_vec_cleanup(v);
  }
  printf("  auto grow             ok\n");

  /* ---- 4. at / at_ref / at_mut ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 2);
    u_vec_insert_back(v, 3);

    assert(u_vec_at(v, 1) == 2);

    /* at(idx, entry): 写入并返回写入值, 其余元素不变 */
    auto written = u_vec_at(v, 1, 20);
    assert(written == 20);
    assert(u_vec_at(v, 0) == 1);
    assert(u_vec_at(v, 1) == 20);
    assert(u_vec_at(v, 2) == 3);

    /* at_ref: 只读指针 */
    const int* r = u_vec_at_ref(v, 2);
    assert(r != NULL && *r == 3);

    /* at_mut: 可写指针 */
    int* m = u_vec_at_mut(v, 0);
    assert(m != NULL);
    *m = 10;
    assert(u_vec_at(v, 0) == 10);

    u_vec_cleanup(v);
  }
  printf("  at / at_ref / at_mut  ok\n");

  /* ---- 5. remove / remove_front / remove_back ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    for (int i = 0; i < 5; i++) {
      u_vec_insert_back(v, i); /* [0,1,2,3,4] */
    }

    u_vec_remove(v, 2); /* [0,1,3,4] */
    assert(v->len == 4);
    assert(u_vec_at(v, 0) == 0);
    assert(u_vec_at(v, 1) == 1);
    assert(u_vec_at(v, 2) == 3);
    assert(u_vec_at(v, 3) == 4);

    u_vec_remove_front(v); /* [1,3,4] */
    assert(v->len == 3);
    assert(u_vec_at(v, 0) == 1);

    u_vec_remove_back(v); /* [1,3] */
    assert(v->len == 2);
    assert(u_vec_at(v, 0) == 1);
    assert(u_vec_at(v, 1) == 3);

    u_vec_remove(v, 0);
    u_vec_remove(v, 0); /* [] */
    assert(v->len == 0);

    u_vec_cleanup(v);
  }
  printf("  remove family         ok\n");

  /* ---- 6. resize: 只扩容, 不改 len, 成功返回 true ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 2);
    u_vec_insert_back(v, 3);

    assert(u_vec_resize(v, 64) != 0);
    assert(v->cap == 64);
    assert(v->len == 3);
    assert(u_vec_at(v, 2) == 3); /* 元素保留 */

    assert(u_vec_resize(v, 128) != 0);
    assert(v->cap == 128);
    assert(v->len == 3);

    u_vec_cleanup(v);
  }
  printf("  resize                ok\n");

  /* ---- 7. each 家族(正向) ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 3);
    u_vec_insert_back(v, 5); /* [1,3,5] */

    int n   = 0;
    int sum = 0;
    u_vec_each (v, it) {
      n++;
      sum += it;
    }
    assert(n == 3 && sum == 9);

    n = 0;
    u_vec_each_if (v, it, it % 2 == 0) {
      n++;
    }
    assert(n == 0); /* 无偶数 */

    n = 0;
    u_vec_each_if (v, it, it > 1) {
      n++;
    }
    assert(n == 2);

    int idx = 0;
    u_vec_each_ref (v, ref) {
      assert(*ref == u_vec_at(v, idx));
      idx++;
    }
    assert(idx == 3);

    n = 0;
    u_vec_each_if_ref (v, ref, *ref > 1) {
      n++;
    }
    assert(n == 2);

    u_vec_each_mut (v, m) {
      *m *= 10; /* [10,30,50] */
    }
    assert(u_vec_at(v, 0) == 10);
    assert(u_vec_at(v, 1) == 30);
    assert(u_vec_at(v, 2) == 50);

    n = 0;
    u_vec_each_if_mut (v, m, *m > 20) {
      n++;
      *m = 0; /* [10,0,0] */
    }
    assert(n == 2);
    assert(u_vec_at(v, 0) == 10);
    assert(u_vec_at(v, 1) == 0);
    assert(u_vec_at(v, 2) == 0);

    u_vec_cleanup(v);
  }
  printf("  each family           ok\n");

  /* ---- 8. reach 家族(反向) ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 3);
    u_vec_insert_back(v, 5); /* [1,3,5] */

    const int expected[3] = {5, 3, 1};
    int idx               = 0;
    u_vec_reach (v, it) {
      assert(it == expected[idx]);
      idx++;
    }
    assert(idx == 3);

    int n = 0;
    u_vec_reach_if (v, it, it > 1) {
      n++;
    }
    assert(n == 2);

    idx = 0;
    u_vec_reach_ref (v, ref) {
      assert(*ref == expected[idx]);
      idx++;
    }
    assert(idx == 3);

    n = 0;
    u_vec_reach_if_ref (v, ref, *ref == 1) {
      n++;
    }
    assert(n == 1);

    u_vec_reach_mut (v, m) {
      *m += 1; /* [2,4,6] */
    }
    assert(u_vec_at(v, 0) == 2);
    assert(u_vec_at(v, 1) == 4);
    assert(u_vec_at(v, 2) == 6);

    u_vec_reach_if_mut (v, m, *m == 4) {
      *m = 40; /* [2,40,6] */
    }
    assert(u_vec_at(v, 0) == 2);
    assert(u_vec_at(v, 1) == 40);
    assert(u_vec_at(v, 2) == 6);

    u_vec_cleanup(v);
  }
  printf("  reach family          ok\n");

  /* ---- 9. find / rfind 家族 ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 5);
    u_vec_insert_back(v, 3);
    u_vec_insert_back(v, 5);
    u_vec_insert_back(v, 2); /* [1,5,3,5,2] */

    /* find_if: 返回第一个匹配值, 未命中返回零值 */
    assert(u_vec_find_if(v, it == 5) == 5);
    assert(u_vec_find_if(v, it == 2) == 2);
    assert(u_vec_find_if(v, it == 99) == 0);

    /* find_nif: 第一个使条件为假的值 */
    assert(u_vec_find_nif(v, it != 5) == 5);

    /* find_if_ref: const T*, 未命中 NULL */
    const int* fr = u_vec_find_if_ref(v, *it == 3);
    assert(fr != NULL && *fr == 3);
    assert(u_vec_find_if_ref(v, *it == 99) == NULL);

    /* find_nif_ref */
    const int* fnr = u_vec_find_nif_ref(v, *it != 3);
    assert(fnr != NULL && *fnr == 3);

    /* find_if_mut: T*, 修改命中元素 */
    int* fm = u_vec_find_if_mut(v, *it == 3);
    assert(fm != NULL && *fm == 3);
    *fm = 30; /* [1,5,30,5,2] */
    assert(u_vec_at(v, 2) == 30);

    /* find_nif_mut */
    int* fnm = u_vec_find_nif_mut(v, *it != 30);
    assert(fnm != NULL && *fnm == 30);

    /* rfind: 反向第一个匹配 */
    assert(u_vec_rfind_if(v, it == 5) == 5);
    assert(u_vec_rfind_nif(v, it != 5) == 5);

    const int* rr = u_vec_rfind_if_ref(v, *it == 5);
    assert(rr != NULL && *rr == 5);
    assert(rr == u_vec_at_ref(v, 3));

    const int* rnr = u_vec_rfind_nif_ref(v, *it != 5);
    assert(rnr != NULL && *rnr == 5);

    int* rm = u_vec_rfind_if_mut(v, *it == 5);
    assert(rm != NULL && rm == u_vec_at_mut(v, 3));
    *rm = 50; /* [1,5,30,50,2] */

    int* rnm = u_vec_rfind_nif_mut(v, *it != 30);
    assert(rnm != NULL && *rnm == 30);

    u_vec_cleanup(v);
  }
  printf("  find / rfind family   ok\n");

  /* ---- 10. map_by ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 2);
    u_vec_insert_back(v, 3);

    auto mv = u_vec_map_by(v, { it = it * 2; });
    assert(mv != NULL);
    assert(mv->len == 3);
    assert(u_vec_at(mv, 0) == 2);
    assert(u_vec_at(mv, 1) == 4);
    assert(u_vec_at(mv, 2) == 6);
    assert(u_vec_at(v, 0) == 1); /* 原 vec 不变 */
    assert(u_vec_at(v, 2) == 3);

    u_vec_cleanup(mv);
    u_vec_cleanup(v);
  }
  printf("  map_by                ok\n");

  /* ---- 11. filter_if / filter_if_ref ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    for (int i = 1; i <= 6; i++) {
      u_vec_insert_back(v, i); /* [1,2,3,4,5,6] */
    }

    auto ev = u_vec_filter_if(v, it % 2 == 0); /* [2,4,6] */
    assert(ev->len == 3);
    assert(u_vec_at(ev, 0) == 2);
    assert(u_vec_at(ev, 1) == 4);
    assert(u_vec_at(ev, 2) == 6);

    auto evr = u_vec_filter_if_ref(v, *it % 2 == 0); /* [2,4,6] */
    assert(evr->len == 3);
    assert(u_vec_at(evr, 0) == 2);
    assert(u_vec_at(evr, 2) == 6);

    auto none = u_vec_filter_if(v, it > 100); /* 空 */
    assert(none != NULL && none->len == 0);

    u_vec_cleanup(ev);
    u_vec_cleanup(evr);
    u_vec_cleanup(none);
    u_vec_cleanup(v);
  }
  printf("  filter family         ok\n");

  /* ---- 12. all_if / any_if(含 ref) ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 2);
    u_vec_insert_back(v, 3);

    assert(u_vec_all_if(v, it > 0));
    assert(!u_vec_all_if(v, it > 1));
    assert(u_vec_any_if(v, it == 2));
    assert(!u_vec_any_if(v, it == 99));

    assert(u_vec_all_if_ref(v, *it > 0));
    assert(!u_vec_all_if_ref(v, *it > 1));
    assert(u_vec_any_if_ref(v, *it == 2));
    assert(!u_vec_any_if_ref(v, *it == 99));

    u_vec_cleanup(v);
  }
  printf("  all_if / any_if       ok\n");

  /* ---- 13. clear: len 归零, cap 保留 ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    for (int i = 0; i < 5; i++) {
      u_vec_insert_back(v, i);
    }
    const int old_cap = v->cap;

    u_vec_clear(v);
    assert(v->len == 0);
    assert(v->cap == old_cap); /* 保留内存 */

    u_vec_insert_back(v, 7); /* 清空后复用 */
    assert(v->len == 1);
    assert(u_vec_at(v, 0) == 7);

    u_vec_cleanup(v);
  }

  /* ---- 14. clear / cleanup 带 proc ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 2);
    u_vec_insert_back(v, 3);

    int calls = 0;
    u_vec_clear(v, calls++);
    assert(calls == 3);
    assert(v->len == 0);

    u_vec_insert_back(v, 4);
    u_vec_insert_back(v, 5);
    calls = 0;
    u_vec_cleanup(v, calls++);
    assert(calls == 2);
    assert(v == NULL);
  }
  printf("  clear / cleanup proc  ok\n");

  /* ---- 15. 空 vec 行为 ---- */
  {
    u_vec_t(int) e = u_vec_new(e);
    assert(e->len == 0);

    u_vec_each (e, it) {
      assert(!"each must not iterate");
    }
    u_vec_each_ref (e, it) {
      assert(!"each_ref must not iterate");
    }
    u_vec_each_mut (e, it) {
      assert(!"each_mut must not iterate");
    }

    assert(u_vec_find_if(e, it == 0) == 0);
    assert(u_vec_find_if_ref(e, *it == 0) == NULL);
    assert(u_vec_find_if_mut(e, *it == 0) == NULL);
    assert(u_vec_rfind_if(e, it == 0) == 0);
    assert(u_vec_rfind_if_ref(e, *it == 0) == NULL);
    assert(u_vec_rfind_if_mut(e, *it == 0) == NULL);

    assert(u_vec_all_if(e, it != 0));  /* 空 → true */
    assert(!u_vec_any_if(e, it == 0)); /* 空 → false */
    assert(u_vec_all_if_ref(e, *it != 0));
    assert(!u_vec_any_if_ref(e, *it == 0));

    auto fe = u_vec_filter_if(e, it == 0);
    assert(fe != NULL && fe->len == 0);

    u_vec_cleanup(fe);
    u_vec_cleanup(e);
  }
  printf("  empty vec             ok\n");

  /* ---- 16. 压力: 批量插入(隐式扩容) / 头部删除 / 中间插入 ---- */
  {
    u_vec_t(int) v = u_vec_new(v);
    for (int i = 0; i < 1000; i++) {
      u_vec_insert_back(v, i);
    }
    assert(v->len == 1000);
    assert(v->cap >= 1000);
    for (int i = 0; i < 1000; i++) {
      assert(u_vec_at(v, i) == i);
    }

    for (int i = 0; i < 500; i++) {
      u_vec_remove(v, 0);
    }
    assert(v->len == 500);
    for (int i = 0; i < 500; i++) {
      assert(u_vec_at(v, i) == 500 + i);
    }

    u_vec_t(int) m = u_vec_new(m);
    u_vec_insert(m, 0, 0);
    u_vec_insert(m, 1, 2);
    u_vec_insert(m, 1, 1); /* [0,1,2] */
    assert(m->len == 3);
    assert(u_vec_at(m, 0) == 0);
    assert(u_vec_at(m, 1) == 1);
    assert(u_vec_at(m, 2) == 2);

    u_vec_cleanup(m);
    u_vec_cleanup(v);
  }
  printf("  stress                ok\n");

  /* ---- 17. 泛型: struct 元素 ---- */
  {
    typedef struct {
      int x;
      int y;
    } pt_t;

    u_vec_t(pt_t) pts = u_vec_new(pts);
    u_vec_insert_back(pts, ((pt_t){1, 2}));
    u_vec_insert_back(pts, ((pt_t){3, 4}));
    u_vec_insert_back(pts, ((pt_t){5, 6}));
    assert(pts->len == 3);

    assert(u_vec_at(pts, 0).x == 1);
    assert(u_vec_at(pts, 2).y == 6);

    pt_t* p = u_vec_at_mut(pts, 1);
    p->x    = 30; /* [{1,2},{30,4},{5,6}] */
    assert(u_vec_at(pts, 1).x == 30);

    int count = 0;
    int sum   = 0;
    u_vec_each (pts, it) {
      count++;
      sum += it.x;
    }
    assert(count == 3 && sum == 36);

    u_vec_each_mut (pts, it) {
      it->y *= 10; /* [{1,20},{30,40},{5,60}] */
    }
    assert(u_vec_at(pts, 0).y == 20);

    pt_t hit = u_vec_find_if(pts, it.x == 5);
    assert(hit.y == 60);

    const pt_t* pr = u_vec_find_if_ref(pts, it->x == 30);
    assert(pr != NULL && pr->y == 40);

    auto big = u_vec_filter_if(pts, it.x > 2); /* [{30,40},{5,60}] */
    assert(big->len == 2);
    assert(u_vec_at(big, 0).x == 30);
    assert(u_vec_at(big, 1).x == 5);

    u_vec_cleanup(big);
    u_vec_cleanup(pts);
  }
  printf("  generic struct        ok\n");

  printf("============ all Vec tests passed ============\n");

  printf("\n");
}

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

void libu_tree() {
  printf("================ Tree<K, V> ================\n");

  /* ---- 1. new / cleanup: len 为 0, ref 自引用, cleanup 置 NULL ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    assert(t);
    assert(t->len == 0);
    assert(t->ref == (any_t)t);

    u_tree_cleanup(t);
    assert(t == NULL);
  }
  printf("  new / cleanup         ok\n");

  /* ---- 2. insert / at 家族 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 5, 50);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 9, 90);
    u_tree_insert(t, 3, 30);
    u_tree_insert(t, 7, 70);
    assert(t->len == 5);

    assert(u_tree_at(t, 1) == 10);
    assert(u_tree_at(t, 3) == 30);
    assert(u_tree_at(t, 5) == 50);
    assert(u_tree_at(t, 7) == 70);
    assert(u_tree_at(t, 9) == 90);

    /* at(key, val): 写入并返回写入值, 其余不变 */
    auto w = u_tree_at(t, 3, 33);
    assert(w == 33);
    assert(u_tree_at(t, 3) == 33);
    assert(u_tree_at(t, 1) == 10);

    /* at_ref: 只读指针 */
    assert(*u_tree_at_ref(t, 7) == 70);

    /* at_mut: 可写指针 */
    int* m = u_tree_at_mut(t, 7);
    assert(m != NULL);
    *m = 77;
    assert(u_tree_at(t, 7) == 77);
    assert(u_tree_at(t, 9) == 90); /* 其余不变 */

    u_tree_cleanup(t);
  }
  printf("  insert / at family    ok\n");

  /* ---- 3. try_at 家族: 命中执行, 未命中跳过 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 2, 20);
    u_tree_insert(t, 4, 40);

    int found = 0;
    u_tree_try_at (t, 2, v) {
      found = 1;
      assert(v == 20);
    }
    assert(found == 1);

    found = 0;
    u_tree_try_at (t, 99, v) {
      found = 1;
      (void)v;
    }
    assert(found == 0);

    found = 0;
    u_tree_try_at_ref (t, 4, r) {
      found = 1;
      assert(*r == 40);
    }
    assert(found == 1);

    found = 0;
    u_tree_try_at_ref (t, 99, r) {
      found = 1;
      (void)r;
    }
    assert(found == 0);

    found = 0;
    u_tree_try_at_mut (t, 2, m) {
      found = 1;
      *m    = 22;
    }
    assert(found == 1);
    assert(u_tree_at(t, 2) == 22);

    found = 0;
    u_tree_try_at_mut (t, 99, m) {
      found = 1;
      (void)m;
    }
    assert(found == 0);

    u_tree_cleanup(t);
  }
  printf("  try_at family         ok\n");

  /* ---- contains / min / max ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 5, 50);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 9, 90);
    u_tree_insert(t, 3, 30);
    u_tree_insert(t, 7, 70);

    assert(u_tree_contains(t, 1));
    assert(u_tree_contains(t, 5));
    assert(u_tree_contains(t, 9));
    assert(!u_tree_contains(t, 0));
    assert(!u_tree_contains(t, 42));

    auto mn = u_tree_min(t);
    assert(mn != NULL && mn->key == 1 && mn->val == 10);

    auto mx = u_tree_max(t);
    assert(mx != NULL && mx->key == 9 && mx->val == 90);

    /* 单节点树 min == max */
    u_tree_t(int, int) one = u_tree_new(one, cmp_int);
    u_tree_insert(one, 7, 70);
    assert(u_tree_min(one)->key == 7 && u_tree_max(one)->key == 7);
    u_tree_cleanup(one);

    u_tree_cleanup(t);
  }
  printf("  contains / min / max  ok\n");

  /* ---- 4. each 家族: 中序遍历即有序 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 5, 50);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 9, 90);
    u_tree_insert(t, 3, 30);
    u_tree_insert(t, 7, 70);

    const int expect[5] = {1, 3, 5, 7, 9};
    int n               = 0;
    u_tree_each (t, it) {
      assert(it.key == expect[n]);
      assert(it.val == it.key * 10);
      n++;
    }
    assert(n == 5);

    n = 0;
    u_tree_each_if (t, it, it.key > 3) {
      n++;
    }
    assert(n == 3);

    n = 0;
    u_tree_each_ref (t, ref) {
      assert(ref->key == expect[n]);
      assert(ref->val == expect[n] * 10);
      n++;
    }
    assert(n == 5);

    n = 0;
    u_tree_each_if_ref (t, ref, ref->val > 50) {
      n++;
    }
    assert(n == 2);

    /* each_mut: 通过指针修改 val */
    u_tree_each_mut (t, mut) {
      mut->val += 1;
    }
    assert(u_tree_at(t, 5) == 51);
    assert(u_tree_at(t, 1) == 11);

    n = 0;
    u_tree_each_if_mut (t, mut, mut->key == 1) {
      n++;
      mut->val = 0;
    }
    assert(n == 1);
    assert(u_tree_at(t, 1) == 0);

    u_tree_cleanup(t);
  }
  printf("  each family           ok\n");

  /* ---- reach 家族: 反向中序即降序 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 5, 50);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 9, 90);
    u_tree_insert(t, 3, 30);
    u_tree_insert(t, 7, 70);

    const int desc[5] = {9, 7, 5, 3, 1};
    int n = 0;
    u_tree_reach (t, it) {
      assert(it.key == desc[n]);
      assert(it.val == it.key * 10);
      n++;
    }
    assert(n == 5);

    n = 0;
    u_tree_reach_if (t, it, it.key < 7) { n++; }
    assert(n == 3);

    n = 0;
    u_tree_reach_ref (t, ref) {
      assert(ref->key == desc[n]);
      assert(ref->val == desc[n] * 10);
      n++;
    }
    assert(n == 5);

    n = 0;
    u_tree_reach_if_ref (t, ref, ref->val < 50) { n++; }
    assert(n == 2);

    /* reach_mut: 反向修改 val */
    u_tree_reach_mut (t, mut) { mut->val += 1; }
    assert(u_tree_at(t, 9) == 91);
    assert(u_tree_at(t, 1) == 11);

    n = 0;
    u_tree_reach_if_mut (t, mut, mut->key == 5) { n++; mut->val = 0; }
    assert(n == 1);
    assert(u_tree_at(t, 5) == 0);

    u_tree_cleanup(t);
  }
  printf("  reach family          ok\n");

  /* ---- map_by / filter_if / filter_if_ref ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 2, 20);
    u_tree_insert(t, 3, 30);

    /* map_by: proc 修改 val(不碰 key, 保持有序), 原树不变 */
    auto mt = u_tree_map_by(t, { it.val *= 2; });
    assert(mt != NULL);
    assert(mt->len == 3);
    assert(u_tree_at(mt, 1) == 20);
    assert(u_tree_at(mt, 2) == 40);
    assert(u_tree_at(mt, 3) == 60);
    assert(u_tree_at(t, 1) == 10); /* 原树不变 */
    u_tree_cleanup(mt);

    /* filter_if / filter_if_ref */
    auto ft = u_tree_filter_if(t, it.val >= 20);       /* [2,3] */
    assert(ft->len == 2);
    assert(u_tree_at(ft, 2) == 20);
    assert(u_tree_at(ft, 3) == 30);
    u_tree_cleanup(ft);

    auto ftr = u_tree_filter_if_ref(t, it->val >= 30); /* [3] */
    assert(ftr->len == 1);
    assert(u_tree_at(ftr, 3) == 30);
    u_tree_cleanup(ftr);

    /* 空树 map/filter 得空树 */
    u_tree_t(int, int) e = u_tree_new(e, cmp_int);
    auto em = u_tree_map_by(e, { it.val = 0; });
    auto ef = u_tree_filter_if(e, it.val > 0);
    assert(em->len == 0 && ef->len == 0);
    u_tree_cleanup(em);
    u_tree_cleanup(ef);
    u_tree_cleanup(e);

    u_tree_cleanup(t);
  }
  printf("  map_by / filter       ok\n");

  /* ---- all_if / any_if(含 ref) ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 2, 20);
    u_tree_insert(t, 3, 30);

    assert(u_tree_all_if(t, it.val > 0));
    assert(!u_tree_all_if(t, it.val > 10));
    assert(u_tree_any_if(t, it.val == 30));
    assert(!u_tree_any_if(t, it.val == 99));

    assert(u_tree_all_if_ref(t, it->val > 0));
    assert(!u_tree_all_if_ref(t, it->val > 10));
    assert(u_tree_any_if_ref(t, it->val == 30));
    assert(!u_tree_any_if_ref(t, it->val == 99));

    u_tree_cleanup(t);
  }
  printf("  all_if / any_if       ok\n");

  /* ---- 5. remove: 叶子/内部/根交错删除, 保持有序 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    for (int i = 0; i < 10; i++) {
      u_tree_insert(t, i, i * 10);
    }
    assert(t->len == 10);

    const int order[10] = {0, 9, 5, 2, 7, 1, 8, 3, 6, 4};
    int removed[10]     = {0};

    for (int i = 0; i < 10; i++) {
      removed[order[i]] = 1;
      u_tree_remove(t, order[i]);
      assert(t->len == 9 - i);

      int prev = -1;
      int n    = 0;
      u_tree_each (t, it) {
        assert(it.key > prev);        /* 中序严格递增 */
        assert(removed[it.key] == 0); /* 已删的键不在 */
        prev = it.key;
        n++;
      }
      assert(n == 9 - i);
    }
    assert(t->len == 0);

    u_tree_cleanup(t);
  }
  printf("  remove                ok\n");

  /* ---- 6. clear: len 归零, 可复用 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    for (int i = 0; i < 100; i++) {
      u_tree_insert(t, i, i);
    }
    assert(t->len == 100);

    u_tree_clear(t);
    assert(t->len == 0);

    u_tree_insert(t, 42, 4242);
    assert(t->len == 1);
    assert(u_tree_at(t, 42) == 4242);

    u_tree_cleanup(t);
  }
  printf("  clear                 ok\n");

  /* ---- 7. 压力: 1000 键乱序插入 + 全查 + 删 500 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);

    int keys[1000];
    for (int i = 0; i < 1000; i++) {
      keys[i] = i;
    }

    /* 确定性伪随机洗牌(LCG, 不依赖 libc rand) */
    unsigned s = 12345u;
    for (int i = 0; i < 500; i++) {
      s       = s * 1664525u + 1013904223u;
      int j   = (int)(s % 1000u);
      int tmp = keys[i];
      keys[i] = keys[j];
      keys[j] = tmp;
    }

    for (int i = 0; i < 1000; i++) {
      u_tree_insert(t, keys[i], keys[i] * 2);
    }
    assert(t->len == 1000);

    for (int i = 0; i < 1000; i++) {
      assert(u_tree_at(t, i) == i * 2);
    }

    int prev = -1;
    int n    = 0;
    u_tree_each (t, it) {
      assert(it.key > prev);
      assert(it.val == it.key * 2);
      prev = it.key;
      n++;
    }
    assert(n == 1000);

    /* 删除前 500 个(按洗牌后顺序) */
    for (int i = 0; i < 500; i++) {
      u_tree_remove(t, keys[i]);
    }
    assert(t->len == 500);

    for (int i = 500; i < 1000; i++) {
      assert(u_tree_at(t, keys[i]) == keys[i] * 2);
    }
    n = 0;
    u_tree_each (t, it) {
      n++;
    }
    assert(n == 500);

    u_tree_cleanup(t);
  }
  printf("  stress                ok\n");

  /* ---- 8. 泛型: struct 键 ---- */
  {
    u_tree_t(rec_t, int) t = u_tree_new(t, cmp_rec);
    u_tree_insert(t, ((rec_t){1, 100}), 111);
    u_tree_insert(t, ((rec_t){3, 300}), 333);
    u_tree_insert(t, ((rec_t){2, 200}), 222);
    assert(t->len == 3);

    assert(u_tree_at(t, ((rec_t){2, 200})) == 222);
    assert(u_tree_at(t, ((rec_t){1, 100})) == 111);

    *u_tree_at_mut(t, ((rec_t){3, 300})) = 999;
    assert(u_tree_at(t, ((rec_t){3, 300})) == 999);

    int n = 0;
    u_tree_each (t, it) {
      assert(it.key.id == n + 1);
      n++;
    }
    assert(n == 3);

    u_tree_cleanup(t);
  }
  printf("  generic struct        ok\n");

  /* ---- 9. 空树行为 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);

    u_tree_each (t, it) {
      assert(!"each must not iterate");
    }
    u_tree_each_ref (t, it) {
      assert(!"each_ref must not iterate");
    }
    u_tree_each_mut (t, it) {
      assert(!"each_mut must not iterate");
    }
    u_tree_reach (t, it) {
      assert(!"reach must not iterate");
    }
    u_tree_reach_ref (t, it) {
      assert(!"reach_ref must not iterate");
    }
    u_tree_reach_mut (t, it) {
      assert(!"reach_mut must not iterate");
    }

    int found = 0;
    u_tree_try_at (t, 1, v) {
      found = 1;
    }
    u_tree_try_at_ref (t, 1, v) {
      found = 1;
    }
    u_tree_try_at_mut (t, 1, v) {
      found = 1;
    }
    assert(found == 0);

    u_tree_remove(t, 1);
    u_tree_clear(t);

    u_tree_cleanup(t);
    assert(t == NULL);
  }
  printf("  empty tree            ok\n");

  /* ---- remove 缺失键: 静默 no-op, len 不变 ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 2, 20);
    u_tree_insert(t, 3, 30);

    u_tree_remove(t, 99); /* 不存在 */
    u_tree_remove(t, -1); /* 不存在(负键合法) */
    assert(t->len == 3);
    assert(u_tree_contains(t, 1) && u_tree_contains(t, 2) && u_tree_contains(t, 3));
    assert(!u_tree_contains(t, 99));

    u_tree_remove(t, 2);
    assert(t->len == 2);
    u_tree_remove(t, 2); /* 已删, no-op */
    assert(t->len == 2);
    assert(!u_tree_contains(t, 2));

    int prev = -1;
    int n    = 0;
    u_tree_each (t, it) {
      assert(it.key > prev);
      prev = it.key;
      n++;
    }
    assert(n == 2);

    u_tree_cleanup(t);
  }
  printf("  remove missing key     ok\n");

  /* ---- clear / cleanup 带 proc (proc 按中序执行) ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    u_tree_insert(t, 5, 50);
    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 9, 90);
    u_tree_insert(t, 3, 30);
    u_tree_insert(t, 7, 70);

    int keys[5] = {0};
    int n       = 0;
    u_tree_clear(t, keys[n++] = it.key); /* proc: 按中序收集 key */
    assert(n == 5);
    for (int i = 0; i < 5; i++) {
      assert(keys[i] == i * 2 + 1); /* 1,3,5,7,9 */
    }
    assert(t->len == 0);

    u_tree_insert(t, 1, 10);
    u_tree_insert(t, 2, 20);
    n = 0;
    u_tree_cleanup(t, n++); /* proc 计数 */
    assert(n == 2);
    assert(t == NULL);
  }
  printf("  clear / cleanup proc  ok\n");

  /* ---- remove + reinsert (free-list 复用) ---- */
  {
    u_tree_t(int, int) t = u_tree_new(t, cmp_int);
    for (int i = 0; i < 100; i++) {
      u_tree_insert(t, i, i);
    }
    assert(t->len == 100);

    for (int i = 0; i < 50; i++) {
      u_tree_remove(t, i); /* 积累 free list */
    }
    assert(t->len == 50);
    for (int i = 0; i < 50; i++) {
      u_tree_insert(t, i, i); /* 消费 free list */
    }
    assert(t->len == 100);

    for (int r = 0; r < 5; r++) { /* 多轮单删单插, 反复复用 free list */
      for (int i = 0; i < 100; i++) {
        u_tree_remove(t, i);
        u_tree_insert(t, i, i);
      }
    }
    assert(t->len == 100);

    for (int i = 0; i < 100; i++) {
      assert(u_tree_at(t, i) == i);
    }

    int prev = -1;
    int n    = 0;
    u_tree_each (t, it) {
      assert(it.key > prev);
      assert(it.val == it.key);
      prev = it.key;
      n++;
    }
    assert(n == 100);

    u_tree_cleanup(t);
  }
  printf("  remove + reinsert     ok\n");

  printf("============ all Tree tests passed ============\n");

  printf("\n");
}

void libu_dict() {
  printf("================ Dict<K, V> ================\n");

  /* ---- 1. new / cleanup: len 为 0, ref 自引用, cleanup 置 NULL ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    assert(d);
    assert(d->len == 0);
    assert(d->ref == (any_t)d);

    u_dict_cleanup(d);
    assert(d == NULL);
  }
  printf("  new / cleanup         ok\n");

  /* ---- 2. insert / at 家族 ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    u_dict_insert(d, 5, 50);
    u_dict_insert(d, 1, 10);
    u_dict_insert(d, 9, 90);
    u_dict_insert(d, 3, 30);
    u_dict_insert(d, 7, 70);
    assert(d->len == 5);

    assert(u_dict_at(d, 1) == 10);
    assert(u_dict_at(d, 3) == 30);
    assert(u_dict_at(d, 5) == 50);
    assert(u_dict_at(d, 7) == 70);
    assert(u_dict_at(d, 9) == 90);

    /* 重复 key: 静默忽略, 原值保留, len 不变 */
    u_dict_insert(d, 3, 999);
    assert(d->len == 5);
    assert(u_dict_at(d, 3) == 30);

    /* at(key, val): 写入并返回写入值, 其余不变 */
    auto w = u_dict_at(d, 3, 33);
    assert(w == 33);
    assert(u_dict_at(d, 3) == 33);
    assert(u_dict_at(d, 1) == 10);

    /* at_ref: 只读指针, 未命中返回 NULL */
    assert(*u_dict_at_ref(d, 7) == 70);
    assert(u_dict_at_ref(d, 99) == NULL);

    /* at_mut: 可写指针, 未命中返回 NULL */
    int* m = u_dict_at_mut(d, 7);
    assert(m != NULL);
    *m = 77;
    assert(u_dict_at(d, 7) == 77);
    assert(u_dict_at(d, 9) == 90); /* 其余不变 */
    assert(u_dict_at_mut(d, 99) == NULL);

    u_dict_cleanup(d);
  }
  printf("  insert / at family    ok\n");

  /* ---- 3. try_at 家族: 命中执行, 未命中跳过 ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    u_dict_insert(d, 2, 20);
    u_dict_insert(d, 4, 40);

    int found = 0;
    u_dict_try_at (d, 2, v) {
      found = 1;
      assert(v == 20);
    }
    assert(found == 1);

    found = 0;
    u_dict_try_at (d, 99, v) {
      found = 1;
      (void)v;
    }
    assert(found == 0);

    found = 0;
    u_dict_try_at_ref (d, 4, r) {
      found = 1;
      assert(*r == 40);
    }
    assert(found == 1);

    found = 0;
    u_dict_try_at_ref (d, 99, r) {
      found = 1;
      (void)r;
    }
    assert(found == 0);

    found = 0;
    u_dict_try_at_mut (d, 2, m) {
      found = 1;
      *m    = 22;
    }
    assert(found == 1);
    assert(u_dict_at(d, 2) == 22);

    found = 0;
    u_dict_try_at_mut (d, 99, m) {
      found = 1;
      (void)m;
    }
    assert(found == 0);

    u_dict_cleanup(d);
  }
  printf("  try_at family         ok\n");

  /* ---- 4. each 家族: 桶序(无序), 集合成员一致 ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    for (int i = 0; i < 100; i++) {
      u_dict_insert(d, i, i * 10);
    }
    assert(d->len == 100);

    int sum_key = 0;
    int sum_val = 0;
    int n       = 0;
    u_dict_each (d, it) {
      assert(it.val == it.key * 10);
      sum_key += it.key;
      sum_val += it.val;
      n++;
    }
    assert(n == 100);
    assert(sum_key == 100 * 99 / 2);
    assert(sum_val == 10 * sum_key);

    n = 0;
    u_dict_each_if (d, it, it.key % 2 == 0) {
      n++;
    }
    assert(n == 50);

    n = 0;
    u_dict_each_ref (d, ref) {
      assert(ref->val == ref->key * 10);
      n++;
    }
    assert(n == 100);

    n = 0;
    u_dict_each_if_ref (d, ref, ref->key % 2 == 1) {
      n++;
    }
    assert(n == 50);

    /* each_mut: 通过指针修改 val */
    u_dict_each_mut (d, mut) {
      mut->val += 1;
    }
    assert(*u_dict_at_ref(d, 5) == 51);
    assert(*u_dict_at_ref(d, 1) == 11);

    n = 0;
    u_dict_each_if_mut (d, mut, mut->key == 5) {
      n++;
      mut->val = 0;
    }
    assert(n == 1);
    assert(u_dict_at(d, 5) == 0);

    u_dict_cleanup(d);
  }
  printf("  each family           ok\n");

  /* ---- 5. remove: 命中返回被删值, 未命中返回零值 ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    for (int i = 0; i < 10; i++) {
      u_dict_insert(d, i, i * 10);
    }
    assert(d->len == 10);

    assert(u_dict_remove(d, 3) == 30);
    assert(d->len == 9);
    assert(u_dict_remove(d, 3) == 0);  /* 已删 */
    assert(u_dict_remove(d, 99) == 0); /* 从未存在 */
    assert(d->len == 9);

    /* 交错删除 */
    for (int i = 0; i < 10; i++) {
      if (i % 2 == 0) {
        u_dict_remove(d, i);
      }
    }
    assert(d->len == 4);

    int n = 0;
    u_dict_each (d, it) {
      assert(it.key % 2 == 1);
      n++;
    }
    assert(n == 4);

    u_dict_cleanup(d);
  }
  printf("  remove                ok\n");

  /* ---- 6. clear / cleanup 带 proc ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    u_dict_insert(d, 5, 50);
    u_dict_insert(d, 1, 10);
    u_dict_insert(d, 9, 90);

    int sum = 0;
    int n   = 0;
    u_dict_clear(d, sum += it.val, n++);
    assert(n == 3);
    assert(sum == 150);
    assert(d->len == 0);

    u_dict_insert(d, 1, 10);
    u_dict_insert(d, 2, 20);
    n = 0;
    u_dict_cleanup(d, n++); /* proc 计数 */
    assert(n == 2);
    assert(d == NULL);
  }
  printf("  clear / cleanup proc  ok\n");

  /* ---- 7. 压力: 1000 键乱序插入(含 rehash) + 全查 + 删 500 ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);

    int keys[1000];
    for (int i = 0; i < 1000; i++) {
      keys[i] = i;
    }

    /* 确定性伪随机洗牌(LCG, 不依赖 libc rand) */
    unsigned s = 12345u;
    for (int i = 0; i < 500; i++) {
      s       = s * 1664525u + 1013904223u;
      int j   = (int)(s % 1000u);
      int tmp = keys[i];
      keys[i] = keys[j];
      keys[j] = tmp;
    }

    for (int i = 0; i < 1000; i++) {
      u_dict_insert(d, keys[i], keys[i] * 2);
    }
    assert(d->len == 1000);

    for (int i = 0; i < 1000; i++) {
      assert(u_dict_at(d, i) == i * 2);
    }

    int n = 0;
    u_dict_each (d, it) {
      assert(it.val == it.key * 2);
      n++;
    }
    assert(n == 1000);

    /* 删除前 500 个(按洗牌后顺序) */
    for (int i = 0; i < 500; i++) {
      u_dict_remove(d, keys[i]);
    }
    assert(d->len == 500);

    for (int i = 500; i < 1000; i++) {
      assert(u_dict_at(d, keys[i]) == keys[i] * 2);
    }
    n = 0;
    u_dict_each (d, it) {
      n++;
    }
    assert(n == 500);

    u_dict_cleanup(d);
  }
  printf("  stress                ok\n");

  /* ---- 8. 空 dict 行为 ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);

    u_dict_each (d, it) {
      assert(!"each must not iterate");
    }
    u_dict_each_ref (d, it) {
      assert(!"each_ref must not iterate");
    }
    u_dict_each_mut (d, it) {
      assert(!"each_mut must not iterate");
    }

    assert(u_dict_at_ref(d, 1) == NULL);
    assert(u_dict_at_mut(d, 1) == NULL);

    int found = 0;
    u_dict_try_at (d, 1, v) {
      found = 1;
    }
    u_dict_try_at_ref (d, 1, v) {
      found = 1;
    }
    u_dict_try_at_mut (d, 1, v) {
      found = 1;
    }
    assert(found == 0);

    assert(u_dict_remove(d, 1) == 0);
    u_dict_clear(d);

    u_dict_cleanup(d);
    assert(d == NULL);
  }
  printf("  empty dict            ok\n");

  /* ---- 9. 自定义 hash_fn ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d, hash_int);
    u_dict_insert(d, 1, 11);
    u_dict_insert(d, 2, 22);
    u_dict_insert(d, 3, 33);
    assert(d->len == 3);

    assert(u_dict_at(d, 1) == 11);
    assert(u_dict_at(d, 2) == 22);
    assert(u_dict_at(d, 3) == 33);

    u_dict_cleanup(d);
  }
  printf("  custom hash fn        ok\n");

  /* ---- 10. rehash: 跨扩容阈值后键仍可查, 桶迁移无丢失/重复 ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    /* 初始桶数 8, 阈值 len*1.5 > bucket_size 时翻倍; 插 10 个必触发 rehash */
    for (int i = 0; i < 10; i++) {
      u_dict_insert(d, i, i * 10);
    }
    assert(d->len == 10);

    /* rehash 后所有键的索引(hash & mask_size)仍正确 */
    for (int i = 0; i < 10; i++) {
      assert(u_dict_at(d, i) == i * 10);
    }

    /* 扩容后桶上再删/插, 校验 rehash 后桶的增删路径 */
    for (int i = 0; i < 5; i++) {
      u_dict_remove(d, i);
    }
    assert(d->len == 5);
    for (int i = 0; i < 5; i++) {
      u_dict_insert(d, 100 + i, 100 + i);
    }
    assert(d->len == 10);
    for (int i = 0; i < 5; i++) {
      assert(u_dict_at(d, 100 + i) == 100 + i);
      assert(u_dict_at_ref(d, i) == NULL); /* 已删 */
    }
    for (int i = 5; i < 10; i++) {
      assert(u_dict_at(d, i) == i * 10); /* 旧键保留 */
    }

    u_dict_cleanup(d);
  }
  printf("  rehash                ok\n");

  /* ---- 11. 删后重插同键 (节点回收路径) ---- */
  {
    u_dict_t(int, int) d = u_dict_new(d);
    for (int i = 0; i < 50; i++) {
      u_dict_insert(d, i, i);
    }
    assert(d->len == 50);

    for (int i = 0; i < 50; i++) {
      u_dict_remove(d, i); /* 全删 */
    }
    assert(d->len == 0);
    for (int i = 0; i < 50; i++) {
      assert(u_dict_at_ref(d, i) == NULL);
    }

    /* 重插同键, 不同 val, 校验节点回收后无残留 */
    for (int i = 0; i < 50; i++) {
      u_dict_insert(d, i, i * 2);
    }
    assert(d->len == 50);
    for (int i = 0; i < 50; i++) {
      assert(u_dict_at(d, i) == i * 2);
    }

    /* 多轮单删单插, 反复触发节点分配/回收 */
    for (int r = 0; r < 5; r++) {
      for (int i = 0; i < 50; i++) {
        u_dict_remove(d, i);
        u_dict_insert(d, i, i + r);
      }
    }
    assert(d->len == 50);
    for (int i = 0; i < 50; i++) {
      assert(u_dict_at(d, i) == i + 4); /* 最后一轮 r=4 */
    }

    u_dict_cleanup(d);
  }
  printf("  remove + reinsert     ok\n");

  printf("============ all Dict tests passed ============\n");

  printf("\n");
}
