rule('generic', function()
  on_config(function(target)
    for _, batch in pairs(target:sourcebatches()) do
      if batch.sourcekind == 'cc' then
        for _, sourcefile in ipairs(batch.sourcefiles) do
          local genericfile = vformat('$(buildir)/generic/' .. sourcefile .. '.h')
          local Generic = { vec = {}, map = {}, set = {}, list = {}, tree = {} }

          ---@class T
          ---@field typ string
          ---@field ktyp string|T
          ---@field vtyp string|T

          ---new class
          ---@param typ string
          ---@return T
          local function new(typ)
            return { typ = typ, ktyp = nil, vtyp = nil }
          end

          ---handle
          ---@param sourcefile string
          local function handle(sourcefile)
            local is_generic = false
            local gentbl = { vec = {}, map = {}, set = {}, list = {}, tree = {} }

            ---@param tbl table<string>
            ---@return T, table<string>
            local function _parse(tbl)
              local typ = table.remove(tbl, 1):trim()

              local m = new(typ)
              if typ == 'vec' or typ == 'set' or typ == 'list' then
                m.ktyp, tbl = _parse(tbl)
              elseif typ == 'map' or typ == 'tree' then
                m.ktyp, tbl = _parse(tbl)
                m.vtyp, tbl = _parse(tbl)
              else
              end

              return m, tbl
            end

            ---@param t T
            ---@return string, string, string
            local function _output(t)
              local txt = ''
              local typ = ''
              local g_typ

              if t.typ == 'vec' then
                local ktxt, ktyp, g_ktyp = _output(t.ktyp)

                typ = ('vec<%s>'):format(g_ktyp)
                g_typ = ('typeof(__u_vec_t(*)(ssize_t, %s))'):format(g_ktyp)
                txt = ('typeof(__u_vec_t(*)(ssize_t, %s)): (%s){}'):format(g_ktyp, g_ktyp)

                gentbl['vec'][typ] = txt
              elseif t.typ == 'set' then
                local ktxt, ktyp, g_ktyp = _output(t.ktyp)

                typ = ('set<%s>'):format(g_ktyp)
                g_typ = ('typeof(__u_set_t(*)(%s))'):format(g_ktyp)
                txt = ('typeof(__u_set_t(*)(%s)): (%s){}'):format(g_ktyp, g_ktyp)

                gentbl['set'][typ] = txt
              elseif t.typ == 'list' then
                local ktxt, ktyp, g_ktyp = _output(t.ktyp)

                typ = ('list<%s>'):format(g_ktyp)
                g_typ = ('typeof(__u_list_t(*)(%s*))'):format(g_ktyp)
                txt = ('typeof(__u_list_t(*)(%s*)): (%s*){}'):format(g_ktyp, g_ktyp)

                gentbl['list'][typ] = txt
              elseif t.typ == 'map' then
                local ktxt, ktyp, g_ktyp = _output(t.ktyp)
                local vtxt, vtyp, g_vtyp = _output(t.vtyp)

                typ = ('map<%s, %s>'):format(g_ktyp, g_vtyp)
                g_typ = ('typeof(__u_map_t(*)(%s, %s))'):format(g_ktyp, g_vtyp)
                txt = ('typeof(__u_map_t(*)(%s, %s)): (struct {%s k; %s v;}){}'):format(g_ktyp, g_vtyp, g_ktyp, g_vtyp)

                gentbl['map'][typ] = txt
              elseif t.typ == 'tree' then
                local ktxt, ktyp, g_ktyp = _output(t.ktyp)
                local vtxt, vtyp, g_vtyp = _output(t.vtyp)

                typ = ('tree<%s, %s>'):format(g_ktyp, g_vtyp)
                g_typ = ('typeof(__u_tree_t(*)(%s, %s))'):format(g_ktyp, g_vtyp)
                txt = ('typeof(__u_tree_t(*)(%s, %s)): (struct {%s k; %s v;}){}'):format(g_ktyp, g_vtyp, g_ktyp, g_vtyp)

                gentbl['tree'][typ] = txt
              else
                txt = t.typ
                typ = t.typ
                g_typ = t.typ
              end

              return txt, typ, g_typ
            end

            local pattern = '#%[%[(.-)%]%]'
            local content = io.readfile(sourcefile)

            for code in string.gmatch(content, pattern) do
              local tbl = code:gsub('%s+', ' '):gsub('[<>,]', '$'):split('$', { plain = true })

              local t, _ = _parse(tbl)
              local _, _, _ = _output(t)

              is_generic = true
            end

            return is_generic, gentbl
          end

          ---generic
          ---@param sourcefile string
          ---@param objectfile string
          local function generic(sourcefile, objectfile)
            local is_generic, gentbl = handle(sourcefile)
            if not is_generic then
              return
            end

            -- include codegen file
            target:add('files', sourcefile, { cflags = ('-include %s'):format(genericfile) })

            -- no changed
            if os.mtime(sourcefile) <= os.mtime(target:objectfile(sourcefile)) and os.exists(genericfile) then
              return
            end

            local out = io.open(genericfile, 'w')

            for _k, _v in pairs(gentbl) do
              local txt = ''
              local is_init = false
              for _, v in pairs(_v) do
                if not is_init then
                  out:write(('#define u_%s_defs \\\n'):format(_k))
                  is_init = true
                end

                txt = ('\t%s,\\\n%s'):format(v, txt)
              end

              if is_init then
                txt = txt:sub(1, txt:len() - 3)
                out:write(txt .. '\n\n')
              end
            end

            out:close()
          end

          generic(sourcefile, target:objectfile(sourcefile))
        end
      end
    end
  end)
end)
