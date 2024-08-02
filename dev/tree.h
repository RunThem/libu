#pragma once

#ifndef _TREE_H__
#  define _TREE_H__

#  include <u/u.h>

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef intmax_t tree_key_t;
typedef any_t tree_val_t;

typedef struct {
  size_t len;
  any_t root;
} tree_t, *tree_ref_t;

/* clang-format off */
extern pub any_t      tree_new      ();
extern pub void       tree_clear    (tree_ref_t self);
extern pub void       tree_cleanup  (tree_ref_t self);
extern pub size_t     tree_len      (tree_ref_t self);
extern pub bool       tree_exist    (tree_ref_t self, tree_key_t key);
extern pub any_t      tree_at       (tree_ref_t self, tree_key_t key);
extern pub tree_val_t tree_pop      (tree_ref_t self, tree_key_t key);
extern pub void       tree_put      (tree_ref_t self, tree_key_t key, tree_val_t val);

extern pub void       tree_dump     (tree_ref_t self);
/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !_TREE_H__ */
