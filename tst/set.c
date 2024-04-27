#include <mut.h>

#define u_set_defs u_defs(set, item)
#include <u/u.h>

typedef struct {
  int a;
  int b;
} item;

fn_compe_def(item, x.a == y.a, x.a > y.a);
fn_compe_dec(item);

mut_test(set_create) {
  u_set_t(item) s = nullptr;

  u_set_init(s, fn_cmp(item));

  mut_assert(s != nullptr);
  mut_assert(0 == u_set_len(s));

  u_set_cleanup(s);

  mut_assert(s == nullptr);
}

mut_test(set_interface) {
  u_set_t(item) s = nullptr;

  item a = {10, 1000};
  item b = {11, 2000};
  item c = {12, 3000};
  item d = {13, 4000};

  u_set_init(s, fn_cmp(item));

  mut_assert(true == u_set_is_empty(s));

  u_set_put(s, a);
  u_set_put(s, b);
  u_set_put(s, c);

  mut_assert(false == u_set_is_empty(s));
  mut_assert(3 == u_set_len(s));

  mut_assert(1000 == u_set_at(s, a).b);
  mut_assert(2000 == u_set_at(s, b).b);
  mut_assert(3000 == u_set_at(s, c).b);

  mut_assert(1000 == u_set_pop(s, a).b);

  mut_assert(2 == u_set_len(s));

  mut_assert(2000 == u_set_at(s, b).b);
  mut_assert(3000 == u_set_at(s, c).b);

  u_set_at(s, b, c);

  mut_assert(2 == u_set_len(s));

  mut_assert(2000 == u_set_at(s, b).b);
  mut_assert(3000 == u_set_at(s, c).b);

  b.b = 5000;
  u_set_at(s, b, b);

  mut_assert(2 == u_set_len(s));

  mut_assert(5000 == u_set_at(s, b).b);
  mut_assert(3000 == u_set_at(s, c).b);

  u_set_cleanup(s);
}

mut_test(set_iterator) {
  u_set_t(item) s = nullptr;

  u_set_init(s, fn_cmp(item));

  item a = {0, 1000};
  item b = {1, 2000};
  item c = {2, 3000};
  item d = {3, 4000};

  u_set_put(s, a);
  u_set_put(s, a);
  u_set_put(s, a);
  u_set_put(s, d);

  u_set_for(s, it) {
    mut_assert(it.b == (it.a + 1) * 1000);
  }

  u_set_rfor(s, it) {
    mut_assert(it.b == (it.a + 1) * 1000);
  }

  u_set_cleanup(s);
}

mut_group(set) {

  mut_add_test(set_create, "create set instance.");

  mut_add_test(set_interface, "set interface.");

  mut_add_test(set_iterator, "set iterator.");
}
