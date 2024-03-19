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

add_requires('miniz', 'libsock')

target('dev.c', function()
  set_kind('binary')
  set_default('false')
  add_files('*.c')
  set_rundir('$(projectdir)')

  -- add_rules('codegen')

  add_deps('u')

  add_defines('N1k=1000', 'N1W=10000', 'N10W=100000', 'N100W=1000000', 'N1000W=10000000')

  add_packages('miniz', 'libsock')
end)

add_requires('fmt')

target('dev.cpp', function()
  set_kind('binary')
  set_default('false')
  add_files('*.cpp')
  set_rundir('$(projectdir)')

  add_defines('N1k=1000', 'N1W=10000', 'N10W=100000', 'N100W=1000000', 'N1000W=10000000')

  add_packages('fmt', { system = false })
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
