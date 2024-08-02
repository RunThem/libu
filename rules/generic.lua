rule('generic', function()
  on_config(function(target)
    for _, batch in pairs(target:sourcebatches()) do
      if batch.sourcekind == 'cc' then
        for _, sourcefile in ipairs(batch.sourcefiles) do
          local genericfile = vformat('$(buildir)/generic/' .. sourcefile .. '.h')
          local Generic = { vec = {}, map = {}, set = {}, list = {}, tree = {} }
          local generic_type_flags = { vec = true, set = true, map = true, list = true, heap = true, tree = true }

          ---@class T
          ---@field typ string
          ---@field arg0 string|T
          ---@field arg1 string|T
          ---@field arg2 string|T

          ---new class
          ---@param typ string
          ---@return T
          local function new(typ)
            return { typ = typ, arg0 = nil, arg1 = nil }
          end

          ---handle
          ---@param sourcefile string
          ---@return table
          local function handle(sourcefile)
            local tbl = { run = false, {}, {} }

            ---@param tbl table<string>
            ---@return T
            local function _parse(tbl)
              local typ = table.remove(tbl, 1):trim()

              local m = new(typ)
              if generic_type_flags[typ] then
                m.arg0 = _parse(tbl)

                if typ == 'map' or typ == 'tree' then
                  m.arg1 = _parse(tbl)
                end
              else
                -- skip base type
              end

              return m
            end

            ---@param t T|string
            ---@return string
            local function _dump(t)
              if t.typ == 'map' or t.typ == 'tree' then
                return string.format('%s<%s, %s>', t.typ, _dump(t.arg0), _dump(t.arg1))
              elseif t.typ == 'vec' or t.typ == 'set' or t.typ == 'list' or t.typ == 'heap' then
                return string.format('%s<%s>', t.typ, _dump(t.arg0))
              end
              return t.typ
            end

            ---@param t T
            ---@return string
            local function _output(t)
              if t == nil then
                return nil
              end

              local txt = ''
              local typ = ''
              local g_typ

              if generic_type_flags[t.typ] then
                local arg0 = _output(t.arg0)
                local arg1 = _output(t.arg1)

                if t.typ == 'map' or t.typ == 'tree' then
                  typ = ('%s<%s, %s>'):format(t.typ, arg0, arg1)
                else
                  typ = ('%s<%s>'):format(t.typ, arg0)
                end

                if t.typ == 'map' or t.typ == 'tree' then
                  txt = ('typeof(__u_%s_ref_t(*)(%s*, %s*)): (%s){}'):format(t.typ, arg0, arg1, arg0)
                  table.insert(tbl[1], txt)

                  txt = ('typeof(__u_%s_ref_t(*)(%s*, %s*)): (%s){}'):format(t.typ, arg0, arg1, arg1)
                  table.insert(tbl[2], txt)
                else
                  txt = ('typeof(__u_%s_ref_t(*)(%s*)): (%s){}'):format(t.typ, arg0, arg0)
                  table.insert(tbl[1], txt)
                end

                g_typ = ('typeof(__u_%s_ref_t(*)(%s*))'):format(t.typ, arg0)
              else
                g_typ = t.typ
              end

              return g_typ
            end

            local pattern = '#%[%[(.-)%]%]'
            local content = io.readfile(sourcefile)

            local all_tbl = {}

            for code in string.gmatch(content, pattern) do
              local types = code:gsub('%s+', ' '):gsub('[<>,]', '$'):split('$', { plain = true })

              local typ = _parse(types)
              local dump = _dump(typ)

              if not all_tbl[dump] then
                _output(typ)
                tbl.run = true
                all_tbl[dump] = true
              end
            end

            return tbl
          end

          ---generic
          ---@param sourcefile string
          ---@param objectfile string
          local function generic(sourcefile, objectfile)
            local tbl = handle(sourcefile)
            if not tbl.run then
              return
            end

            --[[ debug
            print(sourcefile)
            print(tbl)
            --]]

            -- include codegen file
            target:add('files', sourcefile, { cflags = ('-include %s'):format(genericfile) })

            -- no changed
            if os.mtime(sourcefile) <= os.mtime(target:objectfile(sourcefile)) and os.exists(genericfile) then
              return
            end

            local out = io.open(genericfile, 'w')

            for idx, args in ipairs(tbl) do
              if #args ~= 0 then
                local txt = ('#define __u_types_arg%d(self) _Generic(self, %s)\n\n'):format(
                  idx - 1,
                  table.concat(args, ', ')
                )
                out:write(txt)
              end
            end

            out:write('#define u_types(self, i) typeof(__u_types_arg##i(self))\n\n\n\n')

            out:write('#define __u_check_arg1(self) (self(nullptr))\n\n')
            out:write('#define __u_check_arg2(self) (self(nullptr, nullptr))\n\n')
            out:write('#define u_check(self, c, t) static_assert(typeeq(t, __u_check_arg##c(self)))\n\n')
            out:close()
          end

          generic(sourcefile, target:objectfile(sourcefile))
        end
      end
    end
  end)
end)
