local deps = { 'libsock', 'avlmini' }

add_requires(unpack(deps))

target('tree', function()
  set_kind('static')
  set_default('false')
  add_files('tree.c')

  add_deps('u')
end)

target('dev.c', function()
  set_kind('binary')
  set_default('false')
  add_files('dev.c')
  set_rundir('$(projectdir)')

  add_rules('generic')

  add_deps('u')
  add_deps('tree')
  add_packages(unpack(deps))

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
      { 't', 'target', 'kv', 'dev.c', 'target name' },
    },
  })

  on_run(function()
    import('core.base.option')
    import('devel.git')
    import('privilege.sudo')
    import('core.project.project')

    local fg = vformat('$(buildir)/FlameGraph')
    local sh = vformat('$(buildir)/perf.sh')
    local target = project.target(option.get('target'))

    if not target then
      return
    end

    target = vformat('%s/debug/%s', target:targetdir(), target:filename())

    -- download [FlameGraph](https://github.com/brendangregg/FlameGraph)
    if not os.exists(fg) then
      git.clone('https://github.com/brendangregg/FlameGraph', { depth = 1, outputdir = fg })
    end

    local script = [[
#!/usr/bin/env bash

set -x

./build/linux/x86_64/debug/dev.c &>/dev/null &
pid=$!

sleep 1

sudo perf record -g -o perf.data -p ${pid} -- sleep 10
sudo perf script -i perf.data &>perf.unfold
./build/FlameGraph/stackcollapse-perf.pl perf.unfold &>perf.folded
./build/FlameGraph/flamegraph.pl perf.folded >perf.svg

rm -f perf.data perf.unfold perf.folded
]]

    io.writefile('build/perf.sh', format(script, target))

    sudo.exec('bash build/perf.sh')
  end)
end)
