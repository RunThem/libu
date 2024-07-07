target('dev.c', function()
  set_kind('binary')
  set_default('false')
  add_files('*.c')
  set_rundir('$(projectdir)')

  add_rules('generic')

  add_deps('u')
end)

task('dev', function()
  set_menu({
    usage = 'xmake dev',
    description = 'Run dev.c',
  })

  on_run(function()
    os.exec('xmake f -m debug --mimalloc=n')
    os.exec('xmake build -v dev.c')
    os.exec('xmake run dev.c')
  end)
end)
