#pragma once
#ifndef U_ICO_H__
#  define U_ICO_H__

#  ifdef __cplusplus
extern "C" {
#  endif

void co_new(void* fun, ...);

void co_yield (int state);

void co_loop();

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ICO_H__ */
