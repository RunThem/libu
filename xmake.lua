--- Project
set_project('libu')

--- Project version
set_version('3.0.0')

--- xmake configure
set_xmakever('2.6.1')

--- Build mode
add_rules('mode.debug', 'mode.valgrind', 'mode.profile', 'mode.release')

--- Project file
add_rules('plugin.compile_commands.autoupdate', { outputdir = 'build' })

--- Custom rules
includes('rules/*.lua')

--- Language standard
set_languages('clatest', 'cxxlatest')

--- Macro definition
add_defines('_GNU_SOURCE=1')

--- No warning, no error
set_warnings('all', 'error')

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

--- Repositories
add_repositories('RunThem https://github.com/RunThem/My-xmake-repo')

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

--- libu target
target('u', function()
  set_kind('static')
  add_files('$(projectdir)/src/**.c')
  add_headerfiles('$(projectdir)/inc/(**.h)')

  if has_config('mimalloc') then
    add_packages('mimalloc', { public = true })
  end

  add_rules('generic')

  --[[
  add_defines('LIBU_DEBUG')
  --]]
end)

includes('dev', 'tests')
