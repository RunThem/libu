add_repositories('RunThem https://github.com/RunThem/My-xmake-repo')
add_requires('mini-unit-test')

target('check', function()
  set_kind('binary')
  add_files('*.c')
  set_default('false')

  add_packages('mini-unit-test')

  add_deps('u', 'macro')
end)

task('tests', function()
  set_menu({
    usage = 'xmake check',
    description = 'Run unit test',
  })

  on_run(function()
    import('core.project.project')
    local target = project.target('check')
    local bin_path = target:targetdir() .. '/debug/' .. target:name()

    os.exec('xmake f -m debug --mimalloc=n')
    os.exec('xmake build -v check')
    os.exec('xmake run -v check')
  end)
end)

task('check', function()
  set_menu({
    usage = 'xmake check',
    description = 'Run mem check',
  })

  on_run(function()
    import('core.project.project')
    local target = project.target('check')
    local bin_path = target:targetdir() .. '/valgrind/' .. target:name()

    os.exec('xmake f -m valgrind --mimalloc=n')
    os.exec('xmake build -v check')

    local cmd = format('valgrind --tool=memcheck --leak-check=full %s', bin_path)
    os.exec(cmd)
  end)
end)
