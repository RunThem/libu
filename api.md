# 容器
 - arr (???)
 - vec
 - tbl
 - avl
 - lst

## iApi
 - init
 - new
 - clear
 - cleanup
 - isinit
 - isempty
 - isexist
 - len
 - cap (vec)
 - at
 - re
 - pop
 - put
 - first
 - last
 - next
 - prev
 - for
 - rfor

```c
void vec_first(u_vec_t, any_t, any_t)
void tbl_first(u_vec_t, any_t, any_t)
void tbl_first(u_vec_t, any_t, any_t)

void vec_last(u_vec_t, any_t, any_t)
void tbl_last(u_vec_t, any_t, any_t)
void tbl_last(u_vec_t, any_t, any_t)

bool vec_prev(u_vec_t, any_t, any_t)
bool tbl_prev(u_vec_t, any_t, any_t)
bool tbl_prev(u_vec_t, any_t, any_t)

bool vec_next(u_vec_t, any_t, any_t)
bool tbl_next(u_vec_t, any_t, any_t)
bool tbl_next(u_vec_t, any_t, any_t)
#define u_for(u, args...) for (vec_for_init(u->_.mate); vec_for(u->_.mate, &va_at(0, args),
&va_at(1, args)))
```
