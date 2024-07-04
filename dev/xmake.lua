---[[
rule('codegen', function()
  on_config(function(target)
    for _, batch in pairs(target:sourcebatches()) do
      if batch.sourcekind == 'cc' then
        for _, sourcefile in ipairs(batch.sourcefiles) do
          local context = io.readfile(sourcefile)
          local file = vformat('$(buildir)/codegen/' .. sourcefile .. '.h')
          local file_fp = io.open(file, 'w+')

          -- kvec<T>
          local ctx = ''
          -- print(context:gmatch('kvec%(([%w_ ]+)%)'))

          for k, t in context:gmatch('kvec%(([.]+)%)') do
            print(t, k)
            ctx = ctx .. (', kvec(%s): (%s){}'):format(k, k)
          end

          if ctx ~= '' then
            local kdef = ('#define k_T(u) _Generic(u%s)'):format(ctx)

            file_fp:write(kdef)
          end

          file_fp:close()

          target:add('files', sourcefile, { cflags = ('-include %s'):format(file) })
        end
      end
    end
  end)
end)
--]]

add_requires('libsock')
target('dev.c', function()
  set_kind('binary')
  set_default('false')
  add_files('*.c')
  set_rundir('$(projectdir)')

  -- add_rules('codegen')

  add_deps('u')
  add_packages('libsock')

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
    os.exec('xmake f -m release --mimalloc=y')
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

    print(pd)

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
