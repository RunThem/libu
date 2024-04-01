local file = './cg.c'

local T = {}

-- 打开描述符
local fp = io.open(file, 'r')
if fp == nil then
  error('打开描述符错误')
end

local ctx = fp:read('a')

-- local ctx = 'uvec(uvec(int)) v = nullptr;'
local idx
idx = 1

-- @class Type
local Type = { K = '', T = '' }
Type.__index = Type

function Type:new(K, T)
  local t = {}
  setmetatable(t, self)
  t.K = K
  t.T = T or ''

  return t
end

local V = {}
local M = {}
local T = {}
local L = {}

local fn = {}

fn['uvec'] = function(idx)
  local s, e = nil, idx

  s, e = ctx:find('%(', e + 1)
  -- stylua: ignore
  if s == nil then return '', 0 end

  s, e = ctx:find('[%a%d_]+', e + 1)
  -- stylua: ignore
  if s == nil then return '', 0 end

  local type = ctx:sub(s, e)

  if type == 'struct' or type == 'enum' or type == 'union' then
    s, e = ctx:find('[%a%d_]+', e + 1)
    -- stylua: ignore
    if s == nil then return '', 0 end

    type = type .. ' ' .. ctx:sub(s, e)
  elseif type == 'uvec' or type == 'umap' or type == 'uavl' or type == 'ulst' then
    type, e = fn[type](e)
  end

  s, e = ctx:find('%)', e + 1)
  -- stylua: ignore
  if s == nil then return '', 0 end

  local vec = 'uvec(' .. type .. ')'
  if V[vec] == nil then
    V[vec] = Type:new('ssize_t', type)
  end

  return vec, e
end

while true do
  local ok = nil

  ok, idx = ctx:find('[^%w_]-uvec[^%w_]-', idx)
  if ok ~= nil then
    _, idx = fn['uvec'](idx)
  else
    break
  end
end

local code = ''
for k, v in pairs(V) do
  code = code .. (', %s: (%s){}'):format(k, v.T)
end

code = ('#define uT(u) _Generic(u%s)'):format(code)

print(code)

--[[
local function get_type(str, line)
  local n = 0
  local s, e = line:find('[^%a%d_]' .. str .. '%s*%(', 1, false)
  if s == nil then
    return 0, 0
  end

  s = e
  while true do
    local ch = line:sub(e, e)
    e = e + 1

    if ch == '(' then
      n = n + 1
    elseif ch == ')' then
      n = n - 1

      if n == 0 then
        break
      end
    end
  end

  return s + 1, e - 2
end

-- 遍历所有行
for c in fp:lines('l') do
  -- 先假定一行仅有一个匹配结果

  -- vec
  local s, e = get_type('uvec', ' ' .. c .. ' ')
  if e ~= 0 then
    local t = c:sub(s, e)
    if T[t] == nil then
      T[t] = t
    end
  end

  -- map
  s, e = get_type('umap', ' ' .. c .. ' ')
  if e ~= 0 then
    local t = c:sub(s, e)
    if T[t] == nil then
      T[t] = t
    end
  end
end

local code = ''
for _, v in pairs(T) do
  code = code .. (', uvec(%s): (%s){}'):format(v, v)
end

code = ('#define uT(u) _Generic(u%s)'):format(code)

print(code)


--]]
