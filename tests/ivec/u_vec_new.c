/***************************************************************************************************
 * Test: u_vec_new
 *
 * Coverage:
 * - Create vector with default capacity
 * - Create vector with custom capacity
 * - Create vector for int type
 * - Create vector for str_t (dynamic memory) type
 * - Create vector for person_t (struct with nested pointer) type
 * - Empty vector initialization
 **************************************************************************************************/

test() {
  /* int type: default capacity */
  {
    u_vec_t(int) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(v);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);
    mut_eq(16, v->cap);
    mut_eq(0, v->len);
  }

  /* int type: custom capacity */
  {
    u_vec_t(int) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(v, 64);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);
    mut_eq(64, v->cap);
    mut_eq(0, v->len);
  }

  /* int type: large capacity */
  {
    u_vec_t(int) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(v, N);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);
    mut_eq(N, v->cap);
    mut_eq(0, v->len);
  }

  /* str_t type: default capacity */
  {
    u_vec_t(str_t) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(v);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);
    mut_eq(16, v->cap);
    mut_eq(0, v->len);
  }

  /* str_t type: custom capacity */
  {
    u_vec_t(str_t) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(v, 128);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);
    mut_eq(128, v->cap);
    mut_eq(0, v->len);
  }

  /* person_t type: default capacity */
  {
    u_vec_t(person_t) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(v);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);
    mut_eq(16, v->cap);
    mut_eq(0, v->len);
  }

  /* person_t type: custom capacity */
  {
    u_vec_t(person_t) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(v, 256);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);
    mut_eq(256, v->cap);
    mut_eq(0, v->len);
  }

  /* Multiple vectors independence */
  {
    u_vec_t(int) v1 = u_vec_new(v1);
    u_vec_t(int) v2 = u_vec_new(v2, 32);

    mut_ptr_neq_nil(v1);
    mut_ptr_neq_nil(v2);
    mut_ptr_neq_nil(v1->ref);
    mut_ptr_neq_nil(v2->ref);
    mut_ptr_neq(v1->ref, v2->ref);

    mut_eq(16, v1->cap);
    mut_eq(32, v2->cap);
    mut_eq(0, v1->len);
    mut_eq(0, v2->len);
  }

  return 0;
}