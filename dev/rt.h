#pragma once

#ifndef __RT_H
#  define __RT_H

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Print
 **************************************************************************************************/
#  ifndef rt_printf
#    define rt_printf(...) fprintf(stderr, __VA_ARGS__)
#  endif /* rt_printf */

/***************************************************************************************************
 * rt_xxx_if
 **************************************************************************************************/
#  define u_die_if(expr)                                                                           \
    if (expr) {                                                                                    \
      u_xxx("<%s>", is_ptr(expr) ? #expr " == nil" : #expr);                                       \
                                                                                                   \
      exit(EXIT_FAILURE);                                                                          \
    }                                                                                              \
    do {                                                                                           \
    } while (0)

#  define u_brk_if(expr)                                                                           \
    if ((is_ptr(expr) && !(expr)) || (!is_ptr(expr) && (expr))) {                                  \
      u_xxx("<%s>", is_ptr(expr) ? #expr " == nil" : #expr);                                       \
                                                                                                   \
      break;                                                                                       \
    }                                                                                              \
    do {                                                                                           \
    } while (0)

#  define u_cnt_if(expr)                                                                           \
    if ((is_ptr(expr) && !(expr)) || (!is_ptr(expr) && (expr))) {                                  \
      u_xxx("<%s>", is_ptr(expr) ? #expr " == nil" : #expr);                                       \
                                                                                                   \
      continue;                                                                                    \
    }                                                                                              \
    do {                                                                                           \
    } while (0)

#  define u_chk_if(expr, ...)                                                                      \
    if ((is_ptr(expr) && !(expr)) || (!is_ptr(expr) && (expr))) {                                  \
      u_xxx("<%s>", is_ptr(expr) ? #expr " == nil" : #expr);                                       \
                                                                                                   \
      return u_va_at(0, __VA_ARGS__);                                                              \
    }                                                                                              \
    do {                                                                                           \
    } while (0)

#  define u_end_if(expr, ...)                                                                      \
    if ((is_ptr(expr) && !(expr)) || (!is_ptr(expr) && (expr))) {                                  \
      u_xxx("<%s>", is_ptr(expr) ? #expr " == nil" : #expr);                                       \
                                                                                                   \
      goto u_va_0th(end, __VA_ARGS__);                                                             \
    }                                                                                              \
    do {                                                                                           \
    } while (0)

#  define u_free_if(mem)                                                                           \
    if (mem != nullptr) {                                                                          \
      u_free(mem);                                                                                 \
    }                                                                                              \
    do {                                                                                           \
    } while (0)

#  define u_close_if(fd)                                                                           \
    if (fd >= 0) {                                                                                 \
      close(fd);                                                                                   \
    }                                                                                              \
    do {                                                                                           \
    } while (0)

#  define u_fclose_if(fp)                                                                          \
    if (fp != nullptr) {                                                                           \
      fclose(fp);                                                                                  \
    }                                                                                              \
    do {                                                                                           \
    } while (0)
#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* __RT_H */
