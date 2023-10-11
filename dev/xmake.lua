target('macro', function()
  set_kind('phony')

  add_defines('N1k=1000', 'N1W=10000', 'N10W=100000', 'N100W=1000000', { public = true })
end)

add_requires('libsock')

target('dev.c', function()
  set_kind('binary')
  set_default('false')
  add_files('dev.c')

  add_deps('u', 'macro')

  add_packages('libsock')
end)

task('dev', function()
  set_menu({
    usage = 'xmake dev',
    description = 'Run dev.c',
  })

  on_run(function()
    os.exec('xmake f -m debug --mimalloc=n --lambda=n')
    os.exec('xmake build -v dev.c')
    os.exec('xmake run dev.c')
  end)
end)

includes('bench', 'check')
