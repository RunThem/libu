--- Project
set_project('libu')

--- Project version
set_version('0.4.0')

--- xmake configure
set_xmakever('2.6.1')

--- Build mode
add_rules('mode.debug', 'mode.valgrind', 'mode.release')

--- Macro definition
add_defines('_GNU_SOURCE=1')

--- No warning, no error
set_warnings('all', 'error')

--- Language standard
set_languages('clatest', 'cxxlatest')

--- Unused variables and functions
add_cflags('-Wno-unused-function', '-Wno-unused-variable', '-Wno-unused-but-set-variable')

--- DWARF v4
add_cflags('-gdwarf-4')

--- Toolchain
set_toolchains('clang')

--- Task(lsp) generate the project file
task('lsp', function()
  set_category('plugin')

  on_run(function()
    os.exec('xmake project -k cmake build/lsp')
    os.exec('xmake project -k compile_commands build/lsp')
  end)

  set_menu({
    usage = 'xmake lsp',
    description = 'Generate the project file.',
  })
end)

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
  add_files('$(projectdir)/src/**.c')
  add_headerfiles('$(projectdir)/inc/**.h', { prefixdir = 'u' })

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

task('demo', function()
  set_category('plugin')

  on_run(function()
    os.exec('xmake f -m debug --mimalloc=y')
    os.exec('xmake b demo')
    os.exec('xmake r demo')
  end)

  set_menu({
    usage = 'xmake demo',
    description = 'Run "demo" target.',
  })
end)

task('mem', function()
  set_category('plugin')

  on_run(function()
    os.exec('xmake f -m valgrind --mimalloc=n')
    os.exec('xmake b demo')
    os.exec('valgrind --tool=memcheck --leak-check=full ./build/linux/x86_64/valgrind/demo')
  end)

  set_menu({
    usage = 'xmake mem',
    description = 'Use Valgrind to test for memory leaks.',
  })
end)
