-- reference version of nova reconstructor
-- Marcel van Herk 21 Jab 2025
-- start with go 2000
-- configurable variables shown below

dofile ('c:\\software\\nova1200-restoration\\lua_assembler\\telnet_nova.lua')

config = {
  projs=90,
  piprojs=3.1416/90,
  pscale=256/80,
  sinogram = 0xf200
}

t = assemble(octal(2000), [[
  MACRO ASR MOV %1 %1 SL NOLOAD SZC|MOV %1 %1 SR CO SKP|MOV %1 %1 SR CZ
  MACRO ASL MOV %1 %1 SL CZ
  MACRO SET0 LDA 0 constant(%2)|STA 0 variable.%1
 
  SET0 projections config.projs
  SET0 curproj 0
  
  // clear final image and all projection arrays including guard words arround them for fast processing
  LDA 1 constant(labels.image)
  LDA 2 constant(0)
  SET0 ARG labels.endofimage-labels.image
  JSR ASETC

LABEL loopprojs
  SET0 imagepointer labels.image
  
  LDA 0 variable.curproj
  LDA 2 string('processing projection %0\n')
  MESSAGE
   
  // load 32 bit offset
  LDA 3 constant(labels.offsets)
  LDA 0 variable.curproj
  ADD 0 0
  ADD 0 3
  LDA 0 AC3
  LDA 1 AC3 1
  STA 0 variable.curroffset
  STA 1 variable.curroffset1

  // load 32 bit stepx
  LDA 3 constant(labels.stepx)
  LDA 0 variable.curproj
  ADD 0 0
  ADD 0 3
  LDA 0 AC3
  LDA 1 AC3 1
  STA 0 variable.currstepx
  STA 1 variable.currstepx1
    
  // load 32 bit stepy
  LDA 3 constant(labels.stepy)
  LDA 0 variable.curproj
  ADD 0 0
  ADD 0 3
  LDA 0 AC3
  LDA 1 AC3 1
  STA 0 variable.currstepy
  STA 1 variable.currstepy1
    
  // read sinogram from SD, eeprom or flash; two 64 words per sinogram; skip so as to traverse all 180 projections one per degree
  LDA 0 variable.curproj
  REPEAT math.log2(180/config.projs) ADD 0 0
  ADD 0 0
  LDA 1 constant(config.sinogram)
  ADD 1 0
  LDA 2 constant(labels.projection)
  READBLOCK
  READBLOCK
    
  // apply ramp and blur filter (twice) to input data
  //SET0 ARG 128
  //JSR lemayfilter
  
  SET0 ARG 128
  LDA 2 constant(labels.projection)
  LDA 1 constant(labels.projection2)
  JSR rampfilter
  SET0 ARG 128
  LDA 2 constant(labels.projection2)
  LDA 1 constant(labels.projection3)
  JSR blurfilter
  SET0 ARG 128
  LDA 2 constant(labels.projection3)
  LDA 1 constant(labels.projection2)
  JSR blurfilter
 
  // pre-interpolate filtered data
  SET0 ARG 128
  LDA 2 constant(labels.projection2)
  LDA 1 constant(labels.projectionh)
  JSR interpolate
  
  SET0 rows 80

  // led is on during backprojection
  LDA 0 constant(1)
  WRITELED
LABEL looprows
  
  // stretch projection data
  SET0 ARG labels.projectionh
  LDA 0 variable.currstepx
  LDA 1 variable.currstepx1
  STA 0 variable.dx
  STA 1 variable.dx1
  SET0 counter 80
  LDA 0 variable.curroffset
  LDA 1 variable.curroffset1
  LDA 2 constant(labels.projection4)
  JSR GSTRETCH
  
  // add to image row
  SET0 ARG 80
  LDA 2 constant(labels.projection4)
  LDA 1 variable.imagepointer
  JSR AADDA
  STA 1 variable.imagepointer

  // add stepy to offset
  LDA 0 variable.curroffset
  LDA 1 variable.curroffset1
  LDA 2 variable.currstepy
  LDA 3 variable.currstepy1
  ADD 2 0 CZ SZC
  INC 1 1
  ADD 3 1
  STA 0 variable.curroffset
  STA 1 variable.curroffset1
  
  DSZ variable.rows
  JMP looprows
  
  // turn off led
  LDA 0 constant(0)
  WRITELED
  
  // display every 16th result
  LDA 0 variable.curproj
  LDA 1 constant(15)
  AND 0 1 SNR
  JSR DISPLAY
    
  ISZ variable.curproj
  DSZ variable.projections
  JMP loopprojs
  
  // display end result
  JSR DISPLAY
  
  HALT
    
//------------------------
LABEL DISPLAY
  // display 80x80 image on TFT with autorange
  STA 3 variable.display_ret
  LDA 3 constant(labels.image)
  LDA 2 constant(0)
  LDA 1 constant(-6400)
  
  // find maximum value of positive numbers in AC2
LABEL display_loopmax
  LDA 0 AC3
  INC 3 3
  MOV 0 0 NOLOAD SL CZ SZC
  SUB 0 0
  SUB 0 2 NOLOAD CZ SNC
  MOV 0 2
  INC 1 1 SZR
  JMP display_loopmax
  
  // set display range to maximum value and configure display; zoom=3, address=0 range = max/256+1
  REPEAT 8 ASR 2 2 
  REPEAT 6 ASL 2 2 
  LDA 0 constant(octal(0106))
  ADD 2 0
  LDA 1 constant(320*0)
  GRMODE
  
  LDA 3 constant(labels.image)
  STA 3 variable.pointer
  
  LDA 0 constant(80)
  STA 0 variable.rows
LABEL display_start
  LDA 0 variable.pointer
  LDA 1 constant(80)
  GRDISPLAY
  STA 0 variable.pointer
  
  GRNEXT
  DSZ variable.rows
  JMP display_start
  JMP variable.display_ret IND
  //-------------------
  
  //-------------------
  // interpolate factor 2 up
  //AC2 input data pointer, provide zero guard word after
  //AC1 output data pointer
  //ARG input count
LABEL interpolate
  STA 3 variable.AUTO
  MOV 1 3
LABEL interpolate2
  LDA 0 AC2
  STA 0 AC3
  INC 3 3
  LDA 1 AC2 1
  ADD 0 1
  ASR 1 1
  STA 1 AC3
  INC 3 3
  INC 2 2
  DSZ variable.ARG
  JMP interpolate2
  JMP variable.AUTO IND
  //-------------------
  
  //-------------------
  // ramp filter kernel -1 2 -1
  //AC2 input data pointer; provide zero guard word before and after
  //AC1 output data pointer
  //ARG count
LABEL rampfilter
  STA 3 variable.AUTO
  MOV 1 3
LABEL rampfilter2
  LDA 0 1 AC2
  LDA 1 -1 AC2
  ADD 0 1
  LDA 0 AC2
  ASL 0 0
  SUB 1 0
  STA 0 AC3
  INC 3 3
  INC 2 2
  DSZ variable.ARG
  JMP rampfilter2
  MOV 3 1
  JMP variable.AUTO IND
  
  //-------------------
  // blur filter kernel 0.5 1 0.5
  //AC2 input data pointer; provide zero guard word before and after
  //AC1 output data pointer
  //ARG count
LABEL blurfilter
  STA 3 variable.AUTO
  MOV 1 3
LABEL blurfilter2
  LDA 0 1 AC2
  LDA 1 -1 AC2
  ADD 0 1
  ASR 1 1
  LDA 0 AC2
  ADD 1 0
  STA 0 AC3
  INC 3 3
  INC 2 2
  DSZ variable.ARG
  JMP blurfilter2
  MOV 3 1
  JMP variable.AUTO IND
  //-------------------
  
  //-------------------
  //Add short array to short array
  //AC2 source data pointer
  //AC1 destination data pointer
  //ARG count
LABEL AADDA
  STA 3 variable.AUTO
  MOV 1 3
LABEL AADDA2
  LDA 0 AC2
  LDA 1 AC3
  ADD 0 1
  STA 1 AC3
  INC 2 2
  INC 3 3
  DSZ variable.ARG
  JMP AADDA2
  MOV 3 1
  JMP variable.AUTO IND
  //-------------------
       
  //-------------------
  //Set short array to constant value
  //AC2 constant
  //AC1 destination data pointer
  //ARG count
LABEL ASETC
  STA 3 variable.AUTO
  MOV 1 3
LABEL ASETC2
  STA 2 AC3
  INC 3 3
  DSZ variable.ARG
  JMP ASETC2
  MOV 3 1
  JMP variable.AUTO IND
  //-------------------
  
  //-------------------
  // Stretch data, array ARG must be guarded with many zeros for too low or too high pixel indices, e.g. 128 256 128 allocation
  // AC2[i] = ARG[j], where j= I*scale+offset
  // 0 subpixel address, increases with dx
  // 1 pixel address, increased with dx1  
  // 2 is destination pointer
  // counter is number of points (destroyed)
LABEL GSTRETCH
  STA 3  variable.AUTO
LABEL GSTRETCH1
  LDA 3 variable.ARG
  ADD 1 3   // reg1 contains offset address
  LDA 3 AC3 // copy data
  STA 3 AC2
  INC 2 2
  LDA 3 variable.dx // step subpixel source address
  ADD 3 0 CZ SZC
  INC 1 1
  LDA 3 variable.dx1
  ADD 3 1
  DSZ variable.counter
  JMP GSTRETCH1
  JMP variable.AUTO IND
  //-------------------

LABEL GSTRETCHADD
  STA 3 variable.AUTO
LABEL GSTRETCHADD1
  LDA 3 variable.ARG
  ADD 1 3   // reg1 contains offset address
  LDA 3 AC3 // copy data
  LDA 2 variable.pointer
  LDA 2 AC2
  ADD 2 3
  LDA 2 variable.pointer
  STA 3 AC2
  LDA 3 variable.dx // step subpixel source address
  LDA 2 variable.dx1
  ADD 3 0 CZ SZC
  INC 1 1
  ADD 3 2
  ISZ variable.pointer
  DSZ variable.counter
  JMP GSTRETCHADD1
  JMP variable.AUTO IND

// 80x80 image output; input is scaled to 256 words -> center at 128; all values scaled by 65536 as 32 bit fixed point floats
LABEL offsets
  REPEAT config.projs DD math.round(65536*(-40*config.pscale*math.cos((pc-labels.offsets)*config.piprojs/2)-40*config.pscale*math.sin((pc-labels.offsets)*config.piprojs/2)+128))
LABEL stepx
  REPEAT config.projs DD math.round(65536*(1.0*config.pscale*math.cos((pc-labels.stepx)*config.piprojs/2)))
LABEL stepy
  REPEAT config.projs DD math.round(65536*(1.0*config.pscale*math.sin((pc-labels.stepy)*config.piprojs/2)))

LABEL endofcode

ORG octal(0400)
  DW 0
  LABEL projection
  ORG pc+128
  DW 0

ORG octal(04000)
  LABEL image
  ORG pc+6400

  ORG pc+128
LABEL projectionh
  ORG pc+256
  ORG pc+128
  
  ORG pc+10
LABEL projection2
  ORG pc+128
  ORG pc+10
  
  ORG pc+10
LABEL projection3
  ORG pc+128
  ORG pc+10
   
  ORG pc+10
LABEL projection4
  ORG pc+128
  ORG pc+10
  LABEL endofimage
  
  ORG labels.endofcode
  
]], false) -- false omits go command

map()

writeport(32, t, 100)
--writetelnet("192.168.1.234", t, 100)

