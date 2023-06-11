/* libs */
#include "src/buf.h"
#include "src/fs.h"
#include "src/list.h"
#include "src/map.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

int main(int argc, const char** argv) {
  list(int) lst = nullptr;

  list_init(&lst);

  list_push_b(&lst, 1);
  list_push_b(&lst, 2);
  list_push_b(&lst, 3);
  list_push_b(&lst, 4);
  list_push_b(&lst, 5);
  list_push_b(&lst, 6);
  list_push_b(&lst, 7);

  list_iter(int) it = nullptr;

  it = list_get_iter(&lst);

  for (; it != nullptr; it = it->next) {
    inf("%d", it->val);
  }

  return 0;
}
