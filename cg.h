#include <sys/types.h>

typedef struct {
}* u_vec_t;

#define kvec(T) typeof(u_vec_t(*)(ssize_t, T))
