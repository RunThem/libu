local file = './cg.c'

-- 打开描述符
local fp = io.open(file, 'r')
if fp == nil then
  error('打开描述符错误')
end

local ctx = fp:read('a')

-- local ctx = 'uvec(uvec(int)) v = nullptr;'
local idx
idx = 1

---[[
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

-- stylua: ignore start
fn['uvec'] = function(idx)
  local s, e = nil, idx

  s, e = ctx:find('%(', e + 1)
  if s == nil then return '', 0 end

  s, e = ctx:find('[%a%d_]+', e + 1)
  if s == nil then return '', 0 end

  local type = ctx:sub(s, e)

  if type == 'struct' or type == 'enum' or type == 'union' then
    s, e = ctx:find('[%a%d_]+', e + 1)
    if s == nil then return '', 0 end

    type = type .. ' ' .. ctx:sub(s, e)
  elseif type == 'uvec' or type == 'umap' or type == 'uavl' or type == 'ulst' then
    type, e = fn[type](e)
  end

  s, e = ctx:find('%)', e + 1)
  if s == nil then return '', 0 end

  local vec = 'uvec(' .. type .. ')'
  if V[vec] == nil then
    V[vec] = Type:new('ssize_t', type)
  end

  return vec, e
end
-- stylua: ignore end

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
--]]
