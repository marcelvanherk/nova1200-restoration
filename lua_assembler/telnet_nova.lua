-- Assemble and upload programme into Nova; mvh 20190610
-- Added hex output in case upload is unavailable
-- Fixed relative jump, allow label in DW
-- 20190515 Fix AC2 and AC3 relative addressing
-- 20190516 Added rudimentary linker

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

COM = 0100000, NEG = 0100400, MOV = 0101000, INC = 0101400, -- arithmatic
ADC = 0102000, SUB = 0102400, ADD = 0103000, AND = 0103400, -- arithmatic
CZ = 020, CO = 040, CC = 060, -- carry for arithmatic
SL = 0100, SR = 0200, SS = 0300, -- shift for arithmatic 
SKP = 1, SZC = 2, SNC = 3, SZR = 4, SNR = 5, SEZ = 6, SBN = 7, -- skip for arithmatic
NOLOAD = 010, -- noload for arithmatic

HALT = 063077, PUTC = 077277, MESSAGE = 073177, -- single opcode
WRITELED = 073277,
}

-- above table values are OCTAL
for k,v in pairs(kws) do
  kws[k]=octal(v)
end

-- to store labels during assembly
local labels = {}

-- make addressing absolute or pc-relative if needed
function pcrelative(target, pc)
  if target<256 then return target end
  if target>=pc then 
    if target-pc>127 then error('offset too large') end
    return octal(0400)+(target-pc) 
  end
  if target<pc then 
    if target-pc<-128 then error('offset too large') end
    return octal(0400)+bitand(target-pc+256, 255) 
  end
end

-- store word in nova
function deposit(a, v, mem, upload)
  if upload==false then
    mem[a]=v
    return
  end
  conn:send(string.format(":01%04x%04x\r", a, v))
  Sleep(delay)
  var1 = conn:receive('*l')
  Sleep(delay)
end

-- assemble code string and store in Nova
function assemble(start, upl, code)
  local delay=1
  local upload=upl or false
  local nova_memory={}

  local socket = require("socket")
  conn = socket.tcp() 
  if upload==false or conn:connect("192.168.1.105",3002)==nil then
    upload = false
  else
    conn:settimeout(1)
    Sleep(delay)
    var1 = conn:receive('*a')
    if var1=="Port already in use\n\r" then
      print(var1)
      upload=false
    end
  end

  code = split(code, '\n')
  local address = start
  
  -- preprocess REPEAT statement
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
      for i=1, kw[2] do table.insert(code, k+i-1, table.concat(kw, ' ', 3)) end
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
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal})
      address = bitand(chunk()+65536, 65535)
    
    elseif kw[1]=='LABEL' then 
      labels[kw[2]]=address
        
    elseif kw[1]=='STRING' then 
      local chunk = loadstring('return '..kw[2])
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal})
      w = chunk()
      address = address + math.ceil((#w+1)/2) - 1
    end
  end
  
  -- loop statements
  address = start
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
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal})
      address = bitand(chunk()+65536, 65535)
    
    elseif kw[1]=='LABEL' then 
      labels[kw[2]]=address
    
    elseif kw[1]=='DW' then 
      if labels[kw[2]] then 
        w = labels[kw[2]]
      else
        local chunk = loadstring('return '..kw[2])
        setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal})
        w = bitand(chunk()+65536, 65535)
      end
    
    elseif kw[1]=='STRING' then 
      local chunk = loadstring('return '..kw[2])
      setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal})
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
          setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal})
          w = w + bitand(chunk()+256, 255)
        else
          error('error')
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
          setfenv(chunk, {address=pc, pc=pc, math=math, labels=labels, octal=octal})
          w = w + bitand(chunk()+256, 255)
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
    
    elseif kws[kw[1]] then
      w = kws[kw[1]]
    
    else
      error(v)
    end
    
    -- no code
    if w=='' then 
      print(tooctal(pc), '', v)
    
    -- one word code
    elseif type(w)=='number' then
      print(tooctal(pc), tooctal(tonumber(w)), v)
      deposit(pc, w, nova_memory, upload)
    
    -- multiple words code
    elseif type(w)=='string' then
      address = address-1
      for k=1, math.floor(#w/2) do
        local n = 256*string.byte(w, k*2-1)+string.byte(w, k*2)
        print(tooctal(pc), tooctal(n), v)
        deposit(pc, n, nova_memory, upload)
        address = address+1
        pc = address
      end
      if math.fmod(#w,2)~=0 then
        local n = string.byte(w, #w)
        print(tooctal(pc), tooctal(n), v)
        deposit(pc, n, nova_memory, upload)
        address = address+1
        pc = address
      end
      if math.fmod(#w,2)==0 then
        print(tooctal(pc), 0, v)
        deposit(pc, 0, nova_memory, upload)
        address = address+1
        pc = address
      end
    end
  end
  
  if upload==true then
    conn:close()
  else
    local mink, maxk=999999,0
    for k,v in pairs(nova_memory) do
      mink = math.min(k, mink)
      maxk = math.max(k, maxk)
    end
    
    print('********************************')
    local i = mink
    while i<=maxk do
      local N = maxk-i+1
      if N>8 then N=8 end
      local s=''
      for j=0, N-1 do s = s .. string.format("%04x", nova_memory[i+j] or 0) end
      for j=0, N-1 do 
        if nova_memory[i+j] then
          print(string.format(":%02x%04x%s", N, i, s))
          break
        end
      end
      i = i+N
    end
    print(string.format("go %o", start))
    print('********************************')
  end
  
  persistence.store('c:\\software\\nova1200-restoration\\lua_assembler\\'..tooctal(start)..'.lua', nova_memory)
  
  local link_memory={}
  local link_binary={}
  for i=0, octal(40) do
    local t=persistence.load('c:\\software\\nova1200-restoration\\lua_assembler\\'..tooctal(i*64)..'.lua')
    for k,v in pairs(t or {}) do
      if link_memory[k] then
        print('overlap in memory location' .. tooctal(k))
      end
      link_memory[k] = v
    end
  end
  link_binary[0]=16384
  for i=1, 16384 do
    link_binary[i] = link_memory[i-1] or 0
  end
  local s = ConvertBinaryData('16385*u2', link_binary)
  f = io.open('c:\\software\\nova1200-restoration\\lua_assembler\\linked.dmp', 'wb')
  f:write(s)
  f:close()
end
