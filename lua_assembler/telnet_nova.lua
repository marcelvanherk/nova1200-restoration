-- Assemble and upload programme into Nova; mvh 20190610
-- Added hex output in case upload is unavailable
-- Fixed relative jump, allow label in DW
-- 20190515 Fix AC2 and AC3 relative addressing
-- 20190516 Added rudimentary linker
-- 20190915 Added IO instructions, make chunk() pc relative
-- 20190918 Added some composite instructions
-- Todo: output linked prorgram in ab format
-- 20241227 Added GRMODE etc; add constant generator eg LDA 0 constant(-1); recoded the hex lister for multiple blocks; 
--          added variable address generator, e.g. STA 0 variable.aap
--          Also copy hex code to clipboard
-- 20250102 Added INCLUDE MACRO autoinc; allow assemble in pieces and outout all or just last section
-- 20250110 Added math.round and INFO
-- 20250114 Added string function as in LDA 2 string("Hallo %0\r\n"); config addresses at top
-- 20250117 Fixed string(); added config table; math.log2; REPEAT N now expects lua expression as N; save tables for debugging
-- 20250119 Added DD to generate doublewords
-- 20250120 Removed immediate upload and disabled linking, map() function including constants
-- 20250215 Added %r to REPEAT gives sequence 1..N


-- memory locations for numbers and string generation
local c_number_place = 0x40       -- max 192 numbers, variables, long references etc
local c_string_place = 16384-1024 -- max 1024 words for strings in 16KW system

config = config or {}

require('persistence')

-- split string into pieces, return as table
function split(str, pat)
   local t = {} 
   local fpat = "(.-)" .. pat
   local last_end = 1
   local s, e, cap = str:find(fpat, 1)
   while s do
      if s ~= 1 or cap ~= "" then
	 table.insert(t,cap)
      end
      last_end = e+1
      s, e, cap = str:find(fpat, last_end)
   end
   if last_end <= #str then
      cap = str:sub(last_end)
      table.insert(t, cap)
   end
   return t
end

function math.round(x)
  return x>=0 and math.floor(x+0.5) or math.ceil(x-0.5)
end

function math.log2(x)
  return math.round(math.log(x)/math.log(2))
end

-- interpret number as octal
function octal(n)
  return tonumber(tostring(n), 8)
end

-- return number as string on octal
function tooctal(n)
  return string.format('%o', n)
end

-- https://stackoverflow.com/questions/32387117/bitwise-and-in-lua
-- avoid negative numbers, e.g. add 65536 to input
function bitand(a, b)
    a = a+65536
    local result = 0
    local bitval = 1
    while a > 0 and b > 0 do
      if a % 2 == 1 and b % 2 == 1 then -- test the rightmost bits
          result = result + bitval      -- set the current bit
      end
      bitval = bitval * 2 -- shift left
      a = math.floor(a/2) -- shift right
      b = math.floor(b/2)
    end
    return result
end

-- list of assembler keywords (incomplete)
local kws=
{LDA = 020000, STA = 040000, -- load / store
JMP = 0, JSR = 004000, JMS = 004000, ISZ = 010000, DSZ = 014000, -- jmp, increment
PC = 00400, AC2 = 01000, AC3 = 01400, IND = 02000, -- addressing for load and jmp

NIO = 060000,  DIA = 060400, DOA = 061000, DIB = 061400, DOB=062000, DIC = 062400, DOC = 0630000, --io
IOS = 000100, IOC = 000200, IOP = 000300, -- control part of IO
SKPBN = 063400, SKPBZ = 063500, SKPDN = 063600, SKPDZ = 063700, -- io skip
CPU = 077, TTI = 010, TTO = 011,  -- selected io addresses

NIOS = 060100, NIOC = 060200, NIOP = 060300, -- combined IO instructions e.g. NIO IOS etc
DOAS = 061100, DOAC = 061200, DOAP = 061300, 
DOBS = 062100, DOBC = 062200, DOBP = 062300, 
DOCS = 063100, DOCC = 063200, DOCP = 063300, 
DIAS = 060500, DIAC = 060600, DIAP = 060700, 
DIBS = 061500, DIBC = 061600, DIBP = 061700, 
DICS = 062500, DICC = 062600, DIBP = 062700, 

INTEN = 060177, INTDS = 060277,

COM = 0100000, NEG = 0100400, MOV = 0101000, INC = 0101400, -- arithmatic
ADC = 0102000, SUB = 0102400, ADD = 0103000, AND = 0103400, -- arithmatic
CZ = 020, CO = 040, CC = 060, -- carry for arithmatic
SL = 0100, SR = 0200, SS = 0300, -- shift for arithmatic 
SKP = 1, SZC = 2, SNC = 3, SZR = 4, SNR = 5, SEZ = 6, SBN = 7, -- skip for arithmatic
NOLOAD = 010, -- noload for arithmatic

ADDL = 0103100, ADDR = 0103200, ADDS = 0103300, -- combined

HALT = 063077, PUTC = 077277, MESSAGE = 073177, INFO = 077377, -- single opcode
WRITELED = 073277, GRMODE = 063377, GRDISPLAY = 063277, GRNEXT = 063177, READBLOCK = 077077, WRITEBLOCK = 073377

}

