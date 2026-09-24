/***************************************************************************************************
 * Test: Array<T>
 *
 * 覆盖: len / each / each_mut / reach / reach_mut / each_if / reach_if / find / rfind / all_if /
 * any_if / nested
 **************************************************************************************************/

#include "pub.h"

int main(void) {
  printf("================ Array<T> ================\n");

  /* ---- 1. len / each: 值、顺序、副本语义 ---- */
  {
    int a[] = {1, 2, 3, 4};
    int sum = 0, n = 0, last = 0, expect = 1;

    assert(u_arr_len(a) == 4);

    u_arr_each (a, it) {
      assert(it == expect++); /* 每个元素都要走到, 且顺序正确 */
      sum += it;
      last = it;
      n++;
    }
    assert(n == 4);
    assert(sum == 10);
    assert(last == 4);

    /* each 给的是副本: 改 it 不影响数组 */
    u_arr_each (a, it) {
      it = 0;
    }
    assert(a[0] == 1);
    assert(a[3] == 4);

    /* const 数组也能 each */
    const int c[] = {5, 6};

    sum = 0;
    u_arr_each (c, it) {
      sum += it;
    }
    assert(sum == 11);
  }
  printf("  len / each          ok\n");

  /* ---- 2. each_mut: 指向元素本身 ---- */
  {
    int a[] = {1, 2, 3};
    int sum = 0;

    u_arr_each_mut (a, it) {
      sum += *it;
      *it *= 10;
    }
    assert(sum == 6);
    assert(a[0] == 10);
    assert(a[2] == 30);
  }
  printf("  each_mut            ok\n");

  /* ---- 3. reach / reach_mut: 倒序 ---- */
  {
    int a[] = {1, 2, 3};
    int n = 0, expect = 3;

    u_arr_reach (a, it) {
      assert(it == expect--);
      n++;
    }
    assert(n == 3);

    u_arr_reach_mut (a, it) {
      *it += 1;
    }
    assert(a[0] == 2);
    assert(a[2] == 4);
  }
  printf("  reach / reach_mut   ok\n");

  /* ---- 4. 带条件的迭代 ---- */
  {
    int a[] = {1, 2, 3, 4};
    int sum = 0;

    u_arr_each_if (a, it, it % 2 == 0) {
      sum += it;
    }
    assert(sum == 6);

    sum = 0;
    u_arr_each_if_mut (a, it, *it > 2) {
      sum += *it;
    }
    assert(sum == 7);

    sum = 0;
    u_arr_reach_if (a, it, it > 1) {
      sum += it;
    }
    assert(sum == 9);

    sum = 0;
    u_arr_reach_if_mut (a, it, *it % 2 == 0) {
      sum += *it;
    }
    assert(sum == 6);
  }
  printf("  each_if / reach_if  ok\n");

  /* ---- 5. find / rfind ---- */
  {
    int a[] = {1, 2, 3, 2};
    int b[] = {1, 2, 3};
    int* p  = NULL;

    assert(u_arr_find_if(a, it == 2) == 2);  /* 正序第一个 */
    assert(u_arr_find_if(a, it == 9) == 0);  /* 未命中: 零值 */
    assert(u_arr_rfind_if(a, it == 2) == 2); /* 倒序第一个 */
    assert(u_arr_rfind_if(a, it == 9) == 0);
    assert(u_arr_find_nif(b, it == 1) == 2);
    assert(u_arr_rfind_nif(b, it == 3) == 2);

    p = u_arr_find_if_mut(a, *it == 2);
    assert(p == &a[1]);
    *p = 20;
    assert(a[1] == 20);

    p = u_arr_rfind_if_mut(a, *it == 2);
    assert(p == &a[3]);

    assert(u_arr_find_if_mut(a, *it == 9) == NULL);
    assert(u_arr_rfind_if_mut(a, *it == 9) == NULL);
    assert(u_arr_find_nif_mut(b, *it == 1) == &b[1]);
    assert(u_arr_rfind_nif_mut(b, *it == 3) == &b[1]);
  }
  printf("  find / rfind        ok\n");

  /* ---- 6. all / any ---- */
  {
    int a[] = {1, 2, 3};

    assert(u_arr_all_if(a, it > 0));
    assert(!u_arr_all_if(a, it > 1));
    assert(u_arr_any_if(a, it == 3));
    assert(!u_arr_any_if(a, it == 9));
  }
  printf("  all_if / any_if     ok\n");

  /* ---- 7. 嵌套: 分行写 ---- */
  {
    int a[]   = {1, 2, 3};
    int pairs = 0, sum = 0;

    u_arr_each (a, x) {
      u_arr_each (a, y) {
        pairs++;
        sum += x * y;
      }
    }
    assert(pairs == 9);
    assert(sum == 36); /* (1 + 2 + 3) ^ 2 */
  }
  printf("  nested              ok\n");

  printf("============ all Array tests passed ============\n");

  printf("\n");

  return 0;
}
