--[[
target('cg', function()
  on_load(function(target)
    for _, batch in pairs(target:sourcebatches()) do
      if batch.sourcekind == 'cc' then
        for _, file in ipairs(batch.sourcefiles) do
          local context = io.readfile(file)

          local kstr = ''
          local vstr = ''

          -- kmap<K, V>
          local map_tbl = {}
          for k, v in context:gmatch('kmap%(([%w_ ]+),[ ]*([%w_ ]+)%)') do
            if map_tbl[k .. v] == nil then
              kstr = kstr .. (', kmap(%s, %s): (%s){}'):format(k, v, k)
              vstr = vstr .. (', kmap(%s, %s): (%s){}'):format(k, v, v)
            end

            map_tbl[k .. v] = true
          end

          local kdef = ('#define k_K(u) _Generic(u%s)'):format(kstr)
          local vdef = ('#define k_V(u) _Generic(u%s)'):format(vstr)

          local cg_file = '$(buildir)/codegen/' .. file .. '.h'

          local cg_file_fp = io.open(cg_file, 'w+')
          cg_file_fp:write('#define kmap(K, V) typeof(u_map_t(*)(K, V))\n\n')
          cg_file_fp:write(('%s\n\n%s\n'):format(kdef, vdef))

          target:add('cflags', ('-include %s'):format(cg_file))
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

  add_deps('u')

  add_defines('N1k=1000', 'N1W=10000', 'N10W=100000', 'N100W=1000000', 'N1000W=10000000')

  add_packages('miniz', 'libsock')
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
