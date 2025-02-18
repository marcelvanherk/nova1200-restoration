-- demo version of nova reconstructor
-- Marcel van Herk 21 Jab 2025
-- start with go 2000
-- configurable variables shown below

-- 20250122 restore levelshift; added sinogram display
-- 20250125 2015 redisplays; 0 1 2 selects input data
-- 20250126 2017 loads image viewfn from SD card; load/save to SD card through blocks ff00 up
-- 20250127 added more input sinogram options 1-7
-- 20250128 added Lemay filter (WIP)
-- 20250214 Use 16% faster GSTRETCHADD with autoinc.target; better rounding (use 128.5)
-- 20250218 Added GSTRETCHADDFAST

dofile ('c:\\software\\nova1200-restoration\\lua_assembler\\telnet_nova.lua')

config = {
-- set with mem
  sinogram = 2,       -- 0 input data, 0 is test centering; 1=f000 2=f200 etc is hardcoded in flash; lower values point to eeprom or SD card alternative
  step = 5,           -- 1 allowed angle steps 1 2 3 5 6 10
  filter = 1,         -- 2 off/on/lemay
  interpolate = 1,    -- 3 if set use pre-interpolation
  calibrate = 0,      -- 4 shift origin of detector array (0 is correct)
  display = 0,        -- 5 0 end result, 1 progressive, 2 backprojection
  level = 10,         -- 6 level/128 of max added to end result
  window = 30,        -- 7 window/128 of max display range
  export = 0,         -- 8 0 don't save, 1 save end result, 2 save progressive; saved to storefn on SD card
  storefn= 0,         -- 9 store filename (max 255)
  viewfn= 0,          -- 10 view filename (max 255)
  
  pscale=256/80,      -- pixel size 3.2 times sinogram pixel size ~1 mm (not configurable without reassembly)
}

