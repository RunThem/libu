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

#if 0
  auto f = ^(int a) {
    return a * 2;
  };

  auto l = lambda(int a) {
    return a + 3;
  };

  tb_trace_i("%d", f(32));

  auto fn = lambda(int* a, int* b) {
    return *a - *b;
  };

#  define labmbda_cmp(type) ^int(type * x, type * y)

  auto k = labmbda_cmp(int) {
    return *x - *y;
  };

  auto m = Block_copy(f);

  u_inf("blocks is %d", __has_extension(blocks));

#endif

#if 0
  for (int a = 0; true; ({ break; })) {
    tb_trace_i("hello");
  }

  u_dict_t(int, int) d = u_dict_new(d);

  u_dict_insert(d, 0, 0);

  auto ref = u_dict_at_ref(d, 0);

  auto mut = u_dict_at_mut(d, 0);

  tb_trace_i("%d", *mut);

  u_dict_each (d, it) {
    tb_trace_i("[%d] = %d", it.key, it.val);
  }

  tb_trace_i("len is %d", d->len);

  auto val = u_dict_remove(d, 0);

  auto a = NULL;

  auto result = __has_builtin(__builtin_types_compatible_p);

  tb_trace_i("types_compatible_p is %d", result);

  tb_exit();
#endif

  extern void libu_vec();

  libu_vec();

  return EXIT_SUCCESS;

end:
  return EXIT_FAILURE;
}

void libu_vec() {
  printf("[ivec] ================ Vec<T> ================\n");

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
  printf("[ivec]   new / cleanup         ok\n");

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
  printf("[ivec]   insert family         ok\n");

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
  printf("[ivec]   auto grow             ok\n");

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
  printf("[ivec]   at / at_ref / at_mut  ok\n");

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
  printf("[ivec]   remove family         ok\n");

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
  printf("[ivec]   resize                ok\n");

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
  printf("[ivec]   each family           ok\n");

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
  printf("[ivec]   reach family          ok\n");

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
  printf("[ivec]   find / rfind family   ok\n");

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
  printf("[ivec]   map_by                ok\n");

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
  printf("[ivec]   filter family         ok\n");

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
  printf("[ivec]   all_if / any_if       ok\n");

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
  printf("[ivec]   clear / cleanup proc  ok\n");

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
  printf("[ivec]   empty vec             ok\n");

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
  printf("[ivec]   stress                ok\n");

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
  printf("[ivec]   generic struct        ok\n");

  printf("[ivec] ============ all Vec tests passed ============\n");
}