-- above table values are OCTAL
for k,v in pairs(kws) do
  kws[k]=octal(v)
end

-- to store labels during assembly
local labels = {}
labels.c_string_place = c_string_place

-- make addressing absolute or pc-relative if needed
function pcrelative(target, pc)
  if not target then error('unknown target at address: '..pc) end
  if target<256 then return target end
  if target>=pc then 
    if target-pc>127 then 
      --error('offset too large') 
      return octal(2000)+constant(target)
    end
    return octal(0400)+(target-pc) 
  end
  if target<pc then 
    if target-pc<-128 then 
      --error('offset too large') 
      return octal(2000)+constant(target)
    end
    return octal(0400)+bitand(target-pc+256, 255) 
  end
end

-- store word in nova
function deposit(a, v, mem)
  mem[a]=v
end

function to_clipboard(text)
  local p = io.popen("clip.exe", "w")
  p:write(text)
  p:close()
end


-- to store constants during assembly
local constant_place = c_number_place
local constants = {}
local variables = {}
local variable = {}
local autoinc_place = octal(020)
local autoinc = {}
local address = 0
local macros = {}
local strings = {}
local string_place = c_string_place

local nova_memory={}

function constant(value)
  value = bitand(value+65536, 65535)
  if constants[value] then
    return constants[value]
  else
    constants[value] = constant_place
    deposit(constant_place, value, nova_memory, false)
    constant_place = constant_place+1
    if constant_place>255 then error("out of constant/variable space") end
    return constants[value]
  end
end
  
function _string(value)
  if strings[value] then
    return strings[value]
  else
    pc = string_place
    strings[value] = string_place
    local w = value
    for k=1, #w+1, 2 do
      n = 256*(string.byte(value, k) or 0)+(string.byte(value, k+1) or 0)
      deposit(pc, n, nova_memory, false)
      pc = pc+1
    end
    deposit(pc, 0, nova_memory, false)
    pc = pc+1
    string_place = pc
    if string_place>0x4000 then error("out of string space") end
    return strings[value]
  end
end

function _variable(name)
  if rawget(variables,name) then
    return variables[name]
  else
    rawset(variables, name, constant_place)
    deposit(constant_place, 0, nova_memory, false)
    constant_place = constant_place+1
    if constant_place>255 then error("out of constant/variable space") end
    return rawget(variables, name)
  end
end

function _autoinc(name)
  if rawget(autoinc,name) then
    return autoinc[name]
  else
    rawset(autoinc, name, autoinc_place)
    deposit(autoinc_place, 0, nova_memory, false)
    autoinc_place = autoinc_place+1
    if autoinc_place>octal(027) then error("out of autoinc space") end
    return rawget(autoinc, name)
  end
end

setmetatable(variables, {__index = function(t, k) return _variable(k) end })
setmetatable(variable, {__index = function(t, k) return _variable(k) end })
setmetatable(autoinc, {__index = function(t, k) return _autoinc(k) end })

