dofile ('c:\\software\\nova1200-restoration\\lua_assembler\\telnet_nova.lua')

-- Wiener filter: 128 words
-- offsets: 100 words
-- stepx: 100 words
-- stepy: 100 words
-- swaps: 128 words
-- butterflies: 448 words
-- 2 inputs: 256 words
-- 2 filtered: 256 words
-- 1 interpolated: 256 words
--------------- 1772 words
-- 1 slice: 6400 words ; put at 8192


--[[
plot wiener filter: plot 4000 10000
plot offsets: plot 4200 40000
plot stepx: plot 4344 400
plot stepy: plot 4510 400
plot swaps: plot 4656 400
plot butterflies: plot 5056 40000
plot input: plot 10456 400
show slice: vis 20000 14400
clear slice: clear 20000 14400
]]


assemble(octal(1000), true, [[
ORG octal(1000)
  // test program, multiplies two signed numbers and prints results to console
  JMP START
LABEL AAP
  DW -500
LABEL NOOT
  DW -20
LABEL START
  LDA 0 AAP
  LDA 1 NOOT
  LDA 2 mes2
  MESSAGE
  
  SUB 0 0
  LDA 1 AAP
  LDA 2 NOOT
  JSR SMULT
  LDA 2 mes
  MESSAGE
  MOV 0 0
  HALT

LABEL mes
  DW message
LABEL message
  STRING 'product=%0_%1\r\n'
LABEL mes2
  DW message2
LABEL message2
  STRING '%0*%1_'
  
// multiply AC0/1 = AC1*AC2
LABEL MULT   
  SUB 0 0 //test
  // AC0/1 = AC0+AC1*AC2
LABEL MULTA

  STA 3 MULTR
  LDA 3 const16
LABEL MULT1
  MOV 1 1 SR SNC
  MOV 0 0 SR SKP
  ADD 2 0 SR CZ
  INC 3 3 SZR
  JMP MULT1
  MOV 1 1 SR CZ
  JMP MULTR IND
LABEL MULTR
  DW 0  
LABEL const16
  DW -16

// signed multiply: AC0/1 = AC1*AC2
LABEL SMULT
  STA 3 SMULTR
  SUB 0 0
  MOV 1 1 NOLOAD SL SZC
  INC 0 0
  MOV 2 2 NOLOAD SL SZC
  INC 0 0
  MOV 1 1 NOLOAD SL SZC
  NEG 1 1
  MOV 2 2 NOLOAD SL SZC
  NEG 2 2
  SUB 3 3
  INC 3 3
  AND 3 0 SNR
  JMP SMULT1
  JSR MULT
  NEG 0 0
  NEG 1 1
  JMP SMULTR IND
LABEL SMULT1
  JSR MULT
  JMP SMULTR IND
LABEL SMULTR
  DW 0
  
// logical shift AC0 8 places right
LABEL SHIFT8R
  STA 3 SHIFT8RET
  REPEAT 8 MOV 0 0 SR CZ
  JMP SHIFT8RET IND
LABEL SHIFT8RET
  DW 0
  
ORG octal(4000)

LABEL wiener
  REPEAT 64 DW math.floor(256*(pc-labels.wiener)*(math.cos(3.1416*(pc-labels.wiener)/64)+1)/2)
  REPEAT 64 DW math.floor(256*(labels.wiener+128-pc)*(math.cos(3.1416*(labels.wiener+128-pc)/64)+1)/2)

LABEL offsets
  REPEAT 100 DW math.floor(256*(-40.5*math.sin((pc-labels.offsets)*3.1416/100)+64.5))

LABEL stepx
  REPEAT 100 DW math.floor(256*math.sin((pc-labels.stepx)*3.1416/100))

LABEL stepy
  REPEAT 100 DW math.floor(256*math.cos((pc-labels.stepy)*3.1416/100))

LABEL FFTSIZE
DW 128
LABEL NBUTTERFLIES
DW 448

LABEL SWAPS
DW 2
DW 128
DW 4
DW 64
DW 6
DW 192
DW 8
DW 32
DW 10
DW 160
DW 12
DW 96
DW 14
DW 224
DW 18
DW 144
DW 20
DW 80
DW 22
DW 208
DW 24
DW 48
DW 26
DW 176
DW 28
DW 112
DW 30
DW 240
DW 34
DW 136
DW 36
DW 72
DW 38
DW 200
DW 42
DW 168
DW 44
DW 104
DW 46
DW 232
DW 50
DW 152
DW 52
DW 88
DW 54
DW 216
DW 58
DW 184
DW 60
DW 120
DW 62
DW 248
DW 66
DW 132
DW 70
DW 196
DW 74
DW 164
DW 76
DW 100
DW 78
DW 228
DW 82
DW 148
DW 86
DW 212
DW 90
DW 180
DW 92
DW 116
DW 94
DW 244
DW 98
DW 140
DW 102
DW 204
DW 106
DW 172
DW 110
DW 236
DW 114
DW 156
DW 118
DW 220
DW 122
DW 188
DW 126
DW 252
DW 134
DW 194
DW 138
DW 162
DW 142
DW 226
DW 150
DW 210
DW 154
DW 178
DW 158
DW 242
DW 166
DW 202
DW 174
DW 234
DW 182
DW 218
DW 190
DW 250
DW 206
DW 230
DW 222
DW 246
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
DW 0
LABEL BUTTERFLIES
DW 16384
DW 0
DW 0
DW 2
DW 16384
DW 0
DW 4
DW 6
DW 16384
DW 0
DW 8
DW 10
DW 16384
DW 0
DW 12
DW 14
DW 16384
DW 0
DW 16
DW 18
DW 16384
DW 0
DW 20
DW 22
DW 16384
DW 0
DW 24
DW 26
DW 16384
DW 0
DW 28
DW 30
DW 16384
DW 0
DW 32
DW 34
DW 16384
DW 0
DW 36
DW 38
DW 16384
DW 0
DW 40
DW 42
DW 16384
DW 0
DW 44
DW 46
DW 16384
DW 0
DW 48
DW 50
DW 16384
DW 0
DW 52
DW 54
DW 16384
DW 0
DW 56
DW 58
DW 16384
DW 0
DW 60
DW 62
DW 16384
DW 0
DW 64
DW 66
DW 16384
DW 0
DW 68
DW 70
DW 16384
DW 0
DW 72
DW 74
DW 16384
DW 0
DW 76
DW 78
DW 16384
DW 0
DW 80
DW 82
DW 16384
DW 0
DW 84
DW 86
DW 16384
DW 0
DW 88
DW 90
DW 16384
DW 0
DW 92
DW 94
DW 16384
DW 0
DW 96
DW 98
DW 16384
DW 0
DW 100
DW 102
DW 16384
DW 0
DW 104
DW 106
DW 16384
DW 0
DW 108
DW 110
DW 16384
DW 0
DW 112
DW 114
DW 16384
DW 0
DW 116
DW 118
DW 16384
DW 0
DW 120
DW 122
DW 16384
DW 0
DW 124
DW 126
DW 16384
DW 0
DW 128
DW 130
DW 16384
DW 0
DW 132
DW 134
DW 16384
DW 0
DW 136
DW 138
DW 16384
DW 0
DW 140
DW 142
DW 16384
DW 0
DW 144
DW 146
DW 16384
DW 0
DW 148
DW 150
DW 16384
DW 0
DW 152
DW 154
DW 16384
DW 0
DW 156
DW 158
DW 16384
DW 0
DW 160
DW 162
DW 16384
DW 0
DW 164
DW 166
DW 16384
DW 0
DW 168
DW 170
DW 16384
DW 0
DW 172
DW 174
DW 16384
DW 0
DW 176
DW 178
DW 16384
DW 0
DW 180
DW 182
DW 16384
DW 0
DW 184
DW 186
DW 16384
DW 0
DW 188
DW 190
DW 16384
DW 0
DW 192
DW 194
DW 16384
DW 0
DW 196
DW 198
DW 16384
DW 0
DW 200
DW 202
DW 16384
DW 0
DW 204
DW 206
DW 16384
DW 0
DW 208
DW 210
DW 16384
DW 0
DW 212
DW 214
DW 16384
DW 0
DW 216
DW 218
DW 16384
DW 0
DW 220
DW 222
DW 16384
DW 0
DW 224
DW 226
DW 16384
DW 0
DW 228
DW 230
DW 16384
DW 0
DW 232
DW 234
DW 16384
DW 0
DW 236
DW 238
DW 16384
DW 0
DW 240
DW 242
DW 16384
DW 0
DW 244
DW 246
DW 16384
DW 0
DW 248
DW 250
DW 16384
DW 0
DW 252
DW 254
DW 16384
DW 0
DW 0
DW 4
DW 16384
DW 0
DW 8
DW 12
DW 16384
DW 0
DW 16
DW 20
DW 16384
DW 0
DW 24
DW 28
DW 16384
DW 0
DW 32
DW 36
DW 16384
DW 0
DW 40
DW 44
DW 16384
DW 0
DW 48
DW 52
DW 16384
DW 0
DW 56
DW 60
DW 16384
DW 0
DW 64
DW 68
DW 16384
DW 0
DW 72
DW 76
DW 16384
DW 0
DW 80
DW 84
DW 16384
DW 0
DW 88
DW 92
DW 16384
DW 0
DW 96
DW 100
DW 16384
DW 0
DW 104
DW 108
DW 16384
DW 0
DW 112
DW 116
DW 16384
DW 0
DW 120
DW 124
DW 16384
DW 0
DW 128
DW 132
DW 16384
DW 0
DW 136
DW 140
DW 16384
DW 0
DW 144
DW 148
DW 16384
DW 0
DW 152
DW 156
DW 16384
DW 0
DW 160
DW 164
DW 16384
DW 0
DW 168
DW 172
DW 16384
DW 0
DW 176
DW 180
DW 16384
DW 0
DW 184
DW 188
DW 16384
DW 0
DW 192
DW 196
DW 16384
DW 0
DW 200
DW 204
DW 16384
DW 0
DW 208
DW 212
DW 16384
DW 0
DW 216
DW 220
DW 16384
DW 0
DW 224
DW 228
DW 16384
DW 0
DW 232
DW 236
DW 16384
DW 0
DW 240
DW 244
DW 16384
DW 0
DW 248
DW 252
DW 0
DW 16384
DW 2
DW 6
DW 0
DW 16384
DW 10
DW 14
DW 0
DW 16384
DW 18
DW 22
DW 0
DW 16384
DW 26
DW 30
DW 0
DW 16384
DW 34
DW 38
DW 0
DW 16384
DW 42
DW 46
DW 0
DW 16384
DW 50
DW 54
DW 0
DW 16384
DW 58
DW 62
DW 0
DW 16384
DW 66
DW 70
DW 0
DW 16384
DW 74
DW 78
DW 0
DW 16384
DW 82
DW 86
DW 0
DW 16384
DW 90
DW 94
DW 0
DW 16384
DW 98
DW 102
DW 0
DW 16384
DW 106
DW 110
DW 0
DW 16384
DW 114
DW 118
DW 0
DW 16384
DW 122
DW 126
DW 0
DW 16384
DW 130
DW 134
DW 0
DW 16384
DW 138
DW 142
DW 0
DW 16384
DW 146
DW 150
DW 0
DW 16384
DW 154
DW 158
DW 0
DW 16384
DW 162
DW 166
DW 0
DW 16384
DW 170
DW 174
DW 0
DW 16384
DW 178
DW 182
DW 0
DW 16384
DW 186
DW 190
DW 0
DW 16384
DW 194
DW 198
DW 0
DW 16384
DW 202
DW 206
DW 0
DW 16384
DW 210
DW 214
DW 0
DW 16384
DW 218
DW 222
DW 0
DW 16384
DW 226
DW 230
DW 0
DW 16384
DW 234
DW 238
DW 0
DW 16384
DW 242
DW 246
DW 0
DW 16384
DW 250
DW 254
DW 16384
DW 0
DW 0
DW 8
DW 16384
DW 0
DW 16
DW 24
DW 16384
DW 0
DW 32
DW 40
DW 16384
DW 0
DW 48
DW 56
DW 16384
DW 0
DW 64
DW 72
DW 16384
DW 0
DW 80
DW 88
DW 16384
DW 0
DW 96
DW 104
DW 16384
DW 0
DW 112
DW 120
DW 16384
DW 0
DW 128
DW 136
DW 16384
DW 0
DW 144
DW 152
DW 16384
DW 0
DW 160
DW 168
DW 16384
DW 0
DW 176
DW 184
DW 16384
DW 0
DW 192
DW 200
DW 16384
DW 0
DW 208
DW 216
DW 16384
DW 0
DW 224
DW 232
DW 16384
DW 0
DW 240
DW 248
DW 11585
DW 11585
DW 2
DW 10
DW 11585
DW 11585
DW 18
DW 26
DW 11585
DW 11585
DW 34
DW 42
DW 11585
DW 11585
DW 50
DW 58
DW 11585
DW 11585
DW 66
DW 74
DW 11585
DW 11585
DW 82
DW 90
DW 11585
DW 11585
DW 98
DW 106
DW 11585
DW 11585
DW 114
DW 122
DW 11585
DW 11585
DW 130
DW 138
DW 11585
DW 11585
DW 146
DW 154
DW 11585
DW 11585
DW 162
DW 170
DW 11585
DW 11585
DW 178
DW 186
DW 11585
DW 11585
DW 194
DW 202
DW 11585
DW 11585
DW 210
DW 218
DW 11585
DW 11585
DW 226
DW 234
DW 11585
DW 11585
DW 242
DW 250
DW 0
DW 16383
DW 4
DW 12
DW 0
DW 16383
DW 20
DW 28
DW 0
DW 16383
DW 36
DW 44
DW 0
DW 16383
DW 52
DW 60
DW 0
DW 16383
DW 68
DW 76
DW 0
DW 16383
DW 84
DW 92
DW 0
DW 16383
DW 100
DW 108
DW 0
DW 16383
DW 116
DW 124
DW 0
DW 16383
DW 132
DW 140
DW 0
DW 16383
DW 148
DW 156
DW 0
DW 16383
DW 164
DW 172
DW 0
DW 16383
DW 180
DW 188
DW 0
DW 16383
DW 196
DW 204
DW 0
DW 16383
DW 212
DW 220
DW 0
DW 16383
DW 228
DW 236
DW 0
DW 16383
DW 244
DW 252
DW -11585
DW 11585
DW 6
DW 14
DW -11585
DW 11585
DW 22
DW 30
DW -11585
DW 11585
DW 38
DW 46
DW -11585
DW 11585
DW 54
DW 62
DW -11585
DW 11585
DW 70
DW 78
DW -11585
DW 11585
DW 86
DW 94
DW -11585
DW 11585
DW 102
DW 110
DW -11585
DW 11585
DW 118
DW 126
DW -11585
DW 11585
DW 134
DW 142
DW -11585
DW 11585
DW 150
DW 158
DW -11585
DW 11585
DW 166
DW 174
DW -11585
DW 11585
DW 182
DW 190
DW -11585
DW 11585
DW 198
DW 206
DW -11585
DW 11585
DW 214
DW 222
DW -11585
DW 11585
DW 230
DW 238
DW -11585
DW 11585
DW 246
DW 254
DW 16384
DW 0
DW 0
DW 16
DW 16384
DW 0
DW 32
DW 48
DW 16384
DW 0
DW 64
DW 80
DW 16384
DW 0
DW 96
DW 112
DW 16384
DW 0
DW 128
DW 144
DW 16384
DW 0
DW 160
DW 176
DW 16384
DW 0
DW 192
DW 208
DW 16384
DW 0
DW 224
DW 240
DW 15136
DW 6269
DW 2
DW 18
DW 15136
DW 6269
DW 34
DW 50
DW 15136
DW 6269
DW 66
DW 82
DW 15136
DW 6269
DW 98
DW 114
DW 15136
DW 6269
DW 130
DW 146
DW 15136
DW 6269
DW 162
DW 178
DW 15136
DW 6269
DW 194
DW 210
DW 15136
DW 6269
DW 226
DW 242
DW 11585
DW 11585
DW 4
DW 20
DW 11585
DW 11585
DW 36
DW 52
DW 11585
DW 11585
DW 68
DW 84
DW 11585
DW 11585
DW 100
DW 116
DW 11585
DW 11585
DW 132
DW 148
DW 11585
DW 11585
DW 164
DW 180
DW 11585
DW 11585
DW 196
DW 212
DW 11585
DW 11585
DW 228
DW 244
DW 6269
DW 15136
DW 6
DW 22
DW 6269
DW 15136
DW 38
DW 54
DW 6269
DW 15136
DW 70
DW 86
DW 6269
DW 15136
DW 102
DW 118
DW 6269
DW 15136
DW 134
DW 150
DW 6269
DW 15136
DW 166
DW 182
DW 6269
DW 15136
DW 198
DW 214
DW 6269
DW 15136
DW 230
DW 246
DW 0
DW 16383
DW 8
DW 24
DW 0
DW 16383
DW 40
DW 56
DW 0
DW 16383
DW 72
DW 88
DW 0
DW 16383
DW 104
DW 120
DW 0
DW 16383
DW 136
DW 152
DW 0
DW 16383
DW 168
DW 184
DW 0
DW 16383
DW 200
DW 216
DW 0
DW 16383
DW 232
DW 248
DW -6269
DW 15136
DW 10
DW 26
DW -6269
DW 15136
DW 42
DW 58
DW -6269
DW 15136
DW 74
DW 90
DW -6269
DW 15136
DW 106
DW 122
DW -6269
DW 15136
DW 138
DW 154
DW -6269
DW 15136
DW 170
DW 186
DW -6269
DW 15136
DW 202
DW 218
DW -6269
DW 15136
DW 234
DW 250
DW -11585
DW 11585
DW 12
DW 28
DW -11585
DW 11585
DW 44
DW 60
DW -11585
DW 11585
DW 76
DW 92
DW -11585
DW 11585
DW 108
DW 124
DW -11585
DW 11585
DW 140
DW 156
DW -11585
DW 11585
DW 172
DW 188
DW -11585
DW 11585
DW 204
DW 220
DW -11585
DW 11585
DW 236
DW 252
DW -15136
DW 6269
DW 14
DW 30
DW -15136
DW 6269
DW 46
DW 62
DW -15136
DW 6269
DW 78
DW 94
DW -15136
DW 6269
DW 110
DW 126
DW -15136
DW 6269
DW 142
DW 158
DW -15136
DW 6269
DW 174
DW 190
DW -15136
DW 6269
DW 206
DW 222
DW -15136
DW 6269
DW 238
DW 254
DW 16384
DW 0
DW 0
DW 32
DW 16384
DW 0
DW 64
DW 96
DW 16384
DW 0
DW 128
DW 160
DW 16384
DW 0
DW 192
DW 224
DW 16069
DW 3196
DW 2
DW 34
DW 16069
DW 3196
DW 66
DW 98
DW 16069
DW 3196
DW 130
DW 162
DW 16069
DW 3196
DW 194
DW 226
DW 15136
DW 6269
DW 4
DW 36
DW 15136
DW 6269
DW 68
DW 100
DW 15136
DW 6269
DW 132
DW 164
DW 15136
DW 6269
DW 196
DW 228
DW 13622
DW 9102
DW 6
DW 38
DW 13622
DW 9102
DW 70
DW 102
DW 13622
DW 9102
DW 134
DW 166
DW 13622
DW 9102
DW 198
DW 230
DW 11585
DW 11585
DW 8
DW 40
DW 11585
DW 11585
DW 72
DW 104
DW 11585
DW 11585
DW 136
DW 168
DW 11585
DW 11585
DW 200
DW 232
DW 9102
DW 13622
DW 10
DW 42
DW 9102
DW 13622
DW 74
DW 106
DW 9102
DW 13622
DW 138
DW 170
DW 9102
DW 13622
DW 202
DW 234
DW 6269
DW 15136
DW 12
DW 44
DW 6269
DW 15136
DW 76
DW 108
DW 6269
DW 15136
DW 140
DW 172
DW 6269
DW 15136
DW 204
DW 236
DW 3196
DW 16069
DW 14
DW 46
DW 3196
DW 16069
DW 78
DW 110
DW 3196
DW 16069
DW 142
DW 174
DW 3196
DW 16069
DW 206
DW 238
DW 0
DW 16383
DW 16
DW 48
DW 0
DW 16383
DW 80
DW 112
DW 0
DW 16383
DW 144
DW 176
DW 0
DW 16383
DW 208
DW 240
DW -3196
DW 16069
DW 18
DW 50
DW -3196
DW 16069
DW 82
DW 114
DW -3196
DW 16069
DW 146
DW 178
DW -3196
DW 16069
DW 210
DW 242
DW -6269
DW 15136
DW 20
DW 52
DW -6269
DW 15136
DW 84
DW 116
DW -6269
DW 15136
DW 148
DW 180
DW -6269
DW 15136
DW 212
DW 244
DW -9102
DW 13622
DW 22
DW 54
DW -9102
DW 13622
DW 86
DW 118
DW -9102
DW 13622
DW 150
DW 182
DW -9102
DW 13622
DW 214
DW 246
DW -11585
DW 11585
DW 24
DW 56
DW -11585
DW 11585
DW 88
DW 120
DW -11585
DW 11585
DW 152
DW 184
DW -11585
DW 11585
DW 216
DW 248
DW -13622
DW 9102
DW 26
DW 58
DW -13622
DW 9102
DW 90
DW 122
DW -13622
DW 9102
DW 154
DW 186
DW -13622
DW 9102
DW 218
DW 250
DW -15136
DW 6269
DW 28
DW 60
DW -15136
DW 6269
DW 92
DW 124
DW -15136
DW 6269
DW 156
DW 188
DW -15136
DW 6269
DW 220
DW 252
DW -16069
DW 3196
DW 30
DW 62
DW -16069
DW 3196
DW 94
DW 126
DW -16069
DW 3196
DW 158
DW 190
DW -16069
DW 3196
DW 222
DW 254
DW 16384
DW 0
DW 0
DW 64
DW 16384
DW 0
DW 128
DW 192
DW 16305
DW 1605
DW 2
DW 66
DW 16305
DW 1605
DW 130
DW 194
DW 16069
DW 3196
DW 4
DW 68
DW 16069
DW 3196
DW 132
DW 196
DW 15678
DW 4756
DW 6
DW 70
DW 15678
DW 4756
DW 134
DW 198
DW 15136
DW 6269
DW 8
DW 72
DW 15136
DW 6269
DW 136
DW 200
DW 14449
DW 7723
DW 10
DW 74
DW 14449
DW 7723
DW 138
DW 202
DW 13622
DW 9102
DW 12
DW 76
DW 13622
DW 9102
DW 140
DW 204
DW 12665
DW 10393
DW 14
DW 78
DW 12665
DW 10393
DW 142
DW 206
DW 11585
DW 11585
DW 16
DW 80
DW 11585
DW 11585
DW 144
DW 208
DW 10393
DW 12665
DW 18
DW 82
DW 10393
DW 12665
DW 146
DW 210
DW 9102
DW 13622
DW 20
DW 84
DW 9102
DW 13622
DW 148
DW 212
DW 7723
DW 14449
DW 22
DW 86
DW 7723
DW 14449
DW 150
DW 214
DW 6269
DW 15136
DW 24
DW 88
DW 6269
DW 15136
DW 152
DW 216
DW 4756
DW 15678
DW 26
DW 90
DW 4756
DW 15678
DW 154
DW 218
DW 3196
DW 16069
DW 28
DW 92
DW 3196
DW 16069
DW 156
DW 220
DW 1605
DW 16305
DW 30
DW 94
DW 1605
DW 16305
DW 158
DW 222
DW 0
DW 16383
DW 32
DW 96
DW 0
DW 16383
DW 160
DW 224
DW -1605
DW 16305
DW 34
DW 98
DW -1605
DW 16305
DW 162
DW 226
DW -3196
DW 16069
DW 36
DW 100
DW -3196
DW 16069
DW 164
DW 228
DW -4756
DW 15678
DW 38
DW 102
DW -4756
DW 15678
DW 166
DW 230
DW -6269
DW 15136
DW 40
DW 104
DW -6269
DW 15136
DW 168
DW 232
DW -7723
DW 14449
DW 42
DW 106
DW -7723
DW 14449
DW 170
DW 234
DW -9102
DW 13622
DW 44
DW 108
DW -9102
DW 13622
DW 172
DW 236
DW -10393
DW 12665
DW 46
DW 110
DW -10393
DW 12665
DW 174
DW 238
DW -11585
DW 11585
DW 48
DW 112
DW -11585
DW 11585
DW 176
DW 240
DW -12665
DW 10393
DW 50
DW 114
DW -12665
DW 10393
DW 178
DW 242
DW -13622
DW 9102
DW 52
DW 116
DW -13622
DW 9102
DW 180
DW 244
DW -14449
DW 7723
DW 54
DW 118
DW -14449
DW 7723
DW 182
DW 246
DW -15136
DW 6269
DW 56
DW 120
DW -15136
DW 6269
DW 184
DW 248
DW -15678
DW 4756
DW 58
DW 122
DW -15678
DW 4756
DW 186
DW 250
DW -16069
DW 3196
DW 60
DW 124
DW -16069
DW 3196
DW 188
DW 252
DW -16305
DW 1605
DW 62
DW 126
DW -16305
DW 1605
DW 190
DW 254
DW 16384
DW 0
DW 0
DW 128
DW 16364
DW 803
DW 2
DW 130
DW 16305
DW 1605
DW 4
DW 132
DW 16206
DW 2404
DW 6
DW 134
DW 16069
DW 3196
DW 8
DW 136
DW 15892
DW 3980
DW 10
DW 138
DW 15678
DW 4756
DW 12
DW 140
DW 15426
DW 5519
DW 14
DW 142
DW 15136
DW 6269
DW 16
DW 144
DW 14810
DW 7005
DW 18
DW 146
DW 14449
DW 7723
DW 20
DW 148
DW 14053
DW 8423
DW 22
DW 150
DW 13622
DW 9102
DW 24
DW 152
DW 13159
DW 9759
DW 26
DW 154
DW 12665
DW 10393
DW 28
DW 156
DW 12139
DW 11002
DW 30
DW 158
DW 11585
DW 11585
DW 32
DW 160
DW 11002
DW 12139
DW 34
DW 162
DW 10393
DW 12665
DW 36
DW 164
DW 9759
DW 13159
DW 38
DW 166
DW 9102
DW 13622
DW 40
DW 168
DW 8423
DW 14053
DW 42
DW 170
DW 7723
DW 14449
DW 44
DW 172
DW 7005
DW 14810
DW 46
DW 174
DW 6269
DW 15136
DW 48
DW 176
DW 5519
DW 15426
DW 50
DW 178
DW 4756
DW 15678
DW 52
DW 180
DW 3980
DW 15892
DW 54
DW 182
DW 3196
DW 16069
DW 56
DW 184
DW 2404
DW 16206
DW 58
DW 186
DW 1605
DW 16305
DW 60
DW 188
DW 803
DW 16364
DW 62
DW 190
DW 0
DW 16383
DW 64
DW 192
DW -803
DW 16364
DW 66
DW 194
DW -1605
DW 16305
DW 68
DW 196
DW -2404
DW 16206
DW 70
DW 198
DW -3196
DW 16069
DW 72
DW 200
DW -3980
DW 15892
DW 74
DW 202
DW -4756
DW 15678
DW 76
DW 204
DW -5519
DW 15426
DW 78
DW 206
DW -6269
DW 15136
DW 80
DW 208
DW -7005
DW 14810
DW 82
DW 210
DW -7723
DW 14449
DW 84
DW 212
DW -8423
DW 14053
DW 86
DW 214
DW -9102
DW 13622
DW 88
DW 216
DW -9759
DW 13159
DW 90
DW 218
DW -10393
DW 12664
DW 92
DW 220
DW -11002
DW 12139
DW 94
DW 222
DW -11585
DW 11585
DW 96
DW 224
DW -12139
DW 11002
DW 98
DW 226
DW -12664
DW 10393
DW 100
DW 228
DW -13159
DW 9759
DW 102
DW 230
DW -13622
DW 9102
DW 104
DW 232
DW -14053
DW 8423
DW 106
DW 234
DW -14449
DW 7723
DW 108
DW 236
DW -14810
DW 7005
DW 110
DW 238
DW -15136
DW 6269
DW 112
DW 240
DW -15426
DW 5519
DW 114
DW 242
DW -15678
DW 4756
DW 116
DW 244
DW -15892
DW 3980
DW 118
DW 246
DW -16069
DW 3196
DW 120
DW 248
DW -16206
DW 2404
DW 122
DW 250
DW -16305
DW 1605
DW 124
DW 252
DW -16364
DW 803
DW 126
DW 254
  
LABEL input
REPEAT 128 DW math.floor(256*math.sqrt(1-(pc-labels.input-64)*(pc-labels.input-64)/1000))
LABEL input2
REPEAT 128 DW math.floor(256*math.sqrt(1-(pc-labels.input2-64)*(pc-labels.input2-64)/1000))
ORG pc+256

ORG 8192
LABEL slice
ORG pc+80*80

ORG octal(50)
LABEL INDMULT
  DW MULT
LABEL INDSMULT
  DW SMULT
LABEL _wiener
  DW wiener
LABEL _offsets
  DW offsets
LABEL _stepx
  DW stepx
LABEL _stepy
  DW stepy
LABEL _FFTSIZE
  DW FFTSIZE
LABEL _NBUTTERFLIES
  DW NBUTTERFLIES
LABEL _SWAPS
  DW SWAPS
LABEL _BUTTERFLIES
  DW BUTTERFLIES
LABEL _input
  DW input
LABEL _slice
  DW slice
]])

Sleep(100)
