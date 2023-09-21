#pragma once

#ifdef NDEBUG
#  define u_if(expr, arg...) if (expr)

#  define u_assert(expr) (void)0
#else
#  define u_if(expr, arg...)                                                                       \
    if (expr && (errln("(%s) " va_0th("", arg) " ", #expr va_opt(1, arg) va_slice(1, arg)), true))

#  define u_assert(expr) (expr && (__assert_fail(#  expr, __file__, __line__, __func__), 1))
#endif

#define u_die_if(expr, arg...)                                                                     \
  u_if(expr, arg) {                                                                                \
    exit(EXIT_FAILURE);                                                                            \
  }

#define u_ret_if(expr, code, arg...)                                                               \
  u_if(expr, arg) {                                                                                \
    return code;                                                                                   \
  }

#define u_nonret_if(expr, arg...)                                                                  \
  u_if(expr, arg) {                                                                                \
    return;                                                                                        \
  }

#define u_err_if(expr, arg...)                                                                     \
  u_if(expr, va_slice(1, arg)) {                                                                   \
    goto va_0th(err, arg);                                                                         \
  }

#define u_mem_if(mem, arg...)                                                                      \
  u_if((mem) == nullptr, arg) {                                                                    \
    goto err;                                                                                      \
  }

/*
 * free up resources
 * */
#define u_free_if(mem)  ((mem != nullptr) && (u_free(mem), 1))
#define u_close_if(fd)  ((fd != 0) && (close(fd), 1))
#define u_fclose_if(fp) ((fp != nullptr) && (fclose(fp), 1))
