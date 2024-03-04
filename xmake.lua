--- Project
set_project('libu')

--- Project version
set_version('2.0.0')

--- xmake configure
set_xmakever('2.6.1')

--- Build mode
add_rules('mode.debug', 'mode.valgrind', 'mode.profile', 'mode.release')

--- Macro definition
add_defines('_GNU_SOURCE=1')

--- No warning, no error
set_warnings('all', 'error')

--- Language standard
set_languages('clatest', 'cxxlatest')

--- Unused variables and functions
add_cflags(
  '-Wno-unused-label',
  '-Wno-unused-function',
  '-Wno-unused-variable',
  '-Wno-unused-but-set-variable',
  '-Wno-address-of-packed-member'
)

--- Use reserved identifier
add_cflags('-Wno-reserved-macro-identifier', '-Wno-reserved-identifier')

--- Disable VLA extensons
add_cflags('-Werror=vla')

--- DWARF v4
add_cflags('-gdwarf-4')

--- Toolchain
set_toolchains('clang')

--- Repositories
add_repositories('RunThem https://github.com/RunThem/My-xmake-repo')

--- Task(lsp) generate the project file
task('lsp', function()
  set_category('plugin')
  set_menu({
    usage = 'xmake lsp',
    description = 'Generate the project file.',
  })

  on_run(function()
    os.exec('xmake project -k cmake build')
    os.exec('xmake project -k compile_commands build')
  end)
end)

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

--- Third party library.
add_requires('tbox')

--- Main target
target('u', function()
  set_kind('static')
  add_files('$(projectdir)/src/**.c')
  add_headerfiles('$(projectdir)/inc/(**.h)')

  add_packages('tbox', { public = true })

  if has_config('mimalloc') then
    add_packages('mimalloc', { public = true })
  end
end)

--- Dev mode
includes('dev')
