--- 每个 .c 一个 target, 用 xmake 自带的 add_tests 挂上, 所以 `xmake test` 能直接跑
---
--- 文件名即用例名: vec / tree / dict / arr / lock
for _, file in ipairs(os.files('*.c')) do
  local name = path.basename(file)

  target('test_' .. name, function()
    set_kind('binary')
    set_default(false)
    add_files(file)
    set_rundir('$(projectdir)')

    --- 公共前导: 断言 (即使 -DNDEBUG 也保留), tbox, u/u.h, 共用的比较/哈希函数
    add_cflags('-include $(projectdir)/test/pub.h', { force = true })

    add_deps('u')

    add_packages('mimalloc', 'tbox')

    add_tests('default')
  end)
end
