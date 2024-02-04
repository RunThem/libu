#define __file__ __FILE__
#define __line__ __LINE__
#define __func__ __func__

#define print(fmt, ...)   fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)
#define println(fmt, ...) fprintf(stderr, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)

#ifdef NDEBUG
#  define __display(fmt, ...)
#else
#  define __display(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)
#endif

#define __INF_FMT       "[INF]\x1b[02m[%s $%d %s]\x1b[0m: "
#define __inf_args()    __file__, __line__, __func__
#define inf(fmt, ...)   __display(__INF_FMT fmt, __inf_args() __VA_OPT__(, ) __VA_ARGS__)
#define infln(fmt, ...) __display(__INF_FMT fmt "\n", __inf_args() __VA_OPT__(, ) __VA_ARGS__)

#define __ERR_FMT       "[ERR]\x1b[02m[%s $%d %s]\x1b[0m {%s}: "
#define __err_args()    __file__, __line__, __func__, strerror(errno)
#define err(fmt, ...)   __display(__ERR_FMT fmt, __err_args() __VA_OPT__(, ) __VA_ARGS__)
#define errln(fmt, ...) __display(__ERR_FMT fmt "\n", __err_args() __VA_OPT__(, ) __VA_ARGS__)

extern void __inf_hex(const u8_t* arr, size_t size);
#define inf_hex(arr, size)                                                                         \
  do {                                                                                             \
    infln("\x1b[36;1m%s\x1b[0m(%ld)", #arr, size);                                                 \
    __inf_hex(as(arr, u8_t*), size);                                                               \
  } while (0)

void __inf_bit(const u8_t* buf, size_t size);
#define inf_bit(arr, size)                                                                         \
  do {                                                                                             \
    infln("\x1b[36;1m%s\x1b[0m(%ld)", #arr, size);                                                 \
    __inf_bit(as(arr, u8_t*), size);                                                               \
  } while (0)
