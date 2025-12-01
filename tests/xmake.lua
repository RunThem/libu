for _, file in ipairs(os.files('**.c')) do
  local name = path.basename(file)

  target(name, function()
    set_kind('binary')
    set_default(false)
    add_files(file)
    set_rundir('$(projectdir)')

    add_cflags('-include $(projectdir)/tests/pub.h', { force = true })

    add_deps('u')

    add_tests('default')
  end)
end
