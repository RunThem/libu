target('chibicc', function()
  set_kind('binary')
  set_default('false')
  add_files('*.c')
  set_rundir('$(projectdir)')

  add_deps('u')

  after_build(function(T)
    local bin = vformat('$(projectdir)/$(builddir)/$(plat)/$(arch)/$(mode)/%s', T:filename())
    os.cp(bin, '$(projectdir)/chibicc')
  end)
end)

task('tc', function()
  set_menu({
    usage = 'xmake tc',
    description = 'Run chibicc',
  })

  on_run(function()
    os.exec('xmake build -v chibicc')
    os.cd('$(projectdir)/chibicc')
    os.exec('./test.sh')
    os.exec('rm tmp tmp.s')
  end)
end)
