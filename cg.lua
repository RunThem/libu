#!/usr/bin/env lua

local types =
  { 'char', 'bool', 'int8_t', 'uint8_t', 'int16_t', 'uint16_t', 'int32_t', 'uint32_t', 'int64_t', 'uint64_t' }

local def = '#define uv_T(u) _Generic(u, %s)'

local str = ''
for _, t in ipairs(types) do
  str = str .. string.format('uvec(%s): (%s){}, ', t, t)
end

print(string.format(def, str))
