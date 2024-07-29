add_requires('libsock', 'tbox', 'xxhash', 'avlmini')

target('dev.c', function()
  set_kind('binary')
  set_default('false')
  add_files('*.c')
  set_rundir('$(projectdir)')

  add_rules('generic')

  add_deps('u')
  add_packages('libsock', 'tbox', 'xxhash', 'avlmini')

  -- after_build(function(target)
  --   os.trycp('$(projectdir)/$(buildir)/$(plat)/$(arch)/$(mode)/' .. target:name(), '$(projectdir)')
  -- end)
end)

task('dev', function()
  set_menu({
    usage = 'xmake dev',
    description = 'Run dev.c',
  })

  on_run(function()
    os.exec('xmake build -v dev.c')
    os.exec('xmake run dev.c')
  end)
end)

task('perf', function()
  set_menu({
    usage = 'xmake perf',
    description = 'perf',
    options = {
      { 'p', 'perfdata', 'kv', 'build/perf.data', 'perf output filename' },
    },
  })

  on_run(function()
    import('core.base.option')
    import('devel.git')
    import('privilege.sudo')

    local pd = option.get('perfdata')
    local fd = vformat('$(buildir)/FlameGraph')

    if not os.exists(pd) then
      cprint('${bright green}usage: sudo perf record -e cpu-clock -g -o $(buildir)/perf.data -p 1234')

      return
    end

    -- download [FlameGraph](https://github.com/brendangregg/FlameGraph)
    if not os.exists(fd) then
      git.clone('https://github.com/brendangregg/FlameGraph', { depth = 1, outputdir = fd })
    end

    cprint('perf script -i %s &> $(buildir)/perf.unfold', pd)
    -- sudo.run('perf script -i %s &> $(buildir)/perf.unfold', pd)
    -- sudo.run('%s/stackcollapse-perf.pl $(buildir)/perf.unfold &> $(buildir)/perf.folded', fd)
    -- sudo.run('%s/flamegraph.pl $(buildir)/perf.folded > perf.svg', fd)
  end)
end)
