set_project('libu')

set_xmakever('2.6.1')

set_version('0.0.1')

add_plugindirs('.plugins')

add_rules('mode.debug', 'mode.release')

add_defines('_GNU_SOURCE=1')

set_warnings('all', 'error')

add_cflags('-std=gnu2x')

add_requires('mimalloc')

add_includedirs('$(projectdir)/src')

target('u', function()
  set_kind('static')
  add_files('src/*.c')
  add_headerfiles('src/*.h', { prefixdir = 'u' })

  add_packages('mimalloc')
end)

target('test', function()
  set_kind('binary')
  add_files('main.c')
  set_default('false')

  add_deps('u')
  add_packages('mimalloc')
end)

target('fmt', function()
  set_kind('phony')
  set_default('false')

  set_pcheader('src/**.h')
end)
