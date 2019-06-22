dofile ('c:\\software\\nova1200-restoration\\lua_assembler\\telnet_nova.lua')

assemble(octal(1200), false, [[
// addresses imported from nova_testprogram.lua
ORG octal(50)
LABEL INDMULT
  ORG pc+1
LABEL INDSMULT
  ORG pc+1
LABEL _wiener
  ORG pc+1
LABEL _offsets
  ORG pc+1
LABEL _stepx
  ORG pc+1
LABEL _stepy
  ORG pc+1
LABEL _FFTSIZE
  ORG pc+1
LABEL _NBUTTERFLIES
  ORG pc+1
LABEL _SWAPS
  ORG pc+1
LABEL _BUTTERFLIES
  ORG pc+1
LABEL _input
  ORG pc+1
LABEL _slice
  ORG pc+1


ORG octal(1200)
LABEL reconstruct
  STA 3 ret_reconstruct
  
  // initialize counters
  LDA 3 value100
  STA 3 imcount
  LDA 3 _stepx
  STA 3 curstepx

// loop over input data
LABEL loopims
  // 80 colums to process
  LDA 3 value80
  STA 3 colcount
  
  // to loop over all pixels
  LDA 3 _slice
  STA 3 curpixel
  
// get offset and stepx from table, update table pointer
  LDA 3 curstepx
  LDA 0 -100 AC3  // AC0: offset
  LDA 1 0 AC3     // AC1: deltax
  INC 3 3
  STA 3 curstepx
  STA 0 curoffset

// loop over columns
LABEL loopcols
  // 80 rows to process
  LDA 3 value80
  STA 3 rowcount

// loop over rows
LABEL looprows
  // need to find input data: address = input + (offset[im] + col*deltay + row*deltay)>>8
  MOV 0 2 SS     // swap bytes
  LDA 3 val255   // keep low byte
  AND 3 2
  LDA 3 _input   // add to input address
  ADD 3 2
  LDA 3 AC2      // get selected pixel
  LDA 2 curpixel IND // add to slice
  ADD 3 2
  STA 2 curpixel IND
  ISZ curpixel // never 0
  
  ADD 1 0      // next pixel

  DSZ rowcount
  JMP looprows
  
  // update curoffset make AC0 ready for next row
  LDA 0 curoffset
  LDA 3 curstepx
  ADD 3 0
  STA 0 curoffset
  
  DSZ colcount
  JMP loopcols
  
  INC 3 3
  LDA 0 imcount // show other values here to debug
  LDA 2 _message
  WRITELED
  MESSAGE

  ISZ curstepx // never 0
  DSZ imcount
  JMP loopims
  HALT

LABEL _message
  DW message
LABEL message
  STRING 'image=%0_'
LABEL ret_reconstruct
  DW 0
LABEL rowcount
  DW 0
LABEL colcount
  DW 0
LABEL imcount
  DW 0
LABEL value80
  DW 80
LABEL value100
  DW 100
LABEL val255
  DW 255
LABEL curstepx
  DW 0
LABEL curpixel
  DW 0
LABEL curoffset
  DW 0

]])
Sleep(100)
