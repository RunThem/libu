#pragma once

#define u_if(expr, arg...) if (expr && (err("(%s)" va_0th("", arg), #expr va_slice(1, arg)), true))

#define u_assert(expr)                                                                             \
  do {                                                                                             \
    if (expr) {                                                                                    \
      fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", #expr, __file__, __func__, __line__); \
      abort();                                                                                     \
    }                                                                                              \
  } while (0)

#define u_die_if(expr, arg...)                                                                     \
  do {                                                                                             \
    if (expr) {                                                                                    \
      err("(%s) " va_0th("", arg), #expr va_slice(1, arg));                                        \
      exit(EXIT_FAILURE);                                                                          \
    }                                                                                              \
  } while (0)

#define u_ret_if(expr, code, arg...)                                                               \
  do {                                                                                             \
    if (expr) {                                                                                    \
      err("(%s) " va_0th("", arg), #expr va_slice(1, arg));                                        \
      return code;                                                                                 \
    }                                                                                              \
  } while (0)

#define u_noret_if(expr, arg...)                                                                   \
  do {                                                                                             \
    if (expr) {                                                                                    \
      err("(%s) " va_0th("", arg), #expr va_slice(1, arg));                                        \
      return;                                                                                      \
    }                                                                                              \
  } while (0)

#define u_err_if(expr, arg...)                                                                     \
  do {                                                                                             \
    if (expr) {                                                                                    \
      err("(%s) " va_1th("", arg), #expr va_slice(2, arg));                                        \
      goto va_0th(err, arg);                                                                       \
    }                                                                                              \
  } while (0)

#define u_mem_if(mem, arg...) u_err_if((mem) == nullptr, err, arg)

/*
 * free up resources
 * */
#define u_free_if(mem)                                                                             \
  do {                                                                                             \
    if ((mem) != nullptr) {                                                                        \
      u_free(mem);                                                                                 \
    }                                                                                              \
  } while (0)

#define u_close_if(fd)                                                                             \
  do {                                                                                             \
    if ((fd) >= 0) {                                                                               \
      close(fd);                                                                                   \
    }                                                                                              \
  } while (0)

#define u_fclose_if(fp)                                                                            \
  do {                                                                                             \
    if ((fp) != nullptr) {                                                                         \
      fclose(fd);                                                                                  \
    }                                                                                              \
  } while (0)
