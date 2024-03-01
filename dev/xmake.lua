target('macro', function()
  set_kind('phony')

  add_defines('N1k=1000', 'N1W=10000', 'N10W=100000', 'N100W=1000000', 'N1000W=10000000', { public = true })
end)

add_requires('miniz', 'libsock')

target('dev.c', function()
  set_kind('binary')
  set_default('false')
  add_files('dev.c')
  set_rundir('$(projectdir)')

  add_deps('u', 'macro')

  add_packages('miniz', 'libsock', 'tbox')
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

includes('bench', 'check')
