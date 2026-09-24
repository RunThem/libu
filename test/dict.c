/***************************************************************************************************
 * Test: Dict<K, V>
 *
 * 覆盖: new / cleanup / insert / at family / try_at family / each family / remove / clear / cleanup
 * proc / stress / empty dict / custom hash fn / rehash / remove + reinsert
 **************************************************************************************************/

#include "pub.h"

int main(void) {
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

  return 0;
}
