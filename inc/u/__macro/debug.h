#pragma once

#ifdef NDEBUG
#  define u_if(expr, ...) if (expr)

#  define u_assert(expr) (void)0
#else
#  define u_if(expr, ...)                                                                          \
    if (expr && (errln("(%s) " va_0th("", __VA_ARGS__) " ", #expr va_list(1, __VA_ARGS__)), true))

#  define u_assert(expr) (expr && (__assert_fail(#expr, __file__, __line__, __func__), 1))
#endif

#define u_die_if(expr, ...)                                                                        \
  u_if(expr, __VA_ARGS__) {                                                                        \
    exit(EXIT_FAILURE);                                                                            \
  }

#define u_ret_if(expr, code, ...)                                                                  \
  u_if(expr, __VA_ARGS__) {                                                                        \
    return code;                                                                                   \
  }

#define u_nonret_if(expr, ...)                                                                     \
  u_if(expr, __VA_ARGS__) {                                                                        \
    return;                                                                                        \
  }

#define u_err_if(expr, ...)                                                                        \
  u_if(expr, va_slice(1, __VA_ARGS__)) {                                                           \
    goto va_0th(err, __VA_ARGS__);                                                                 \
  }

#define u_mem_if(mem, ...)                                                                         \
  u_if((mem) == nullptr, __VA_ARGS__) {                                                            \
    goto err;                                                                                      \
  }

/*
 * free up resources
 * */
#define u_free_if(mem)  ((mem != nullptr) && (u_free(mem), 1))
#define u_close_if(fd)  ((fd != 0) && (close(fd), 1))
#define u_fclose_if(fp) ((fp != nullptr) && (fclose(fp), 1))
