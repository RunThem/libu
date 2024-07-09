for _, file in ipairs(os.files('test_*.c')) do
  local name = path.basename(file)

  target(name, function()
    set_kind('binary')
    set_default(false)
    add_files(file)

    add_undefines('USE_MIMALLOC')
    add_cflags('-include $(projectdir)/tests/mut.h')

    add_deps('u')
    add_rules('generic')

    add_tests('default')
  end)
end
