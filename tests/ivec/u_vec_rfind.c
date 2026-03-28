/***************************************************************************************************
 * Test: u_vec_rfind_if / u_vec_rfind_nif / u_vec_rfind_if_ref / u_vec_rfind_if_mut
 *
 * Coverage:
 * - Reverse find element matching condition
 * - Reverse find element not matching condition
 * - Reverse find in empty vector
 * - Reverse find when no element matches
 * - Find last matching element
 * - Reverse find for int type
 * - Reverse find for str_t type
 **************************************************************************************************/

test() {
  /* int type: rfind_if in empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    auto result = u_vec_rfind_if(v, it == 42);
    mut_eq(0, result);
  }

  /* int type: rfind_if single element matching */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    auto result = u_vec_rfind_if(v, it == 42);
    mut_eq(42, result);
  }

  /* int type: rfind_if single element not matching */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    auto result = u_vec_rfind_if(v, it == 0);
    mut_eq(0, result);
  }

  /* int type: rfind_if last matching element */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 10);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 30);
    u_vec_insert_back(v, 20);

    auto result = u_vec_rfind_if(v, it == 20);
    mut_eq(20, result);
  }

  /* int type: rfind_if vs find_if - verify difference */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 10);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 30);
    u_vec_insert_back(v, 20);

    auto first = u_vec_find_if(v, it == 20);
    auto last  = u_vec_rfind_if(v, it == 20);

    mut_eq(20, first);
    mut_eq(20, last);
  }

  /* int type: rfind_if in full vector */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto result = u_vec_rfind_if(v, it == N / 2);
    mut_eq(N / 2, result);
  }

  /* int type: rfind_if when no match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto result = u_vec_rfind_if(v, it == N * 10);
    mut_eq(0, result);
  }

  /* int type: rfind_nif (not matching) */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto result = u_vec_rfind_nif(v, it >= N / 2);
    mut_eq(N / 2 - 1, result);
  }

  /* int type: rfind_if with complex condition */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto result = u_vec_rfind_if(v, it % 100 == 50);
    mut_eq(N - 50, result);
  }

  /* int type: rfind_if_ref */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    const int* p = u_vec_rfind_if_ref(v, *it == 100);
    mut_ptr_neq_nil(p);
    mut_eq(100, *p);
  }

  /* int type: rfind_if_ref when no match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    const int* p = u_vec_rfind_if_ref(v, *it == N * 10);
    mut_ptr_eq_nil(p);
  }

  /* int type: rfind_if_mut */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* p = u_vec_rfind_if_mut(v, *it == 100);
    mut_ptr_neq_nil(p);
    mut_eq(100, *p);

    *p = 999;
    mut_eq(999, u_vec_at(v, 100));
  }

  /* int type: rfind_if_mut when no match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* p = u_vec_rfind_if_mut(v, *it == N * 10);
    mut_ptr_eq_nil(p);
  }

  /* int type: rfind_nif_ref */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    const int* p = u_vec_rfind_nif_ref(v, *it >= N / 2);
    mut_ptr_neq_nil(p);
    mut_eq(N / 2 - 1, *p);
  }

  /* int type: rfind_nif_mut */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* p = u_vec_rfind_nif_mut(v, *it >= N / 2);
    mut_ptr_neq_nil(p);
    mut_eq(N / 2 - 1, *p);
  }

  /* str_t type: rfind_if */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    str_t result = u_vec_rfind_if(v, strcmp(it, "str_100") == 0);
    mut_str_eq("str_100", result);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: rfind_if when no match */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    str_t result = u_vec_rfind_if(v, strcmp(it, "not_exist") == 0);
    mut_ptr_eq_nil(result);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: rfind_if_ref */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    const str_t* p = u_vec_rfind_if_ref(v, strcmp(*it, "str_50") == 0);
    mut_ptr_neq_nil(p);
    mut_str_eq("str_50", *p);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: rfind_if_mut */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    str_t* p = u_vec_rfind_if_mut(v, strcmp(*it, "str_5") == 0);
    mut_ptr_neq_nil(p);
    mut_str_eq("str_5", *p);

    str_free(*p);
    *p = str_new("modified");
    mut_str_eq("modified", u_vec_at(v, 5));

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}