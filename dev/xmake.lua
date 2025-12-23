local deps = { 'mimalloc', 'tbox' }

add_requires(unpack(deps))

target('dep', function()
  set_kind('static')
  set_default('false')

  -- add_files('dyad.c')

  add_packages(unpack(deps))
end)

target('dev.c', function()
  set_kind('binary')
  set_default(false)
  add_files('dev.c')
  set_rundir('$(projectdir)')

  -- add_rules('generic')

  -- add_deps('dep')
  add_deps('u')

  add_packages(unpack(deps))

  -- after_build(function(target)
  --   os.trycp('$(projectdir)/$(builddir)/$(plat)/$(arch)/$(mode)/' .. target:name(), '$(projectdir)')
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
      { 't', 'target', 'kv', 'dev.c', 'target name' },
    },
  })

  on_run(function()
    import('core.base.option')
    import('devel.git')
    import('privilege.sudo')
    import('core.project.project')
    import('core.project.config')

    os.cd('$(builddir)')

    local target = project.target(option.get('target'))
    if not target then
      return
    end

    local bin = vformat('%s/$(mode)/%s', target:targetdir(), target:filename())
    print(target:targetfile())
    print(vformat('%s/$(mode)/%s', target:targetdir(), target:filename()))

    print(vformat('%s/%s/%s', target:targetdir(), config.mode(), target:filename()))

    -- download [FlameGraph](https://github.com/brendangregg/FlameGraph)
    if not os.exists('FlameGraph') then
      git.clone('https://github.com/brendangregg/FlameGraph', { depth = 1 })
    end

    -- sudo.exec("bash -c 'perf record -F 997 -g %s'", bin)
    -- sudo.exec("bash -c 'perf script -i perf.data > perf.unfold'")

    -- sudo.exec("bash -c 'FlameGraph/stackcollapse-perf.pl perf.unfold > perf.folded'")
    -- sudo.exec("bash -c 'FlameGraph/flamegraph.pl perf.folded > ../perf.svg'")
  end)
end)
