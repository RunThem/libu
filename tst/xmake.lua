add_requires('mini-unit-test')

target('check', function()
  set_kind('binary')
  add_files('*.c')
  set_default('false')

  add_deps('u')

  add_defines('N1k=1000', 'N1W=10000', 'N10W=100000', 'N100W=1000000', 'N1000W=10000000')

  add_packages('mini-unit-test')
end)

task('mcheck', function()
  set_menu({
    usage = 'xmake mcheck',
    description = 'Run mem check',
  })

  on_run(function()
    import('core.project.project')
    import('lib.detect.find_program')

    local target = project.target('check')
    local bin_path = target:targetdir() .. '/valgrind/' .. target:name()

    os.exec('xmake f -m valgrind --mimalloc=n')
    os.exec('xmake build -v check')

    if find_program('valgrind') ~= nil then
      local cmd = format('valgrind --tool=memcheck --leak-check=full %s', bin_path)
      os.exec(cmd)
    else
      os.exec('xmake run -v check')
    end
  end)
end)

task('ucheck', function()
  set_menu({
    usage = 'xmake ucheck',
    description = 'Run unit check',
  })

  on_run(function()
    os.exec('xmake f -m release --mimalloc=y')
    os.exec('xmake build -v check')
    os.exec('xmake run -v check')
  end)
end)