-- assemble code string and store in Nova
function assemble(start, code, go, makehex)
  local delay=1
  if makehex==nil then makehex=true end
  if go==nil then go=true end

  code = split(code, '\n')
  address = start or address
  local saddress = address
  
  -- preprocess REPEAT and INCLUDE statement
  for k,v in ipairs(code) do 
    kw = split(v, ' ')
    local i=1
    while i<=#kw do
      if kw[i]=='' or kw[i]==',' then
        table.remove(kw, i)
      else
        i=i+1
      end
    end
    
    if kw[1]=='REPEAT' then
      table.remove(code, k)
      local chunk = loadstring('return '..kw[2])
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
      w = chunk()
      for i=1, w do table.insert(code, k+i-1, (string.gsub(table.concat(kw, ' ', 3), '%%r',  i))) end
      kw = split(code[k], ' ')
    end
    
    if kw[1]=='INCLUDE' then
      code[k]='//'..table.concat(kw, ' ', 1)
      local i=1
      for l in io.lines(kw[2]) do
        table.insert(code, k+i, l) 
        i=i+1
      end
    end
  
    if kw[1]=='MACRO' then
      code[k]='//'..table.concat(kw, ' ', 1)
      local i=1
      if not macros[kw[2]] then 
        macros[kw[2]] = table.concat(kw, ' ', 3)
      else
        local m = macros[kw[2]]
        for j=1, 4 do m = string.gsub(m, '%%'..j, kw[2+j] or '') end
        for k2, v2 in ipairs(string.split(m, '|')) do
          table.insert(code, k+i, v2) 
          i=i+1
        end
      end
    end
    
    if macros[kw[1]] then
      code[k]='//'..table.concat(kw, ' ', 1)
      local i=1
      local m = macros[kw[1]]
      for j=1, 4 do m = string.gsub(m, '%%'..j, kw[1+j] or '') end
      for k2, v2 in ipairs(string.split(m, '|')) do
        table.insert(code, k+i, v2) 
        i=i+1
      end
    end

  end
  
  -- first pass: get label statements
  for k,v in ipairs(code) do 
    kw = split(v, ' ')
    local i=0
    while i<=#kw do
      if kw[i]=='' or kw[i]==',' then
        table.remove(kw, i)
      else
        i=i+1
      end
    end
    
    kw[1]=kw[1] or '//'
    
    if kw[1]~='LABEL' and kw[1]~='ORG' and string.sub(kw[1], 1, 2)~='//' then 
      address=address+1 
    end
    
    local pc = address
    local w = ''
    if kw[1]=='ORG' then 
      local chunk = loadstring('return '..kw[2])
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
      address = bitand(chunk()+65536, 65535)
    
    elseif kw[1]=='LABEL' then 
      if labels[kw[2]] then error('label "'..kw[2]..'" redefined:  first at address '..labels[kw[2]]) end
      labels[kw[2]]=address
        
    elseif kw[1]=='DD' then 
      address = address+1
    
    elseif kw[1]=='STRING' then 
      local chunk = loadstring('return '..kw[2])
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
      w = chunk()
      address = address + math.ceil((#w+1)/2) - 1
    end
  end
  
  -- loop statements
  address = saddress
  for k,v in ipairs(code) do 
    kw = split(v, ' ')
    local i=0
    while i<=#kw do
      if kw[i]=='' or kw[i]==',' then
        table.remove(kw, i)
      else
        i=i+1
      end
    end
    
    local pc = address
    local w = ''
    
    kw[1]=kw[1] or '//'
    
    if kw[1]~='LABEL' and kw[1]~='ORG' and string.sub(kw[1], 1, 2)~='//' then 
      address=address+1 
    end
    
    if string.sub(kw[1], 1, 2)=='//' then 
      -- do nothing
    elseif kw[1]=='ORG' then 
      local chunk = loadstring('return '..kw[2])
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
      address = bitand(chunk()+65536, 65535)
    
    elseif kw[1]=='LABEL' then 
      labels[kw[2]]=address
    
    elseif kw[1]=='DW' then 
      if labels[kw[2]] then 
        w = labels[kw[2]]
      else
        local chunk = loadstring('return '..kw[2]..(kw[3] or ''))
        setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
        w = bitand(chunk()+65536, 65535)
      end
    
    elseif kw[1]=='DD' then 
      if labels[kw[2]] then 
        w = labels[kw[2]]
      else
        local chunk = loadstring('return '..kw[2]..(kw[3] or ''))
        setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
        w = chunk() 
        if w<0 then w = w + 65536*65536 end
        w = {bitand(w, 65535), bitand(math.floor(w/65536), 65535)} -- processed below
      end
    
    elseif kw[1]=='STRING' then 
      local chunk = loadstring('return '..kw[2])
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
      w = chunk() -- processed below
    
    elseif kw[1]=='LDA' or kw[1]=='STA' then 
      w = kws[kw[1]] + kw[2]*2048
      for j=3, #kw do
        if string.sub(kw[j], 1, 2) == '//' then
          break
        elseif tonumber(kw[j]) then 
          w = w + bitand(tonumber(kw[j]+256), 255)
        elseif labels[kw[j]] then 
          w = w + pcrelative(labels[kw[j]], pc)
        elseif (kws[kw[j]]) then 
          w = w + kws[kw[j]] 
        elseif loadstring('return '..kw[j]) then
          local chunk = loadstring('return '..kw[j])
          setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
          local a = pcrelative(chunk(), pc)
	  if string.match(kw[j], 'string.+') then a=bitand(a, 0xff) end
          w = w + a 
          break
        elseif loadstring('return '..table.concat(kw, ' ', j)) then
          local chunk = loadstring('return '..table.concat(kw, ' ', j))
          setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
	  local a = pcrelative(chunk(), pc)
	  if string.match(kw[j], 'string.+') then a=bitand(a, 0xff) end
          w = w + a
	  break
        else
          error('error '..v)
        end
      end
    
    elseif kw[1]=='JMP' or kw[1]=='JSR' or kw[1]=='JMS' or kw[1]=='ISZ' or kw[1]=='DSZ' then 
      w = kws[kw[1]]
      for j=2, #kw do
        if string.sub(kw[j], 1, 2) == '//' then
          break
        elseif tonumber(kw[j]) then 
          w = w + bitand(tonumber(kw[j]+256), 255)
        elseif labels[kw[j]] then 
          w = w + pcrelative(labels[kw[j]], pc)
        elseif (kws[kw[j]]) then 
          w = w + kws[kw[j]] 
        elseif loadstring('return '..kw[j]) then
          local chunk = loadstring('return '..kw[j])
          setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal, bitand=bitand, constant=constant, variable=variable, variables=variables, autoinc=autoinc, string=_string, config=config})
          w = w + pcrelative(chunk(), pc)
        else
          error('error '..v)
        end
      end
    
    elseif kw[1]=='COM' or kw[1]=='NEG' or kw[1]=='MOV' or kw[1]=='INC' 
        or kw[1]=='ADC' or kw[1]=='SUB' or kw[1]=='ADD' or kw[1]=='AND' then 
      w = kws[kw[1]] + kw[2]*8192 + kw[3]*2048
      for j=4, #kw do
        if string.sub(kw[j], 1, 2) == '//' then
          break
        elseif (kws[kw[j]]) then 
          w = w + kws[kw[j]] 
        else
          error('error '..v)
        end
      end
    
    elseif kw[1]=='DIA' or kw[1]=='DOA' or 
           kw[1]=='DIB' or kw[i]=='DOB' or
           kw[1]=='DIC' or kw[i]=='DOC'
    then 
      w = kws[kw[1]] + kw[2]*8192
      for j=3, #kw do
        if string.sub(kw[j], 1, 2) == '//' then
          break
        elseif (kws[kw[j]]) then 
          w = w + kws[kw[j]] 
        else
          error('error '..v)
        end
      end
    
    elseif kws[kw[1]] then
      w = kws[kw[1]]
      for j=2, #kw do
        if string.sub(kw[j], 1, 2) == '//' then
          break
        elseif (kws[kw[j]]) then 
          w = w + kws[kw[j]] 
        else
          error('error '..v)
        end
      end
    else
      error(v)
    end
    
    -- no code
    if w=='' then 
      print(tooctal(pc), '', v)
    
    -- one word code
    elseif type(w)=='number' then
      print(tooctal(pc), tooctal(tonumber(w)), v)
      deposit(pc, w, nova_memory)
    
    -- two word code
    elseif type(w)=='table' then
      print(tooctal(pc), tooctal(tonumber(w[1])), tooctal(tonumber(w[2])), v)
      deposit(pc, w[1], nova_memory)
      pc = pc+1
      address = pc+1
      deposit(pc, w[2], nova_memory)
    
    -- multiple words code
    elseif type(w)=='string' then
      address = address-1
      for k=1, math.floor(#w/2) do
        local n = 256*string.byte(w, k*2-1)+string.byte(w, k*2)
        print(tooctal(pc), tooctal(n), v)
        deposit(pc, n, nova_memory)
        address = address+1
        pc = address
      end
      if math.fmod(#w,2)~=0 then
        local n = string.byte(w, #w)
        print(tooctal(pc), tooctal(n), v)
        deposit(pc, n, nova_memory)
        address = address+1
        pc = address
      end
      if math.fmod(#w,2)==0 then
        print(tooctal(pc), 0, v)
        deposit(pc, 0, nova_memory)
        address = address+1
        pc = address
      end
    end
  end
  
  local mink, maxk=999999,0
  for k,v in pairs(nova_memory) do
    mink = math.min(k, mink)
    maxk = math.max(k, maxk)
  end
  
  if makehex then
    print('********************************')
    local t = ''
    local i = mink
    while i<=maxk do
      if nova_memory[i] then
        local N = maxk-i+1
        if N>8 then N=8 end
        while not nova_memory[i+N-1] do N=N-1 end
        local s=''
        for j=0, N-1 do s = s .. string.format("%04x", nova_memory[i+j] or 0) end
        if N>0 then print(string.format(":%02x%04x%s", N, i, s)) t = t..string.format(":%02x%04x%s\n", N, i, s) end
        i = i+N
      else
        i=i+1
      end
    end
    print(string.format("go %o", saddress))
    if go then t = t .. string.format("go %o", saddress) end
    nova_memory={}
    print('********************************')
     
    to_clipboard(t)
    print('*** hex code copied to clipboard ***')
    
    -- for debugnova.lua
    persistence.store('c:\\temp\\variables.dat', variables)
    persistence.store('c:\\temp\\labels.dat', labels)
    persistence.store('c:\\temp\\constants.dat', constants)
    persistence.store('c:\\temp\\novamem.dat', nova_memory)
    
    return t
  end
  
  --persistence.store('c:\\software\\nova1200-restoration\\lua_assembler\\'..tooctal(start)..'.lua', nova_memory)
  
--  local link_memory={}
--  local link_binary={}
--  for i=0, octal(40) do
--    local t=persistence.load('c:\\software\\nova1200-restoration\\lua_assembler\\'..tooctal(i*64)..'.lua')
--    for k,v in pairs(t or {}) do
--      if link_memory[k] then
--        print('overlap in memory location' .. tooctal(k))
--      end
--      link_memory[k] = v
--    end
--  end
--  link_binary[0]=16384
--  for i=1, 16384 do
--    link_binary[i] = link_memory[i-1] or 0
--  end
--  local s = ConvertBinaryData('16385*u2', link_binary)
--  f = io.open('c:\\software\\nova1200-restoration\\lua_assembler\\linked.dmp', 'wb')
--  f:write(s)
--  f:close()
end

function writeport(port, str, delay)
  suc = Integer:new()
  if not pcall(function() AVS:SERIAL_OPEN(port) end) then return end
  AVS:SERIAL_PUTBYTE(port, 3, suc)
  AVS:SERIAL_CLOSE(port)
  Sleep(10)

  AVS:SERIAL_OPEN(port)
  local t = string.split(str, '\n')
  for k, v in ipairs(t) do
    AVS:SERIAL_PUTSTRING(port, v)
    print('sent:', v)
    AVS:SERIAL_PUTBYTE(port, 13, suc)
    Sleep(delay or 20)
  end
  AVS:SERIAL_CLOSE(port)
end

function writetelnet(address, str, delay)
  local socket = require("socket")
  conn = socket.tcp()
  if conn:connect(address, 3002)==nil then
    return
  else
    conn:settimeout(1)
    Sleep(delay)
    var1 = conn:receive('*a')
    if var1=="Port already in use\n\r" then
      print(var1)
      return
    end
  end

  local t = string.split(str, '\n')
  for k, v in ipairs(t) do
    conn:send(v..'\r')
    print('sent:', v)
    Sleep(delay or 20)
  end
  conn:close()
end
  
function examine(port, a)
  AVS:SERIAL_OPEN(port)
  v   = 'dump '..string.format('%o', a)..' 1'
  suc = Integer:new()
  s = String:new()
  AVS:SERIAL_PUTSTRING(port, v)
  AVS:SERIAL_PUTBYTE(port, 13, suc)
  AVS:SERIAL_GETSTRING(port, s, 13, 500)
  AVS:SERIAL_GETSTRING(port, s, 13, 500)
  v = string.match(s.value, '%d+ (%d+).+$')
  AVS:SERIAL_CLOSE(port)
  v = tonumber(v or '-1', 8)
  if v>32768 then v = v-65536 end
  return v
end

function debug(port)
  print('Variables:')
  variables = persistence.load('c:\\temp\\variables.dat')
  local u = {} for k, v in pairs(variables) do table.insert(u, {k=k, v=v}) end
  table.sort(u, function(a, b) return a.v<b.v end)
  for k, v in ipairs(u) do 
    e = examine(port, v.v)
    print(v.k .. string.rep(' ', 20-#v.k), string.format('%6o (%d)', v.v, v.v), string.format('%6o (%d)', e, e)) 
  end
  print()
  print('Labels:')
  labels = persistence.load('c:\\temp\\labels.dat')
  local u = {} for k, v in pairs(labels) do table.insert(u, {k=k, v=v}) end
  table.sort(u, function(a, b) return a.v<b.v end)
  for k, v in ipairs(u) do 
    e = examine(port, v.v)
    print(v.k .. string.rep(' ', 20-#v.k), string.format('%6o (%d)', v.v, v.v), string.format('%6o (%d)', e, e)) 
  end
end

function map()
  local total = 0
  local variables = persistence.load('c:\\temp\\variables.dat')
  local u = {} for k, v in pairs(variables) do table.insert(u, {k=k, v=v}) end
  print('Variables:', #u)
  total = total + #u
  table.sort(u, function(a, b) return a.v<b.v end)
  for k, v in ipairs(u) do 
    e = 0
    print(v.k .. string.rep(' ', 20-#v.k), string.format('%6o (%d)', v.v, v.v), string.format('%6o (%d)', e, e)) 
  end
  print()
  local labels = persistence.load('c:\\temp\\labels.dat')
  local u = {} for k, v in pairs(labels) do table.insert(u, {k=k, v=v}) end
  print('Labels:', #u)
  total = total + #u
  table.sort(u, function(a, b) return a.v<b.v end)
  for k, v in ipairs(u) do 
    e = 0
    print(v.k .. string.rep(' ', 20-#v.k), string.format('%6o (%d)', v.v, v.v), string.format('%6o (%d)', e, e)) 
  end
  
  print()
  local constants = persistence.load('c:\\temp\\constants.dat')
  total = total + #u
  local u = {} for k, v in pairs(constants) do table.insert(u, {k=k, v=v}) end
  print('Constants:', #u)
  table.sort(u, function(a, b) return a.v<b.v end)
  for k, v in ipairs(u) do 
    e = 0
    print(v.k .. string.rep(' ', 20-#tostring(v.k)), string.format('%6o (%d)', v.v, v.v), string.format('%6o (%d)', e, e)) 
  end
  
  print()
  print("page zero use (of 192):", total)
end


function makesession()
  --local mem=persistence.load('c:\\temp\\novamem.dat')
  
  mem={}
  mem[5]=1
  mem[6]=2
  mem[7]=3
  mem[8]=3
  mem[9]=3
  mem[10]=3
  mem[11]=3
  mem[12]=3
  mem[13]=3
  mem[20]=5
  mem[21]=6
  
  local prev=65537
  local block={address=0, len=0, data={}}
  
  for i=0, 32767 do
    if not mem[i] then mem[i]=0 end
  end
  
  for i=0, 31 do
    if block.len==0 or #block.data==0 then 
      block.address=i 
      prev = mem[i]
    end
    
    table.insert(block.data, mem[i])
    
    if mem[i]==prev then 
      block.len=block.len+1
    else
      if block.len<2 then 
        block.len=0 
        prev = 65537
      else
        for i=1, #block.data-block.len do print(block.data[i]) end
        print('constant', block.address, block.address+block.len-1, prev)
        block.len=0
        block.data={}
      end
    end
  end
  print('constant', block.address, block.address+block.len-1, prev)
  for i=1, #block.data-block.len do print(block.data[i]) end
end
