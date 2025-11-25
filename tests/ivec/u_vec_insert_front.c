test() {

  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_front(v, i);

      mut_eq(i + 1, v->len);
    }

    each(i, N - 1, -1, -1) {
    }
  }

  return 0;
}
