#include <sys/types.h>

typedef struct {
}* u_vec_t;

#define uvec(T) typeof(u_vec_t(*)(ssize_t, T))
#if 0
#  define uK(u)
#  define uT(u)
#endif

typedef struct {
}* u_avl_t;

#define uavl(K, T) typeof(u_avl_t(*)(K, T))