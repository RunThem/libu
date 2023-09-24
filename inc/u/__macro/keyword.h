#define auto __auto_type

#if !defined(__clang__) || __clang_major__ < 17
#  define nullptr NULL
#endif
