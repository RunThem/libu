target('codegen', function()
  set_kind('phony')
  -- add_files('main.c')
  -- set_default('false')

  add_configfiles('main.c.in', { variables = { funcs = "{ 'vec_new', test_vec_new }," } })
end)
