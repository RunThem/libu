--!A cross-platform build utility based on Lua
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--
-- Copyright (C) 2015-present, TBOOX Open Source Group.
--
-- @author      ruki
-- @file        main.lua
--

-- imports
import('core.base.option')
import('core.project.config')
import('core.project.project')
import('lib.detect.find_tool')
import('private.action.require.impl.packagenv')
import('private.action.require.impl.install_packages')

-- main
function main()
  -- load configuration
  config.load()

  clang_format = find_tool('clang-format', { force = true })
  assert(clang_format, 'clang-format not found!')

  -- create style file
  local argv = {}
  local projectdir = project.directory()

  -- inplace flag
  table.insert(argv, '-i')

  local target = project.target('fmt')
  assert(target, [[target 'fmt' not found!]])

  -- source files
  for _, source in ipairs(target:sourcefiles()) do
    table.insert(argv, path.join(projectdir, source))
  end

  -- header files
  for _, header in ipairs(target:headerfiles()) do
    table.insert(argv, path.join(projectdir, header))
  end

  print(argv)

  -- format files
  os.vrunv(clang_format.program, argv, { curdir = projectdir })
  cprint('${color.success}format ok!')

  -- done
  os.setenvs(oldenvs)
end
