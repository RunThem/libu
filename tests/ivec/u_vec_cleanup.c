/***************************************************************************************************
 * Test: u_vec_cleanup
 *
 * Coverage:
 * - Cleanup empty vector
 * - Cleanup vector with int elements
 * - Cleanup vector with str_t elements (with cleanup callback)
 * - Cleanup vector with person_t elements (with cleanup callback)
 * - Verify vector is set to NULL after cleanup
 **************************************************************************************************/

test() {
  /* int type: cleanup empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_ptr_neq_nil(v);

    u_vec_cleanup(v);
    mut_ptr_eq_nil(v);
  }

  /* int type: cleanup vector with elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N, v->len);

    u_vec_cleanup(v);
    mut_ptr_eq_nil(v);
  }

  /* str_t type: cleanup empty vector without callback */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    mut_ptr_neq_nil(v);

    u_vec_cleanup(v);
    mut_ptr_eq_nil(v);
  }

  /* str_t type: cleanup vector with elements and free callback */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N, v->len);

    u_vec_cleanup(v, str_free(it));
    mut_ptr_eq_nil(v);
  }

  /* str_t type: cleanup small vector */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N_SMALL, v->len);

    u_vec_cleanup(v, str_free(it));
    mut_ptr_eq_nil(v);
  }

  /* person_t type: cleanup empty vector */
  {
    u_vec_t(person_t) v = u_vec_new(v);
    mut_ptr_neq_nil(v);

    u_vec_cleanup(v);
    mut_ptr_eq_nil(v);
  }

  /* person_t type: cleanup vector with elements and free callback */
  {
    u_vec_t(person_t) v = make_person_vec(N)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N, v->len);

    u_vec_cleanup(v, person_free(&it));
    mut_ptr_eq_nil(v);
  }

  /* person_t type: cleanup small vector */
  {
    u_vec_t(person_t) v = make_person_vec(N_SMALL)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N_SMALL, v->len);

    u_vec_cleanup(v, person_free(&it));
    mut_ptr_eq_nil(v);
  }

  /* Multiple vectors cleanup */
  {
    u_vec_t(int) v1 = make_vec(N)->ref;
    u_vec_t(str_t) v2 = make_str_vec(N)->ref;
    u_vec_t(person_t) v3 = make_person_vec(N)->ref;

    u_vec_cleanup(v1);
    mut_ptr_eq_nil(v1);

    u_vec_cleanup(v2, str_free(it));
    mut_ptr_eq_nil(v2);

    u_vec_cleanup(v3, person_free(&it));
    mut_ptr_eq_nil(v3);
  }

  return 0;
}