/* clang-format off */
#define N1K     1000
#define N1W     1'0000
#define N10W    10'0000
#define N100W   100'0000
#define N1000W  1000'0000

#define test() int main(int argc, const char* argv[])

#define inf(fmt, ...) fprintf(stderr, "\n" fmt __VA_OPT__(, ) __VA_ARGS__)

#define mut_t(expr) do { if ((expr) != true)    { inf("L.%d: mut_t",  __LINE__); return -1; } } while (0)
#define mut_f(expr) do { if ((expr) != false)   { inf("L.%d: mut_f",  __LINE__); return -1; } } while (0)
#define mut_s(a, b) do { if (0 != strcmp(a, b)) { inf("L.%d: mut_s",  __LINE__); return -1; } } while (0)
#define mut_e(a, b) do { if (a != b)            { inf("L.%d: mut_e",  __LINE__); return -1; } } while (0)
#define mut_a(expr) do { if (!(expr))           { inf("L.%d: mut_a",  __LINE__); return -1; } } while (0)
#define mut_n(ptr)  do { if ((ptr) == nullptr)  { inf("L.%d: mut_n",  __LINE__); return -1; } } while (0)
#define mut_nn(ptr) do { if ((ptr) != nullptr)  { inf("L.%d: mut_nn", __LINE__); return -1; } } while (0)
