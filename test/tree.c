/***************************************************************************************************
 * Test: Tree<K, V>
 *
 * 覆盖: new / cleanup / insert / at family / try_at family / contains / min / max / each family /
 * reach family / map_by / filter / all_if / any_if / remove / clear / stress / generic struct /
 * empty tree / remove missing key / clear / cleanup proc / remove + reinsert
 **************************************************************************************************/

#include "pub.h"

int main(void) {
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
    int n             = 0;
    u_tree_reach(t, it) {
      assert(it.key == desc[n]);
      assert(it.val == it.key * 10);
      n++;
    }
    assert(n == 5);

    n = 0;
    u_tree_reach_if(t, it, it.key < 7) {
      n++;
    }
    assert(n == 3);

    n = 0;
    u_tree_reach_ref(t, ref) {
      assert(ref->key == desc[n]);
      assert(ref->val == desc[n] * 10);
      n++;
    }
    assert(n == 5);

    n = 0;
    u_tree_reach_if_ref(t, ref, ref->val < 50) {
      n++;
    }
    assert(n == 2);

    /* reach_mut: 反向修改 val */
    u_tree_reach_mut(t, mut) {
      mut->val += 1;
    }
    assert(u_tree_at(t, 9) == 91);
    assert(u_tree_at(t, 1) == 11);

    n = 0;
    u_tree_reach_if_mut(t, mut, mut->key == 5) {
      n++;
      mut->val = 0;
    }
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
    auto ft = u_tree_filter_if(t, it.val >= 20); /* [2,3] */
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
    auto em              = u_tree_map_by(e, { it.val = 0; });
    auto ef              = u_tree_filter_if(e, it.val > 0);
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
    u_tree_reach(t, it) {
      assert(!"reach must not iterate");
    }
    u_tree_reach_ref(t, it) {
      assert(!"reach_ref must not iterate");
    }
    u_tree_reach_mut(t, it) {
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

  return 0;
}