t = assemble(octal(2000), [[
  MACRO ASR MOV %1 %1 SL NOLOAD SZC|MOV %1 %1 SR CO SKP|MOV %1 %1 SR CZ
  MACRO ASL MOV %1 %1 SL CZ
  MACRO SET0 LDA 0 constant(%2)|STA 0 variable.%1
  MACRO JZ MOV %1 %1 SNR|JMP %2
  MACRO JNZ MOV %1 %1 SZR|JMP %2
  MACRO LSL MOV 0 0 SL CZ|MOV 1 1 SL
  MACRO LASR MOV 1 1 SL NOLOAD SZC|MOV 1 1 SR CO SKP|MOV 1 1 SR CZ|MOV 0 0 SR
  
  JMP begin
LABEL configblock
  DW config.sinogram
  DW config.step
  DW config.filter
  DW config.interpolate
  DW config.calibrate
  DW config.display
  DW config.level
  DW config.window
  DW config.export
  DW config.storefn
  DW config.viewfn
  DW 0

  // for redisplay from monitor
  JSR DISPLAY
  HALT
 
  // for viewer
  JSR LOADIMAGE
  JSR DISPLAY
  HALT

LABEL begin
  JSR SINOGRAM
  
  SET0 curangle 0
  
  // clear final image and all projection arrays including guard words arround them for fast processing
  LDA 1 constant(labels.image)
  LDA 2 constant(0)
  SET0 ARG labels.endofimage-labels.image
  JSR ASETC

LABEL loopprojs
  LDA 0 variable.curangle
  LDA 2 string('processing angle %0\n')
  MESSAGE
   
  // compute 32 bit offset, add possible miscalibration
  LDA 3 constant(labels.offsets)
  LDA 0 variable.curangle
  ADD 0 3
  LDA 0 AC3
  SUB 1 1
  MOV 0 0 NOLOAD SL CZ SZC
  COM 1 1
  REPEAT 10 LSL
  LDA 2 labels.configblock+4
  ADD 2 1
  STA 0 variable.curroffset
  STA 1 variable.curroffset1

  // compute 32 bit stepx (cos..); uses that sin(90+alpha) = cos(alphs)
  LDA 3 constant(labels.sintable)
  LDA 0 variable.curangle
  ADD 0 3
  LDA 0 constant(90)
  ADD 0 3
  LDA 0 AC3
  SUB 1 1
  MOV 0 0 NOLOAD SL CZ SZC
  COM 1 1
  REPEAT 3 LSL
  STA 0 variable.dx
  STA 1 variable.dx1
    
  // compute 32 bit stepy (sin..)
  LDA 3 constant(labels.sintable)
  LDA 0 variable.curangle
  ADD 0 3
  LDA 0 AC3
  SUB 1 1
  MOV 0 0 NOLOAD SL CZ SZC
  COM 1 1
  REPEAT 3 LSL
  STA 0 variable.currstepy
  STA 1 variable.currstepy1
    
  // read selected sinogram from SD, eeprom or flash; two 64 words per sinogram
  JSR readprojection
    
  // apply filter to input data
  LDA 0 labels.configblock+2
  NEG 0 0
  JZ 0 nofilter
  INC 0 0
  JZ 0 rampf
  
  SET0 ARG 128
  JSR lemayfilter
  SET0 ARG 128
  LDA 2 constant(labels.projection2)
  LDA 1 constant(labels.projection)
  JSR ASETA
  JMP endf
  
LABEL rampf
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
  LDA 1 constant(labels.projection)
  JSR blurfilter
  JMP endf

LABEL nofilter
  // if not scale input data down by 16x
  LDA 1 constant(labels.projection)
  LDA 2 constant(4)
  SET0 ARG 128
  JSR ASHRC
  JMP endf
 
  // optional pre-interpolate filtered data or zoom without interpolation
LABEL endf
  SET0 ARG 128
  LDA 2 constant(labels.projection)
  LDA 1 constant(labels.projectionh)
  LDA 3 labels.configblock+3
  JZ 3 nointerpolate
  JSR interpolate
  JMP interpolatedone
LABEL nointerpolate
  JSR zoom2x
LABEL interpolatedone
  
  // led is on during backprojection
  LDA 0 constant(1)
  WRITELED

  LDA 0 constant(labels.image-1)
  STA 0 autoinc.target 
  SET0 rows 80
LABEL looprows
  
  // stretch projection data
  SET0 ARG labels.projectionh
  SET0 counter 80
  LDA 0 variable.curroffset
  LDA 1 variable.curroffset1
  JSR GSTRETCHADD

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
  
  // optional display progressive
  LDA 0 labels.configblock+5
  JZ 0 skipdisplay
  JSR DISPLAY
LABEL skipdisplay
    
  // optional display backprojection without adding
  LDA 0 labels.configblock+5
  LDA 1 constant(2)
  SUB 1 0
  JNZ 0 skipzeroimage
  LDA 1 constant(labels.image)
  LDA 2 constant(0)
  SET0 ARG 6400
  JSR ASETC
LABEL skipzeroimage
  
  LDA 0 labels.configblock+8
  LDA 1 constant(2)
  SUB 1 0
  JNZ 0 skipprogressiveexport
  SET0 exportcount 100
  LDA 2 constant(labels.image)
LABEL loopprogressiveexport
  LDA 0 labels.configblock+9
  LDA 1 constant(0xff00)
  ADD 1 0
  WRITEBLOCK
  DSZ variable.exportcount
  JMP loopprogressiveexport
LABEL skipprogressiveexport
  
  LDA 0 variable.curangle
  LDA 1 labels.configblock+1
  ADD 0 1
  STA 1 variable.curangle
  LDA 1 constant(180)
  SUB 1 0
  JNZ 0 loopprojs
  
  // display end result
  JSR DISPLAY
  
  // export end result
  LDA 0 labels.configblock+8
  LDA 1 constant(1)
  SUB 1 0
  JNZ 0 skipfinalexport
  SET0 exportcount 100
  LDA 2 constant(labels.image)
LABEL loopfinalexport
  LDA 0 labels.configblock+9
  LDA 1 constant(0xff00)
  ADD 1 0
  WRITEBLOCK
  DSZ variable.exportcount
  JMP loopfinalexport
  
  LDA 0 labels.configblock+9
  INC 0 0
  LDA 1 constant(255)
  AND 1 0
  STA 0 labels.configblock+9
LABEL skipfinalexport
  
  HALT
    
//------------------------
LABEL DISPLAY
  // display 80x80 image on TFT with autorange
  STA 3 variable.display_ret
  
  // find maximum value of positive numbers in AC2
  LDA 3 constant(labels.image)
  LDA 2 constant(0)
  LDA 1 constant(-6400)
LABEL display_loopmax
  LDA 0 AC3
  INC 3 3
  MOV 0 0 NOLOAD SL CZ SZC
  SUB 0 0
  SUB 0 2 NOLOAD CZ SNC
  MOV 0 2
  INC 1 1 SZR
  JMP display_loopmax
  STA 2 variable.displaymax
  
  // level
  LDA 1 labels.configblock+6
  LDA 2 variable.displaymax
  JSR MULT
  MOV 1 2
  MOV 0 1
  MOV 2 0
  REPEAT 7 LASR
  MOV 0 2
  
  STA 2 variable.levelshift
  LDA 1 constant(labels.image)
  SET0 ARG 6400
  JSR AADDC
  
  // window
  LDA 1 labels.configblock+7
  LDA 2 variable.displaymax
  JSR MULT
  MOV 1 2
  MOV 0 1
  MOV 2 0
  REPEAT 7 LASR
  MOV 0 2
      
  // set display range to windows*maximum/128 and configure display; zoom=3, address=0 range = max/256+1
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
  
  LDA 2 variable.levelshift
  NEG 2 2
  LDA 1 constant(labels.image)
  SET0 ARG 6400
  JSR AADDC
  
  JMP variable.display_ret IND
  //-------------------
  
  //-------------------
  // read selected sinogram from SD, eeprom or flash; two 64 words per sinogram
LABEL readprojection
  STA 3 variable.readp_ret
  
  LDA 1 labels.configblock+0
  NEG 1 0
  INC 0 0 SNR
  LDA 1 constant(0xf000) // 1 --> 4 points
  INC 0 0 SNR
  LDA 1 constant(0xf200) // 2 --> head CT
  INC 0 0 SNR
  LDA 1 constant(0xf400) // 3 --> head CT
  INC 0 0 SNR
  LDA 1 constant(0xf600) // 4 --> head CT
  INC 0 0 SNR
  LDA 1 constant(0xf800) // 5 --> head CT
  INC 0 0 SNR
  LDA 1 constant(0xfa00) // 6 --> head CT
  INC 0 0 SNR
  LDA 1 constant(0xfc00) // 7 --> head CT
  
  LDA 0 variable.curangle
  ADD 0 0
  ADD 1 0
  
  LDA 2 constant(labels.projection)
  READBLOCK
  READBLOCK
  
  // sinogram 0 -> test centering, projection[64] is origin
  LDA 1 labels.configblock+0
  JNZ 1 skiptestcenter
  LDA 1 constant(labels.projection)
  LDA 2 constant(0)
  SET0 ARG 128
  JSR ASETC
  LDA 2 constant(labels.projection+64)
  LDA 0 constant(1000)
  STA 0 AC2
LABEL skiptestcenter
  JMP variable.readp_ret IND
  //-------------------
  
  //------------------------
LABEL SINOGRAM
  // display 128x180 sinogram on TFT with autorange
  STA 3 variable.sinogram_ret
   
  // clear screen
  LDA 0 constant(octal(0142))
  LDA 1 constant(0)
  GRMODE
  
  // find maximum value of positive numbers in AC2
  SET0 maxvalue 0
  SET0 curangle 170

LABEL sinogram_angles
  JSR readprojection
  
  LDA 1 constant(-128)
  LDA 2 variable.maxvalue
  LDA 3 constant(labels.projection)
LABEL sinogram_loopmax
  LDA 0 AC3
  INC 3 3
  MOV 0 0 NOLOAD SL CZ SZC
  SUB 0 0
  SUB 0 2 NOLOAD CZ SNC
  MOV 0 2
  INC 1 1 SZR
  JMP sinogram_loopmax
  STA 2 variable.maxvalue
  
  LDA 0 variable.curangle
  LDA 1 constant(10)
  SUB 1 0
  STA 0 variable.curangle
  JNZ 0 sinogram_angles
    
  // set display range to maximum value and configure display; zoom=1, address=0, clear, range = max/256+1
  LDA 2 variable.maxvalue
  REPEAT 8 ASR 2 2 
  REPEAT 6 ASL 2 2 
  LDA 0 constant(octal(0102))
  ADD 2 0
  STA 0 variable.displaymode
  LDA 1 constant(320*0+248)
  GRMODE 
  
  SET0 curangle 0
  LDA 0 constant(180)
  STA 0 variable.rows
LABEL sinogram_start
  JSR readprojection
  
  SET0 ARG 64
  LDA 2 constant(labels.projection)
  LDA 1 constant(labels.projection2)
  JSR DOWNSIZE
  
  LDA 0 constant(labels.projection2)
  LDA 1 constant(64)
  GRDISPLAY
  
  LDA 1 variable.curangle
  REPEAT 6 ASL 1 1
  MOV 1 2
  REPEAT 2 ASL 2 2
  ADD 2 1
  LDA 0 constant(248)
  ADD 0 1
  LDA 0 variable.displaymode
  GRMODE
  
  ISZ variable.curangle
  DSZ variable.rows
  JMP sinogram_start
  JMP variable.sinogram_ret IND
  //-------------------

  //-----------------
  // AC0/1 = AC1*AC2 (unsigned)
  LABEL MULT   
  SUB 0 0
  // AC0/1 = AC0+AC1*AC2
  LABEL MULTA
  STA 3 variable.MULTR
  LDA 3 constant(-16)
LABEL MULT1
  MOV 1 1 SR SNC
  MOV 0 0 SR SKP
  ADD 2 0 SR CZ
  INC 3 3 SZR
  JMP MULT1
  MOV 1 1 SR CC
  JMP variable.MULTR IND
  //-----------------
  
LABEL LOADIMAGE
  STA 3 variable.load_ret
  LDA 2 constant(labels.image)
  LDA 3 constant(-100)
LABEL loopload
  LDA 0 labels.configblock+10
  LDA 1 constant(0xff00)
  ADD 1 0
  READBLOCK
  INC 3 3 SZR
  JMP loopload
  // below only used side effect of read block 0: resets contiguous file reader
  SUB 0 0
  LDA 2 constant(labels.projection3)
  READBLOCK 
  JMP variable.load_ret IND
  
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
  // downsize factor 2
  //AC2 input data pointer
  //AC1 output data pointer
  //ARG output count
LABEL DOWNSIZE
  STA 3 variable.AUTO
  MOV 1 3
LABEL downsize2
  LDA 0 AC2
  INC 2 2
  INC 2 2
  STA 0 AC3
  INC 3 3
  DSZ variable.ARG
  JMP downsize2
  JMP variable.AUTO IND
  //-------------------

//-------------------
  // zoom factor 2 up without interpolation
  //AC2 input data pointer, provide zero guard word after
  //AC1 output data pointer
  //ARG input count
LABEL zoom2x
  STA 3 variable.AUTO
  MOV 1 3
LABEL zoom2x2
  LDA 0 AC2
  STA 0 AC3
  STA 0 AC3 1
  INC 3 3
  INC 3 3
  INC 2 2
  DSZ variable.ARG
  JMP zoom2x2
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
  
  //-----------------
  // filter projection (needs 16 guard words) into projection2
LABEL lemayfilter
  STA 3 variable.AUTO
  LDA 0 constant(labels.projection)
  STA 0 variable.input
  LDA 0 constant(labels.projection2)
  STA 0 variable.output
LABEL lemayfilter2
  SET0 sum 0
  MACRO lm LDA 2 variable.input|LDA 0 %1 AC2|LDA 1 -%1 AC2|ADD 0 1|LDA 2 constant(labels.lemay)|LDA 2 %1 AC2|JSR MULT|LDA 1 variable.sum|ADD 0 1|STA 1 variable.sum
  lm 1
  lm 2
  lm 3
  lm 4
  lm 5
  lm 6
  lm 7
  lm 8
  lm 9
  lm 10
  lm 11
  lm 12
  lm 13
  lm 14
  lm 15
  
  LDA 2 variable.input
  LDA 1 AC2
  INC 2 2
  STA 2 variable.input
  LDA 0 variable.sum
  SUB 0 1
 
  LDA 2 variable.output
  STA 1 AC2
  INC 2 2
  STA 2 variable.output
  DSZ variable.ARG
  JMP lemayfilter2
  JMP variable.AUTO IND

// scaled constants from Lemay's circle method from Hounsfield book 'Intuitive genius of CT' page 114; but scaled 
LABEL lemay
  DW math.round(65536*1-1)        
  DW math.round(65536*0.44447) 
  DW math.round(65536*0.01498)
  DW math.round(65536*0.02548)
  DW math.round(65536*0.002699)
  DW math.round(65536*0.006653) 
  DW math.round(65536*0.0004478)
  DW math.round(65536*0.00003331)
  DW math.round(65536*0.00002552)
  DW math.round(65536*0.00002022)
  DW math.round(65536*0.00001639) 
  DW math.round(65536*0.000001356)
  DW math.round(65536*0.000001140)
  DW math.round(65536*0.0000009717)
  DW math.round(65536*0.00000008381)
  DW math.round(65536*0.00000007306)
  //DW math.round(65536*0.004478)
  //DW math.round(65536*0.003331)
  //DW math.round(65536*0.002552)
  //DW math.round(65536*0.002022)
  //DW math.round(65536*0.001639) 
  //DW math.round(65536*0.001356)
  //DW math.round(65536*0.001140)
  //DW math.round(65536*0.0009717)
  //DW math.round(65536*0.0008381)
  //DW math.round(65536*0.0007306)
  //-----------------
           
  //-------------------
  //Copy short array to short array
  //AC2 source data pointer
  //AC1 destination data pointer
  //ARG count
LABEL ASETA
  STA 3 variable.AUTO
  MOV 1 3
LABEL ASETA2
  LDA 0 AC2
  STA 0 AC3
  INC 2 2
  INC 3 3
  DSZ variable.ARG
  JMP ASETA2
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
  //Shift short array right by 4 (TBD constant value)
  //AC1 destination data pointer
  //ARG count
LABEL ASHRC
  STA 3 variable.AUTO
  MOV 1 3
LABEL ASHRC2
  LDA 0 AC3
  REPEAT 4 ASR 0 0
  STA 0 AC3
  INC 3 3
  DSZ variable.ARG
  JMP ASHRC2
  MOV 3 1
  JMP variable.AUTO IND
  //-------------------

  //-------------------
  //Add constant to short array 
  //AC2 constant
  //AC1 destination data pointer
  //ARG count
LABEL AADDC
  STA 3 variable.AUTO
  MOV 1 3
LABEL AADDC2
  LDA 0 AC3
  ADD 2 0
  STA 0 AC3
  INC 3 3
  DSZ variable.ARG
  JMP AADDC2
  MOV 3 1
  JMP variable.AUTO IND
  //-------------------
 
  //-------------------
  // Stretch data and add to target, array ARG must be guarded with many zeros for too low or too high pixel indices, e.g. 128 256 128 allocation
  // *autoinc.target += ARG[j], where j= I*scale+offset
  // autoinc.target points one prior to image (need to set once only)
  // ARG projection data
  // 0 subpixel address, increases with dx
  // 1 pixel address, increased with dx1  
  // counter is number of points (destroyed)
  // unrolled: 13 cycles per pixel + 18/16 cycles overhead (15.25 for 8 times, 14.1 for 16 times unroll)
 
// step subpixel address and store integer part
MACRO GS1 STA 1 variable.temp%1|ADD 3 0 CZ SZC|INC 1 1|ADD 2 1

// load projection and add to AC2[i]
MACRO GS2 LDA 1 IND variable.temp%1|LDA 3 %1 AC2|ADD 1 3|STA 3 %1 AC2

LABEL GSTRETCHADDFAST
  STA 3 variable.AUTO
  LDA 3 variable.ARG
  ADD 3 1   // reg1 contains offset address
  
  // loop unrolled 16 times, adjust counter
  LDA 3 variable.counter
  REPEAT 4 MOV 3 3 CZ SR
  STA 3 variable.counter
    
LABEL GSTRETCHADDFAST1
  // 4 cycles
  LDA 3 variable.dx // step subpixel source address
  LDA 2 variable.dx1
  
  // 5 cycles per pixel
  REPEAT 16 GS1 %r
  
  // 4 cycles
  STA 1 variable.temp
  LDA 2 autoinc.target

  // 8 cycles per pixel
  REPEAT 16 GS2 %r

  // 10 cycles
  LDA 1 constant(16)
  ADD 1 2
  STA 2 autoinc.target
  LDA 1 variable.temp
  DSZ variable.counter
  JMP GSTRETCHADDFAST1
  
  JMP variable.AUTO IND
  //-------------------

  //-------------------
  // Stretch data and add to target, array ARG must be guarded with many zeros for too low or too high pixel indices, e.g. 128 256 128 allocation
  // *autoinc.target += ARG[j], where j= I*scale+offset
  // autoinc.target points one prior to image (need to set once only)
  // ARG projection data
  // 0 subpixel address, increases with dx
  // 1 pixel address, increased with dx1  (in ac3 during loop)
  // counter is number of points (destroyed)
  // 20 cycles, not unrolled
 
LABEL GSTRETCHADD
  STA 3 variable.AUTO
  LDA 3 variable.ARG
  ADD 1 3
LABEL GSTRETCHADD1
  LDA 1 AC3 // get projection data and accumulate

  LDA 2 IND autoinc.target
  ADD 2 1
  LDA 2 autoinc.target
  STA 1 AC2
  
  LDA 1 variable.dx // step subpixel source address
  LDA 2 variable.dx1
  ADD 1 0 CZ SZC
  INC 3 3
  ADD 2 3
  DSZ variable.counter
  JMP GSTRETCHADD1
  JMP variable.AUTO IND
  //-------------------

LABEL endofcode

ORG octal(0400)
  REPEAT 16 DW 0
  LABEL projection
  ORG pc+128
  REPEAT 16 DW 0

// #IF config.uploadtables 
// location of first pixel in projection array
// 80x80 image output; input is scaled to 256 words -> center at 128; all values scaled by 16 bit fixed point floats
LABEL offsets
  REPEAT 180 DW math.round(64*(-40*config.pscale*math.cos((pc-labels.offsets)*math.pi/180)-40*config.pscale*math.sin((pc-labels.offsets)*math.pi/180)+128.5))

// scaled sin(angle) used to calculate stepx and stepy
LABEL sintable
  REPEAT 270 DW math.round(8192*config.pscale*math.sin((pc-labels.sintable)*math.pi/180))
// #ENDIF

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
  
]], true) -- false omits go command

writeport(32, t, 100)
--writetelnet("192.168.1.234", t, 100)
map()