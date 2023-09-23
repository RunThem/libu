target('check', function()
  set_kind('binary')
  add_files('*.c')
  set_default('false')

  add_deps('u', 'macro')
end)

task('check', function()
  set_menu({
    usage = 'xmake check',
    description = 'Run check',
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
