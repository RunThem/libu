target('bench', function()
  set_kind('binary')
  add_files('*.c')
  set_default('false')

  add_deps('u', 'macro')

  add_packages('mimalloc')
end)

task('bench', function()
  set_menu({
    usage = 'xmake bench',
    description = 'Run benchmark',
  })

  on_run(function()
    --[[
    import('core.project.project')
    local target = project.target('bench')
    local bin = target:targetdir() .. '/profile/' .. target:name()
    local gmon = target:targetdir() .. '/profile/' .. 'gmon.out'

    os.exec('xmake f -m profile --mimalloc=n --lambda=n')
    os.exec('xmake build -v bench')
    os.exec('xmake run bench')

    if os.exists(gmon) then
      local cmd = format('gprof -b %s %s', bin, gmon)
      os.exec(cmd)
    end
    --]]

    os.exec('xmake f -m release --mimalloc=y --lambda=n')
    os.exec('xmake build -v bench')
    os.exec('xmake run bench')
  end)
end)
