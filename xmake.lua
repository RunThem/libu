--- xmake configure
set_xmakever('2.6.1')
add_plugindirs('.plugins')

--- Project name
set_project('libu')

--- Project version
set_version('0.0.1')

--- Build mode
add_rules('mode.debug', 'mode.release')

--- Macro definition
add_defines('_GNU_SOURCE=1')

--- No warning
set_warnings('all', 'error')

--- Build C flags
add_cflags('-std=gnu2x')

--- Unused variables and functions
add_cflags('-Wno-unused-function', '-Wno-unused-variable')

--- Lambda expressions
local lambda = false
if lambda then
  add_cflags('-fblocks')
  if is_plat('linux') then
    add_ldflags('-lBlocksRuntime')
    add_defines('__linux__=1')
  end
end

--- Private repositories
add_requires('mimalloc')

--- Project common header file path
add_includedirs('$(projectdir)/inc')

--- Main target
target('u', function()
  set_kind('static')
  add_files('$(projectdir)/src/*.c')
  add_headerfiles('$(projectdir)/inc/*.h', { prefixdir = 'u' })

  add_packages('mimalloc')
end)

--- Demo target
target('demo', function()
  set_kind('binary')
  add_files('main.c')
  set_default('false')

  add_deps('u')
  add_packages('mimalloc')
end)
