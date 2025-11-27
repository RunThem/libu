/* clang-format off */
#define N       100
// #define N1K     1000
// #define N1W     1'0000
// #define N10W    10'0000
// #define N100W   100'0000
// #define N1000W  1000'0000

#define test() int main(int argc, const char* argv[])

#define each(idx, s, e, b) for (i32_t idx = s; (s < e) ? (idx < e) : (idx >= e); idx += b)

#define inf(fmt, ...) fprintf(stderr, "\n" fmt __VA_OPT__(, ) __VA_ARGS__)

// #define mut_t(expr) do { if ((expr) != true)    { inf("L.%d: mut_t(false)",  __LINE__);            return -1; } } while (0)
// #define mut_f(expr) do { if ((expr) != false)   { inf("L.%d: mut_f(true)",  __LINE__);             return -1; } } while (0)
// #define mut_s(a, b) do { if (0 != strcmp(a, b)) { inf("L.%d: mut_s('%s', '%s')",  __LINE__, a, b); return -1; } } while (0)
// #define mut_a(expr) do { if (!(expr))           { inf("L.%d: mut_a(%d)",  __LINE__, expr);         return -1; } } while (0)
// #define mut_n(ptr)  do { if ((ptr) == nullptr)  { inf("L.%d: mut_n(nil)",  __LINE__);              return -1; } } while (0)
// #define mut_nn(ptr) do { if ((ptr) != nullptr)  { inf("L.%d: mut_nn(%p)", __LINE__, ptr);          return -1; } } while (0)
// #define mut_e(a, b) do { if (a != b)            { inf("L.%d: mut_e(%zu, %zu)",  __LINE__, (ssize_t)a, (ssize_t)b);   return -1; } } while (0)

#define mut_true(expr)       do { if (!(expr))       { inf("L.%d: %s == false",  __LINE__, #expr); return -1; } } while (0)
#define mut_false(expr)      do { if ((expr))        { inf("L.%d: %s == true",  __LINE__, #expr);  return -1; } } while (0)
#define mut_assert(expr)     do { if (!(expr))       { inf("L.%d: %s",  __LINE__, #expr);          return -1; } } while (0)
#define mut_ptr_neq_nil(ptr) do { if (!(ptr))        { inf("L.%d: %s == nil",  __LINE__, #ptr);    return -1; } } while (0)
#define mut_ptr_eq_nil(ptr)  do { if ((ptr))         { inf("L.%d: %s != nil", __LINE__, #ptr);     return -1; } } while (0)
#define mut_eq(a, b)         do { if (a != b)        { inf("L.%d: %s != %s",  __LINE__, #a, #b);   return -1; } } while (0)
#define mut_str_eq(a, b)     do { if (!strcmp(a, b)) { inf("L.%d: '%s' != '%s'",  __LINE__, a, b); return -1; } } while (0)
#define mut_ptr_eq(a, b)     do { if (any(a) != any(b)) { inf("L.%d: %s != %s",  __LINE__, #a, #b);   return -1; } } while (0)
