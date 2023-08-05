--- Project
set_project('libu')

--- xmake configure
set_xmakever('2.6.1')
add_plugindirs('.plugins')

--- Project version
set_version('0.2.0')

--- Build mode
add_rules('mode.debug', 'mode.release')

--- Macro definition
add_defines('_GNU_SOURCE=1')

--- No warning, no error
set_warnings('all', 'error')

--- Language standard
set_languages('clatest')

--- Unused variables and functions
add_cflags('-Wno-unused-function', '-Wno-unused-variable')

--- Lambda expressions option
option('lambda', function()
  set_default(false)
  set_category('option')
  set_description('Enable or disable "clang lambda expression"')
end)

if has_config('lambda') then
  add_defines('USE_LAMBDA')
  add_cflags('-fblocks')
  if is_plat('linux') then
    add_ldflags('-lBlocksRuntime')
    add_defines('__linux__=1')
  end
end

--- Mimalloc option
option('mimalloc', function()
  set_default(true)
  set_category('option')
  set_description('Enable or disable "mimalloc"')
end)

if has_config('mimalloc') then
  add_requires('mimalloc')
  add_defines('USE_MIMALLOC')
end

--- Project common header file path
add_includedirs('$(projectdir)/inc')

--- Main target
target('u', function()
  set_kind('static')
  add_files('$(projectdir)/src/*.c')
  add_headerfiles('$(projectdir)/inc/*.h', { prefixdir = 'u' })

  if has_config('mimalloc') then
    add_packages('mimalloc')
  end
end)

--- Demo target
target('demo', function()
  set_kind('binary')
  add_files('main.c')
  set_default('false')

  add_deps('u')

  if has_config('mimalloc') then
    add_packages('mimalloc')
  end
end)
