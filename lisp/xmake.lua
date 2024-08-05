local deps = { 'mpc', 'linenoise' }

add_requires(unpack(deps))

target('lisp', function()
  set_kind('binary')
  set_default('false')
  add_files('*.c')
  set_rundir('$(projectdir)')

  add_rules('generic')
  add_deps('u')
  add_packages(unpack(deps))

  after_build(function(target)
    os.trycp('$(projectdir)/$(buildir)/$(plat)/$(arch)/$(mode)/' .. target:name(), '$(projectdir)')
  end)
end)

task('lisp', function()
  set_menu({
    usage = 'xmake lisp',
    description = 'Run lisp',
  })

  on_run(function()
    os.exec('xmake build -v lisp')
    os.exec('xmake run lisp')
  end)
end)
