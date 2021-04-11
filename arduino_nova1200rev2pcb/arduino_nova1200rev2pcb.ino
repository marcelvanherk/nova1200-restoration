// mvh 20170801 added lcd display
// mvh 20170802 show all keys on lcd; show test hints and progress
// mvh 20170803 mini debugger shows on instruction step
// mvh 20170804 added IOT decoding and cpu control instructions
// mvh 20170820 added keyboard readout
// mvh 20170821 keyboard can examine/goto/edit(ac or memory) and step
// mvh 20170822 fix disassembler error
// mvh 20170822 code cleanup and layout
// mvh 20170823 prepare for small or large LCD; store lcd size in eeprom(0)
// mvh 20170825 start decimal mode, more work on assembly mode
// mvh 20170827 simplified and more work on mini assembler (does not do IOT instructions yet)
// mvh 20170827 added serial interface
// mvh 20170828 added : and ?, set prevpc on istep switch
// mvh 20170828 added I/O instructions to mini assembler
// mvh 20170829 added interactive mode with memory mapped serial 10=in 11=out
//              test program:
//              000000 020010 LDA0 10
//              000001 101015 MOV#00snr
//              000002 000000 JMP 0
//              000003 101400 INC00
//              000004 040011 STA0 11
//              000005 102400 SUB00
//              000006 040010 STA0 10
//              000007 000000 JMP 0
// mvh 20170829 fixed continue button (inst ff), some code cleanup
// mvh 20170830 Added IO mode allowing memory mapped IO demo without PC
//              Force Nova stop when accessing kb functions and some key functions
//              Todo: connect LCD3, solder mux to read 5 lights
// mvh 20190116 Adapted for rev2 pcb
//              Reversed polarity of bit 8 of register 10
//              Changed order of LCD pinout
//              Changed order of keypad pinout
//              Made CONTROLSWITCHES conditional
//              Add tests to keypad mode 6
// mvh 20190117 Proper return from tests
// mvh 20190120 Accelerated register read and write; added read test to tests(0)
// mvh 20190126 Reduced writeinst to 8bit; use data to test for equality in tests(0); 115200 Baud
//              Added readLights, show in regtst and with X2
//              Note: 8K dump with serial ~6s without serial < 1s
//              Added test mode 5: generate interrupt (requires wire from SEL7 to pin B29 on backplane)
// mvh 20190127 Start on interrupt; Use data register to emulate switches; I/O mode output also on switches
// mvh 20190128 Fixed continue; address was modified by examine etc, reset with examine(a)
//              Added mini-assembler and continueNovaSw() test program; to load key 9-6-7
// mvh 20190201 Added LABEL and JMPL and JSRL to assembler; made it a clean routine
//              Assumes large screen only; started on special instructions
// mvh 20190201 WIP: Serial output misses characters
// mvh 20190202 Fixed serial; wait up to 100 ms for halt, defined and disassemble system calls
//              added WRITELED system call (note messes with lcd2)
//              Added internal/external EEPROM code for READBLOCK and WRITEBLOCK
//              Started on READBLOCK and WRITEBLOCK and their test programs
// mvh 20190203 Fixed indirect; added serial r/w eeprom; move prog2 and 3 to 050 and 060 and fix for bit2
//              Added memstepNova(); 9-9=run; serial timeout to 4s
// mvh 20190203 Store data bigendian into EEPROM
// mvh 20190203 Added ADDRESS, LETTERS and LABEL assembly; added MESSAGE and test program
// mvh 20190203 Made 24LC1025 eeprom work
// mvh 20190204 EEPROM core bank 0=512 byte (fits arduino), 1-4=8kw, 5=32kw (total 128kb to fit 24LC1025)
// mvh 20190205 Fix LABEL code; show compile date; mem size on start; layout; removed superfluous &15
//              Assembly code no longer adjusted for broken D14 (value 2)
// mvh 20190206 Removed some dead code (Arduino very close to full!)
// mvh 20190207 Added '.' Serial, fix e.g. 1# and 123# on keyboard, now directly edits value
//              INFO shows standard debug information
//              Assembler: Added DW, DB, ORG, LDAL, STAL, ISZL and DSZL and AUTO label;
//              Do not output END, allow LABEL+IND
// mvh 20190208 Added GPIO, also use it to indicate READBLOCK and WRITEBLOCK
//              Started on SKIPBUSYZ and SKIPDONE
// mvh 20190212 Added DELAY syscall, changed SKIPDONE, continue after syscall for speed
//              Add multiply routine (tested at 120 us), relocated test programs
// mvh 20190302 Added #TEENSY define for TEENSY3.5, connected to rightsided 24 pins
//		with USB connector pointing to inside of board, 24 other teensy pins NC 
// mvh 20190302 Fixed type errors and warnings in Teensy compile
// mvh 20190302 Added IO instructions to mini assembler
// mvh 20190407 Teensy 3.5 functioning including SoftWire I2C
// mvh 20190414 Enable LED output; make memory tests run longer; add tests(8)
//              Push 9 long will force stop Nova
// mvh 20190422 Added interactive mode, octal with readable commands
//              Added led command, command line history, backspace
// mvh 20190423 Added auto propose next cmd, history, help and some cleanup
// mvh 20190424 Added test N, start on 'file system' in eeprom
// mvh 20190424 Examine 0 after memory sizing; remove hw interrupt
// mvh 20190425 Discard \n to make work with termux telnet
//              Added unused teensy pins and DAC to WRITELED command
//              Added serial dac and adc commands; led command extends to teensy D11 and D12
// mvh 20190426 Added .mul .div .memcpy .memclr .db
// mvh 20190501 Added .db; fixed accelerated assembler; dump shows ascii, fix readHex
//              init will clear lcdpos
// mvh 20190508 Typo in serial manual
// mvh 20190511 Added ? to serial mode to quickly read all lights
// mvh 20190512 ? will propose ?; added memory command to hex dump block
// mvh 20190602 Started coding FFT and CT reconstruction 
// mvh 20190609 $ will dump all registers in hex
// mvh 20190610 Added vis and plot command; fix memory command
// mvh 20190610 % in MESSAGE will now print signed decimal; fix warning in vis command
// mvh 20190715 Added support for serial port to Nova
// mvh 20190717 Added tape loaded tape FILENAME (max 12 characters)
// mvh 20190720 tape list, store, restore, fp xx, HALT at xxx
// mvh 20210411 Align with rev3pcb code; added tapeloader storing .basic in code; K upload

// TODO: mode to list serial reply in hex
// Do I need to keep @ as cancel for direct mode? 
// Allow longer filenames for tape command
// continue to restore serial IO
// Add echo file command to load tape as input
// Test checksum in tape loader

#define TEENSY
//Nano to Teensy (3.5 or 3.2) mapping:
//GND	GND	GND
//D2	D0	D0
//..	..	..
//D12	D10	ENLCD1
//D13	A1	ENLCD2
//A0	A4	--- (KEYP)
//A1	A5	--- 
//A2	A6	LCD4
//A3	A7	LCD5
//A4	A8	LCD6
//A5	A9	LCD7
//A6	X (3.3 V)	---
//A7	--- (AGND)	---
//5V	Vin	5V
// Teensy 3.2 pins A0, A2, A3, A14/DAC, and D11, D12, D13 are free

#include <EEPROM.h>

#ifdef TEENSY
#include <LiquidCrystalFast.h>
#else
#include <LiquidCrystal.h>
#endif

#ifdef TEENSY
  #include <AsyncDelay.h>
  #include <SoftWire.h>
  SoftWire softwarei2c(A8, A9);
  #define Wire softwarei2c
  void WireEnd() {
    Wire.end();
    pinMode(A8, OUTPUT);
    pinMode(A9, OUTPUT);
  }
  byte rxbuffer[64];
  byte txbuffer[64];
  void Wirebegin() {
    Wire.setDelay_us(1);
    Wire.setRxBuffer(rxbuffer, 64);
    Wire.setTxBuffer(txbuffer, 64);
    Wire.begin();
  }
#else
  #include <Wire.h>
  #define WireEnd Wire.end
  #define Wirebegin Wire.begin
#endif

#ifdef CONTROLSWITCHES
  LiquidCrystal lcd (6, 12, A4, A5, A2, A3);
  LiquidCrystal lcd2(6, 13, A4, A5, A2, A3); // hand wired board
#else
  #ifdef TEENSY
    LiquidCrystalFast lcd (4, 10, A6, A7, A8, A9); // nova1200rev2 teensy
    LiquidCrystalFast lcd2(4, A1, A6, A7, A8, A9);
  #else
    LiquidCrystal lcd (6, 12, A2, A3, A4, A5); // nova1200rev2
    LiquidCrystal lcd2(6, 13, A2, A3, A4, A5);
  #endif
#endif

// very mini assembler for nova
#define LDA(d, a) (020000+(d<<11)+(a&0xff))
#define STA(d, a) (040000+(d<<11)+(a&0xff))
#define JMP(a)    (000000+(a&0xff))
#define JSR(a)    (004000+(a&0xff))
#define ISZ(a)    (010000+(a&0xff))
#define DSZ(a)    (014000+(a&0xff))
#define COM(s, d) (0100000+(s<<13)+(d<<11))
#define NEG(s, d) (0100400+(s<<13)+(d<<11))
#define MOV(s, d) (0101000+(s<<13)+(d<<11))
#define INC(s, d) (0101400+(s<<13)+(d<<11))
#define ADDC(s, d)(0102000+(s<<13)+(d<<11)) // clashes with arduino's ADC
#define SUB(s, d) (0102400+(s<<13)+(d<<11))
#define ADD(s, d) (0103000+(s<<13)+(d<<11))
#define AND(s, d) (0103400+(s<<13)+(d<<11))
#define HALT       063077
#define INTEN      060177
#define INTDS      060277
#define READS(d)  (060477+(d<<11))
#define SKIP      (MOV(0,0)+NOLOAD+SKP) 

// IO instructions
#define NIO(d)    (060000+d)
#define DIA(a, d) (060400+(a<<13)+d)
#define DOA(a, d) (061000+(a<<13)+d)
#define DIB(a, d) (061400+(a<<13)+d)
#define DOB(a, d) (062000+(a<<13)+d)
#define DIC(a, d) (062400+(a<<13)+d)
#define DOC(a, d) (063000+(a<<13)+d)
#define SKPBN(d)  (063400+d)
#define SKPBZ(d)  (063500+d)
#define SKPDN(d)  (063600+d)
#define SKPDZ(d)  (063700+d)

// function code to add to IO instructions
#define IOS 0100
#define IOC 0200
#define IOP 0300

// relative addressing for a, e.g. STA(0, 10)+AC2 or STA(0, 10)+IND or JMP(0)+AC3
#define PC  00400
#define AC2 01000
#define AC3 01400
#define IND 02000

// carry for with arithmetic instructions e.g. ADD(0,0)+CO
#define CZ 020 // zero
#define CO 040 // one
#define CC 060 // complement

// shift for with arithmetic instructions e.g. ADD(0,0)+SL
#define SL 0100 // left
#define SR 0200 // right
#define SS 0300 // swap bytes

// skip for with arithmetic instructions ADD(0,0)+SKP
#define SKP 1 // always skip
#define SZC 2
#define SNC 3
#define SZR 4
#define SNR 5
#define SEZ 6
#define SBN 7

// noload for with arithmetic instructions ADD(0,0)+NOLOAD
#define NOLOAD 010

// any constant in output
#define CONSTANT(a) ((a)&0xffff)
#define DW(a)       ((a)&0xffff)
#define LETTERS(a, b) (a*256+b)
#define DB(a, b)      (a*256+b)

// use rarely used instructions to code specials
#define ORG(a)     060777,a // Locate next code at address a
#define END        067777   

// label mechanism (labels 0..19 supported, or AUTO+n)
#define LABEL(a)   061777,a // store label[a]

#define ADDRESS(a) 062777,a // Output address of label[a] as constant
#define JMPL(a)    063770,a // JMP to label[a]
#define JSRL(a)    063771,a // JSR to label[a]
#define ISZL(a)    063772,a // ISZ label[a]
#define DSZL(a)    063773,a // DSZ label[a]
#define LDAL(r, a) 064770+(r),a // LDA r from label[a]
#define STAL(r, a) 064774+(r),a // STA r to label[a]

#define AUTO       01000   // AUTO, AUTO+1: automatic variables, only allowed in LABEL instructions, also +IND is allowed

// Addresses for memory mapped I/O
#define CHARIN 040   // input to nova
#define CHAROUT 042  // output from nova

// System calls (these all are functional HALT instructions, interpreted by Arduino program)
#define INFO       077377 // show debug information on LCD
#define PUTC       077277 // output character to LCD and serial
#define GETC       077177 // get character from serial (/ mode) or keypad (9-5 mode)
#define READBLOCK  077077 // read 64 word block A0 from device (EEPROM) to address A2
#define WRITEBLOCK 073377 // write 64 word block A0 to device (EEPROM) from address A2
#define WRITELED   073277 // A0 bit 0 sets Arduino LED
#define MESSAGE    073177 // Write text string with %0..%3 substituted for AC0..3
#define GPIO       073077 // High bits define data direction (O=out), low bits value
#define SKIPBUSYZ  (MOV(3,3)+NOLOAD+SKP) // Always skip
#define SKIPDONE   067377 // skip when character available
#define DELAY      067277 // Delay A0 ms, showing A1 on lights
#define ADC        067177 // Read A0 from teensy
// 6 are open 06[3,7][1-3]77 except 063077; maybe add 
// HWMUL, HWDIV, DEVICESEL, SKIPBUSYZ (always skip)

// test assembly program for switch based input
unsigned short prog1[]={
  JMP(2),
  JMP(2),
  LABEL(0),
  INC(1,1),        // keep track if loop and halt works
  //HALT,
  //READS(0),        // read input data from switches
  GETC,
  INC(1,1),        // keep track if loop and halt works
  GPIO,
  // STA(0, CHAROUT), // echo to arduino
  INTEN,           // ION light ON
  //WRITELED,

  LDA(2, 2)+PC,    // delay: load constant
  SKIP,            // skip constant
  CONSTANT(65532),
  INC(2,2)+SZR,    // loop 65535 times
  JMP(-1)+PC,
  INTDS,           // ION light OFF
  
  PUTC,           // print A0 on LCD
  //HALT,

  /*
  // CONSTANT(077377), // displays A0 and A1 on LCD

  LDA(2, 2)+PC,    // delay
  SKIP,
  CONSTANT(65530),
  INC(2,2)+SZR,
  JMP(-1)+PC,
  INTEN,          // ION light ON
  
  LDA(2, 2)+PC,   // delay
  SKIP,
  CONSTANT(65530),
  INC(2,2)+SZR,
  JMP(-1)+PC,
*/
  INTDS,          // ION light OFF

  JMPL(0),
  END
};

enum {MULTA=2, MULT=3, MULTADD=10, MULTADD1, MULTSUB, MULTSUB1, BUTTERFLY, STRETCH, const128};

#define ARG 4 // used for arithmatic routines
#define RE  5 // used for arithmatic routines
#define IM  6 // used for arithmatic routines
#define LEFTP  7 // used for arithmatic routines
#define RIGHTP 8 // used for arithmatic routines
#define dx 9 // used for arithmatic routines
#define dx1 10 // used for arithmatic routines
#define counter 11 // used for arithmatic routines

// butterfly table access: AC2[0]=SIN, AC2[1]=COS, AC2[2]=LEFTP, AC2[3]=RIGHTP

unsigned short prog2[]={
  SUB(0, 0),
  SUB(2, 2),
  WRITEBLOCK,
  SUB(2, 2),
  LABEL(1), /// loop
  JSRL(2),
  INC(2,2)+SZR,
  JMPL(1),
  HALT,

  // AC0/1 = AC1*AC2
  LABEL(MULT),   // MULTIPLY
  SUB(0, 0),
  // AC0/1 = AC0+AC1*AC2
  LABEL(MULTA),   // MULTIPLY AND ADD
  STAL(3, AUTO),
  LDAL(3, MULT+2), // -16
  LABEL(MULT+1),   // LOOP
  MOV(1, 1)+SR+SNC,
  MOV(0, 0)+SR+SKP,
  ADD(2, 0)+SR+CZ,
  INC(3, 3)+SZR,
  JMPL(MULT+1),
  MOV(1, 1)+SR,
  JMPL(AUTO+IND), // RETURN
  HALT,
  LABEL(MULT+2),
  CONSTANT(-16),

  // AC0/1 += AC2[0]*ARG
  LABEL(MULTADD),   // MULTIPLY AND ADD
  STAL(3, AUTO+1),
  STAL(0, AUTO+2),
  STAL(1, AUTO+3),
  STAL(2, AUTO+4),
  LDA(1, 0)+AC2,
  LDA(2, ARG),
  JSRL(MULT),
  LDAL(2, AUTO+2),
  LDAL(3, AUTO+3),
  ADD(3, 1)+CZ+SZC,
  INC(2, 2),
  ADD(2, 0),
  LDAL(2, AUTO+2),
  JMPL(AUTO+1+IND), // RETURN

  // AC0/1 += AC2[1]*ARG
  LABEL(MULTADD1),   // MULTIPLY AND ADD 1
  STAL(3, AUTO),
  STAL(0, AUTO+1),
  STAL(1, AUTO+2),
  STAL(2, AUTO+3),
  LDA(1, 1)+AC2,
  LDA(2, ARG),
  JSRL(MULT),
  LDAL(2, AUTO+1),
  LDAL(3, AUTO+2),
  ADD(3, 1)+CZ+SZC,
  INC(2, 2),
  ADD(2, 0),
  LDAL(2, AUTO+1),
  JMPL(AUTO+IND), // RETURN

  // AC0/1 -= AC2[0]*ARG
  LABEL(MULTSUB),   // MULTIPLY AND SUBTRACT
  STAL(3, AUTO),
  STAL(0, AUTO+1),
  STAL(1, AUTO+2),
  STAL(2, AUTO+3),
  LDA(1, 0)+AC2,
  LDA(2, ARG),
  JSRL(MULT),
  LDAL(2, AUTO+1),
  LDAL(3, AUTO+2),
  SUB(3, 1)+CZ+SZC,
  SUB(2, 0)+SKP,
  ADDC(2, 0),
  LDAL(2, AUTO+1),
  JMPL(AUTO+IND), // RETURN

  // AC0/1 -= AC2[1]*ARG
  LABEL(MULTSUB1),   // MULTIPLY AND SUBTRACT 1
  STAL(3, AUTO),
  STAL(0, AUTO+1),
  STAL(1, AUTO+2),
  STAL(2, AUTO+3),
  LDA(1, 1)+AC2,
  LDA(2, ARG),
  JSRL(MULT),
  LDAL(2, AUTO+1),
  LDAL(3, AUTO+2),
  SUB(3, 1)+CZ+SZC,
  SUB(2, 0)+SKP,
  ADDC(2, 0),
  LDAL(2, AUTO+1),
  JMPL(AUTO+IND), // RETURN

  // BUTTERFLY, AC2 POINTS to FFT TABLE
  LABEL(BUTTERFLY),
  STAL(3, AUTO),

  // RE = cos*array[right]-sin*array[right+1]
  LDA(3, 1)+AC2, //cos
  STA(3, ARG),
  SUB(0, 0),
  SUB(1, 1),
  INC(2, 2),
  INC(2, 2),
  INC(2, 2), // AC2 points to right
  JSRL(MULTADD),
  LDA(3, -3)+AC2, //sin
  STA(3, ARG),
  JSRL(MULTSUB1),
  STA(1, RE),

  // IM = sin*array[right]+cos*array[right+1]
  SUB(0, 0),
  SUB(1, 1),
  JSRL(MULTADD),
  LDA(3, -2)+AC2, //cos
  STA(3, ARG),
  JSRL(MULTADD1),
  STA(1, IM),

  NEG(2, 3),
  INC(3, 3),
  NEG(3, 3), // AC3 points to left

  // array[right] = array[left]-re
  LDA(0, 0)+AC3,
  LDA(1, RE),
  SUB(1, 0),
  STA(0, 0)+AC2,
  
  // array[right+1] = array[left+1]-im
  LDA(0, 1)+AC3,
  LDA(1, IM),
  SUB(1, 0),
  STA(0, 1)+AC2,
  
  // array[left] = array[left]+re
  LDA(0, 0)+AC2,
  LDA(1, RE),
  ADD(1, 0),
  STA(0, 0)+AC2,
  
  // array[left+1] = array[left+1]+im
  LDA(0, 1)+AC2,
  LDA(1, IM),
  ADD(1, 0),
  STA(0, 1)+AC2,

  INC(2, 2),    // AC2 points to next butterfly
  JMPL(AUTO+IND), // RETURN from BUTTERFLY


  // AC2[i] = ARG[j], where j= I*scale+offset
  LABEL(STRETCH),
  STAL(3, AUTO),
  LDA(3, const128),
  NEG(3, 3),
  STA(3, counter),
  LABEL(STRETCH+1),
  LDA(3, ARG),
  ADD(3, 1),
  LDA(3, 0)+AC3,
  STA(2, 0)+AC2,
  INC(2, 2),
  LDA(3, dx),
  ADD(3, 0)+CZ+SZC,
  INC(1, 1),
  LDA(3, dx1),
  ADD(3, 1),
  ISZ(counter),
  JMPL(STRETCH+1),
  JMPL(AUTO+IND), // RETURN from STRETCH

  LABEL(const128),
  DW(128),
  
  END
};

unsigned short prog3[]={
  SUB(0, 0),
  LDA(2, 2)+PC,    
  SKIP,
  CONSTANT(0100),
  READBLOCK,
  HALT,
  END
};

unsigned short prog4[]={
  LABEL(3),
  LDAL(0, 2), 
  LDA(2, 2)+PC,    
  SKIP,
  ADDRESS(0),
  MESSAGE,
  DELAY,
  INC(1,1),
  MOV(1,0),
  GPIO,
  JMPL(3),
  LABEL(2),
  CONSTANT(500),
  LABEL(0),
  LETTERS('H', 'e'),
  LETTERS('l', 'l'),
  LETTERS('o', 0),
  // assembler tests
  //LABEL(1),
  //JMPL(1),
  //ISZL(1),
  //LDAL(2, 1),
  //STAL(2, 1),
  //STAL(2, AUTO),
  //STAL(2, AUTO+IND),
  END
};

// assemble unsigned short array as above to location 'to' in Nova; interpreting labels
void assemble(int to, unsigned short *prog)
{ unsigned int i=0, j=to, a, Auto;
  unsigned short labels[200];
  
  // 2 pass assembler; pass 1 collects labels only
  do
  { a=prog[i];
    if (a==060777) // ORG(a)
    { j=prog[i+1];
      i++;
    }
    else if (a==061777) // define label: 2 input items, no output 
    { labels[prog[i+1]]=j;
      i++;
    }
    else if (a==062777 || (a>=063770 && a<=063773) || (a>=064770 && a<=064777)) // use label: 2 input, 1 output
    { // if (prog[i+1]>=Auto) nAuto++;
      i++;
      j++;
    }
    else if (a!=END)
      j++;
    i++;
  }
  while (a!=END);
  Auto=j;
  // Internal = Auto+nAuto;

  // pass 2; outputs code
  i=0; j=to;
  do
  { a=prog[i];
    if (a==060777) // org(a)
    { j=prog[i+1];
      i++;
    }
    else if (a==061777) // ignore label definition
      i++;
    else if (a==062777)  // output label addresss
    { deposit(j++, labels[prog[i+1]]);
      i++;
    }
    else if ((a>=063770 && a<=063773) || (a>=064770 && a<=064777)) // relative or page 0 jumps, isz/dsz, load or store
    { int target;
      unsigned int Ind = prog[i+1]&IND;
      if (prog[i+1]>=AUTO) target=Auto+  (prog[i+1]&(AUTO-1)); // e.g. STAL(0, AUTO))
      else                 target=labels[(prog[i+1]&(AUTO-1))];

      if (target<256)                // page 0
      { target = target+Ind;
        if (a>=063770 && a<=063773) deposit(j++,        04000*(a&3)+target);
        else                        deposit(j++, 020000+04000*(a&7)+target);
      }                             // PC relative
      else if (target-(signed)j>-127 && target-(signed)j<127)
      { target = target-(signed)j;
        target = (target & 255) + Ind;
        if (a>=063770 && a<=063773) deposit(j++,   0400+04000*(a&3)+target);
        else                        deposit(j++, 020400+04000*(a&7)+target);
      }                             // out of range
      else
      { lcdprint("RANGE");
        //lcdprint(" @");
        //lcdprint(j, 8);
        //lcdprint(" to ");
        //lcdprint(target, 8);
        deposit(j++, 017777);
        delay(1000);
        return;
      }
      i++;
    }
    else if (a!=END)
      deposit(j++, a);
    i++;
  }
  while (a!=END);
}

/* list of interface registers (4 bits)

board 1
0 = data L (bits 12-15 nova, note bit 15 is LSB!)
1 = data H (bits 8-11 nova)
2 = inst l (bits 12-15 instruction)
3 = inst H (bits 8-11 instruction)
4 = data L 
5 = data H 
6 = spare
7 = spare 
20 = read addr L
21 = read addr H

board 2
8 = data L (bits 4-7 nova)
9 = data H (bits 0-3 nova)
10 = ctl H
11 = ctl L
12 = read data L 
13 = read data H 
14 = inst L (bits 4-7 instruction)
15 = inst H (bits 0-3 instruction)
28 = read addr L 
29 = read addr H

// signals on control registers 10 and 11, *means active low
// 10 1 = ISTP*
// 10 2 = PL*
// 10 4 = RESET*
// 10 8 = 1=keys 0=data connected to key request from nova
// 11 1 = MSTP*
// 11 2 = CON+*
// 11 4 = CONREQ*
// 11 8 = STOP*

// read status lights & aux inputs (reg 22 and 6)
// 1=RUN
// 2=FETCH
// 4=EXEC
// 8=AUX
// 16=ION
// 32=DEFER
*/

void wait(void) {
  delayMicroseconds(2);
}

#ifdef TEENSY
void WriteReg(int n, int value) {
  if (n==10) {
    value^=7;        // reverse polarity of enregsw
    value&=7;        // force switches off (to allow switch emulation)
  }
  else
    value=~value;      // negate value to make active high

  digitalWrite(0, value&1);		// output 4bit data
  digitalWrite(1, (value>>1)&1);
  digitalWrite(2, (value>>2)&1);
  digitalWrite(3, (value>>3)&1);
  
  digitalWrite(4, n&1);			// output 4bits address (3 bits used)
  digitalWrite(5, (n>>1)&1);
  digitalWrite(8, (n>>2)&1);
  digitalWrite(9, 0); // A0'

  if (n&8) {				// pulse enable on one of the boards
    digitalWrite(7, 1);
    delayMicroseconds(1);
    digitalWrite(7, 0);
    delayMicroseconds(1);
    digitalWrite(7, 1);
  }
  else {
    digitalWrite(6, 1);
    delayMicroseconds(1);
    digitalWrite(6, 0);
    delayMicroseconds(1);
    digitalWrite(6, 1);
  }
}
#else
void WriteReg(byte n, byte value) {
#ifdef CONTROLSWITCHES
  value=~value;				// negate value to make active high
#else
  if (n==10) {
    value^=7;        // reverse polarity of enregsw
    value&=7;        // force switches off (to allow switch emulation)
  }
  else
    value=~value;      // negate value to make active high
#endif

  PORTD = (PORTD&3)|((value&15)<<2)|((n&3)<<6);
  byte s=(PORTB&0xF0)|(n&4);
  PORTB = s|3; 
  if (n&8)
  { PORTB = s|1;
    PORTB = s|3; 
  }
  else
  { PORTB = s|2;
    PORTB = s|3; 
  }
}
#endif

unsigned short memsize=0;
int lcdpos=0;
bool clearline=false;

void setup() {

#ifdef TEENSY
  for (byte i=0; i<=10; i++) 
    pinMode(i, OUTPUT);
  pinMode(A1, OUTPUT); 
  digitalWrite(7, 1);     // disable both board enables
  digitalWrite(6, 1);
  pinMode(13, OUTPUT); // led
  pinMode(12, OUTPUT); // free pins
  pinMode(11, OUTPUT);
  analogWriteResolution(12);
#else
  for (byte i=2; i<=11; i++) 
    pinMode(i, OUTPUT);
  digitalWrite(9, 1);     // disable both board enables
  digitalWrite(8, 1);
#endif

#ifdef CONTROLSWITCHES
  pinMode(A0, INPUT);     // control switches are connected via register network to 2 analog inputs 
  pinMode(A1, INPUT);
#endif

  WriteReg(10, 8);      // disable all ctl signals, set switch mode (if switches wired)
  WriteReg(11, 0);

#ifdef TEENSY
  pinMode(A4, INPUT);     // keypad is readout using register network on Y line and LCD4-7 on X wires
  Serial2.setTX(31);
  Serial2.setRX(26);
  Serial2.begin(4800,SERIAL_8N1);
#endif

  lcdpos=0;
  clearline=false;
  lcd.begin(40, 2);   // welcome text on LCD
  lcdsetCursor(0, 0);
  lcdprint(F("Nova panel - Marcel van Herk " __DATE__));

  for (byte i=0; i<16; i++)
  { unsigned int a=i*2048-1;
    if (examine(a)==0)
    { deposit(a, 1);
      if (examine(a)==1) memsize=a+1;
      deposit(a, 0);
    }
    else
      memsize=a+1;
  }
  lcdsetCursor(0, 1);
  lcdprint(F("Nova memory: "));
  lcdprint(memsize);
  lcdprint(F(" 16-bit words"));
  examine(0);
  
  lcd2.begin(40, 2);    // welcome text on LCD
  lcd2.setCursor(0, 1);
  lcd2.print(F("DATA GENERAL CORPORATION       NOVA 1210"));
  printHelp(F("This computer was manufactured in 1971"));

  Serial.begin(115200);     // usb serial for debug output
  Serial.setTimeout(5000);
}

void writeData(int d)    // set data register
{ WriteReg(0, d&15);     // data L
  WriteReg(1, (d>>4));   // data H
  WriteReg(8, (d>>8));   // data L brd2
  WriteReg(9, (d>>12));  // data H brd2
}

void writeInst(int i)    // set instruction register (only 8 MSB bits used, reg 2 and 3 ignored)
{ WriteReg(14, i);       // data L brd2
  WriteReg(15, (i>>4));  // data H brd2
}

#ifdef TEENSY
unsigned int readReg(int n) {
  int res;
  pinMode(0, INPUT);		// change bus to input
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  digitalWrite(4, n&1);		// write 4 bit address
  digitalWrite(5, (n>>1)&1);
  digitalWrite(8, (n>>2)&1);
  digitalWrite(9, (n>>4)&1); // A0'
  digitalWrite(6, 1);
  digitalWrite(7, 1);		// make sure all boards are disabled

  if (n&8) {			// read from board 2
    digitalWrite(7, 1);
    digitalWrite(7, 0);
    delayMicroseconds(1);
    res = 15-(digitalRead(0)|(digitalRead(1)<<1)|(digitalRead(2)<<2)|(digitalRead(3)<<3));
    digitalWrite(7, 1);
  }
  else {			// read from board 1
    digitalWrite(6, 1);
    digitalWrite(6, 0);
    delayMicroseconds(1);
    res = 15-(digitalRead(0)|(digitalRead(1)<<1)|(digitalRead(2)<<2)|(digitalRead(3)<<3));
    digitalWrite(6, 1);
  }

  pinMode(0, OUTPUT);		// back to output
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  return res;
}
#else
unsigned int readReg(byte n) {
  int res;

  DDRD &= 0xc3;
  PORTD = (PORTD&0x3f)|((n&3)<<6);
  byte s=(PORTB&0xF0)|(n&0x04)|((n>>1)&0x08);
  PORTB = s|3;
  if (n&8)
  { PORTB = s|1;
    wait();
    res = ((PIND>>2)&0x0f)^0x0f;
    PORTB = s|3; 
  }
  else
  { PORTB = s|2;
    wait();
    res = ((PIND>>2)&0x0f)^0x0f;
    PORTB = s|3; 
  }
  DDRD |= 0x3c;
  
  return res;
}
#endif

// read data lights
unsigned int readData()
{ unsigned int a = (readReg(13)<<12)+(readReg(12)<<8)+(readReg(5)<<4)+readReg(4);
  return a;
}

// read address lights, high bit is connected to carry light
unsigned int readAddr()
{ unsigned int a = (readReg(29)<<12)+(readReg(28)<<8)+(readReg(21)<<4)+readReg(20);
  return a;
}

// read status lights
unsigned int readLights()
{ unsigned int a = (readReg(22)<<4)+readReg(6);
  return a;
}

unsigned int CurrentSwitchValue=0;

// read nova accumulator 0..3
unsigned int examineAC(int address) {
  writeInst(0x67+(address<<3));
  WriteReg(11, 4);
  wait();
  WriteReg(11, 0);
  wait();
  int v = readData();
  return v;
}

// read single word from nova memory or register
unsigned int examine(int address) {
  if (address>=0xfffc)
    return examineAC(address-0xfffc);
  writeInst(0xf9);
  writeData(address);
  WriteReg(10, 0);	// connect data register to bus
  WriteReg(11, 4);	// pulse CONREQ
  wait();
  WriteReg(11, 0);	// revert to idle state
  WriteReg(10, 8);
  wait();
  int v = readData();	// read lights
  writeData(CurrentSwitchValue);
  return v;
}

// write single word to nova accumulator
void depositAC(unsigned int address, unsigned int value) {
  writeInst(0x23+(address<<3));
  writeData(value);
  WriteReg(10, 0);
  WriteReg(11, 4);
  wait();
  WriteReg(11, 0);
  wait();
  WriteReg(10, 8);
  writeData(CurrentSwitchValue);
}

// write single word to nova memory or accumulator
void deposit(unsigned int address, unsigned int value) {
  if (address>=0xfffc) 
  { depositAC(address-0xfffc, value);
    return;
  }
  writeInst(0xf9);
  writeData(address);
  WriteReg(10, 0);
  WriteReg(11, 4);
  wait();
  WriteReg(11, 0);
  wait();
  writeInst(0xdd);
  writeData(value);
  WriteReg(11, 4);
  wait();
  WriteReg(11, 0);
  wait();
  WriteReg(10, 8);
  writeData(CurrentSwitchValue);
}

bool novaRunning=false;

unsigned int stopNova(void)
{ WriteReg(11, 8);
  wait();
  WriteReg(11, 0);
  wait();
  novaRunning=false;
  return readAddr();
}

// continue
// note: examine and deposit affect continue address
void continueNova(void)
{ writeInst(0xfb);
  WriteReg(11, 6);
  WriteReg(11, 0);
  wait();
  novaRunning=true;
}              

// continue but pass value of sw to switch register (disabling hardware switches)
// note: examine and deposit affect continue address, restore it by a=readAddr() .... examine(a); continueNovaSw(int sw)
void continueNovaSw(int sw)
{ CurrentSwitchValue=sw;
  writeData(CurrentSwitchValue);
  WriteReg(10, 0);
  wait();
  writeInst(0xfb);
  WriteReg(11, 6);
  WriteReg(11, 0);
  wait();
  WriteReg(10, 8);
  novaRunning=true;
}              

// start at address a
void startNova(unsigned int a)
{ writeData(a);
  writeInst(0xfb);
  WriteReg(11, 4);
  wait();
  WriteReg(11, 0);
  wait();
  writeData(CurrentSwitchValue);
  novaRunning=true;
}              

unsigned int prevpc=0;              // used when stepping back in debugging
void stepNova(void)
{ prevpc=readAddr();
  writeInst(0xff);
  WriteReg(10, 9);
  WriteReg(11, 6);
  wait();
  Serial.println(F("instruction step"));
  WriteReg(11, 0);
  WriteReg(10, 8);
}

void memstepNova(void)
{ writeInst(0xff);
  WriteReg(11, 7);
  wait();
  WriteReg(11, 0);
}

void resetNova(void)
{ stopNova();
  WriteReg(10, 12);
  WriteReg(11, 6);
  delay(100);
  Serial.println("reset");
  WriteReg(11, 0);
  WriteReg(10, 8);
  lcdsetCursor(0,1);
  lcdprint(makespc(40));
  lcdsetCursor(0,1);
  lcdprint("reset"); 
  novaRunning=false;
  examine(0);
}

// make string of given number of spaces
String makespc(int n)
{ String s;
  for (int i=0; i<n; i++) s.concat(" ");
  return s;
}

void lcdprint(String text)
{ lcd.print(text);
}

void lcdwrite(uint8_t c)
{ lcd.write(c);  
}

void lcdsetCursor(int x, int y)
{ lcd.setCursor(x,y);
}

void lcdcursor(void)
{ lcd.cursor();
}

void lcdnoCursor(void)
{ lcd.noCursor();
}

void lcdclear(void)
{ lcd.clear();
}

void lcdclearline(int y)
{ lcd.setCursor(0, y);
  lcd.print(makespc(40));
}

// make hex string with leading zeros
String toHex(unsigned int v) {
  String s;
  if (v<0x1000) s += "0";
  if (v<0x0100) s += "0";
  if (v<0x0010) s += "0";
  s += String(v, HEX);
  return s;
}

// make hex string with leading zeros
String toHex2(unsigned int v) {
  String s;
  if (v<0x0010) s += "0";
  s += String(v, HEX);
  return s;
}

// make octal string with leading zeros
String toOct(unsigned int v) {
  String s;
  if (v<0x8000) s += "0";
  if (v<0x1000) s += "0";
  if (v<0x0200) s += "0";
  if (v<0x0040) s += "0";
  if (v<0x0008) s += "0";
  s += String(v, OCT);
  return s;
}

// convert n digit hex string in buffer
unsigned int readHex(char *buf, byte n)
{ unsigned int r=0;
  for (int i=0; i<n; i++)
  { r = r<<4;
    if (buf[i]>='a') r = r+buf[i]-'a'+10;
    else if (buf[i]>='A') r = r+buf[i]-'A'+10;
    else r = r+buf[i]-'0';
  }
  return r;
}

// read octal string in buffer
unsigned int readOct(String s, int pos)
{ unsigned int r=0;
  for (unsigned int i=pos; i<s.length(); i++)
  { if (s[i]<'0' || s[i]>'7') break;
    r = r<<3;
    r = r+s[i]-'0';
  }
  return r;
}

////////////////////////////////////////////////////
// print octal on LCD with leading zeros
void lcdPrintOctal(unsigned int v) {
 lcdprint(toOct(v));
}

// print octal on LCD with leading zeros followed by space and two ascii characters
void lcdPrintOctalAscii(unsigned int v) {
 lcdPrintOctal(v);
 lcdprint(char(32));
 lcdprint(char(v>>8));
 lcdprint(char(v&255));
}

// print hex on LCD with leading zeros
void lcdPrintHex(unsigned int v) {
 lcdprint(toHex(v));
}

// print decimal on LCD with leading zeros
void lcdPrintDecimal(unsigned int v) {
 if (v<10000) lcdprint(0);
 if (v<1000) lcdprint(0);
 if (v<100) lcdprint(0);
 if (v<10) lcdprint(0);
 lcdprint(v);
}

// print long octal (21 bits) on LCD with leading zeros
void lcdPrintLongOctal(unsigned long v) {
 if (v<0x40000) lcdprint(0);
 if (v<0x08000) lcdprint(0);
 if (v<0x01000) lcdprint(0);
 if (v<0x00200) lcdprint(0);
 if (v<0x00040) lcdprint(0);
 if (v<0x00008) lcdprint(0);
 lcdprint(String(v, OCT));
}

// print help string on 3rd line of big LCD
void printHelp(String a)
{ lcd2.setCursor(0, 0);
  lcd2.print(makespc(40));
  lcd2.setCursor(0, 0);
  lcd2.print(a);
}

/////////////////////// disassembler ////////////////////////
// part of disassembler - print shift in operate instruction
String printShift(unsigned int s) {
  if (s==1) return String("L");
  else if (s==2) return String("R");
  else if (s==3) return String("S"); // byte swap
  else return("");
}

// part of disassembler - print carry op in operate instruction
String printCarry(unsigned int s) {
  if (s==1) return String("Z");
  else if (s==2) return String("O");
  else if (s==3) return String("C");
  else return String("");
}

// part of disassembler - print noload in operate instruction
String printNoload(unsigned int s) {
  if (s==1) return String("#");
  else return String("");
}

// part of disassembler - print skip part of operate instruction
String printSkip(unsigned int s) {
  if (s==1) return String("skp");
  if (s==2) return String("szc");
  if (s==3) return String("snc");
  if (s==4) return String("szr");
  if (s==5) return String("snr");
  if (s==6) return String("sez");
  if (s==7) return String("sbn");
  return String("");
}

// part of disassembler - print target of memory access instruction
String printTarget(unsigned int mode, unsigned int disp, int octalmode) {
  String s=" ";
  if (mode==0) { s += String(disp&255, octalmode); }
  if (mode==1) s += ("pc"); 
  if (mode==2) s += ("ac2"); 
  if (mode==3) s += ("ac3");
  if (mode>=4) s += ("@");
  if (mode==4) { s += String(disp&255, octalmode); }
  if (mode==5) s += ("pc"); 
  if (mode==6) s += ("ac2"); 
  if (mode==7) s += ("ac3");
  if (mode==1 or mode==2 or mode==3 or mode==5 or mode==6 or mode==7) 
  { if (disp&0x80)
    { s += ('-'); s += String(128-(disp&127), octalmode); }
    else
    { s += ('+'); s += String(disp&127, octalmode); }
  }
  return s;
}

// part of disassembler - print transfer and control in IOT instruction
String printIOT(unsigned int tr, unsigned int sk) {
  String s = "";
  if (tr==0) s += ("NIO");
  if (tr==1) s += ("DIA");
  if (tr==2) s += ("DOA");
  if (tr==3) s += ("DIB");
  if (tr==4) s += ("DOB");
  if (tr==5) s += ("DIC");
  if (tr==6) s += ("DOC");
  if (tr==7)
  { s += ("SKP");
    if (sk==0) s += ("BN");
    if (sk==1) s += ("BZ");
    if (sk==2) s += ("DN");
    if (sk==3) s += ("DZ");
  }
  else	// control part
  { if (sk==0) s += ("");
    if (sk==1) s += ("S");
    if (sk==2) s += ("C");
    if (sk==3) s += ("P");
  }
  return s;
}

// Disassemble entire instruction and return
String printDisas(unsigned int v, int octalmode) {
  String s(" ");
  unsigned int mref = (v>>11)&0x1f;
  unsigned int opcode = (v>>8)&0x7;

  // system calls
       if (v==GETC)             s+=(".GETC");  
  else if (v==PUTC)             s+=(".PUTC");  
  else if (v==INFO)             s+=(".INFO");  
  else if (v==WRITELED)         s+=(".WRITELED");
  else if (v==READBLOCK)        s+=(".READBLOCK");
  else if (v==WRITEBLOCK)       s+=(".WRITEBLOCK");
  else if (v==MESSAGE)          s+=(".MESSAGE");
  else if (v==GPIO)             s+=(".GPIO");
  else if (v==SKIPBUSYZ)        s+=(".SKIPBUSYZ");
  else if (v==DELAY)            s+=(".DELAY");
  else if (v==ADC)              s+=(".ADC");
  else if (v==HALT)             s+=("HALT");    //doc0, 4 bits free
  else if ((v&0xe73f)==0x663f)  s+=(".HALT");   // unused system call

  // cpu control instructions
  else if ((v&0xff3f)==0x653f)  s+=("IORST"); //dicc0
  else if ((v&0xe73f)==0x613f) {s+=("READS");s+=String(mref&3);}//dia#
  else if (v==0x607f) s+=("INTEN"); // nios0
  else if (v==0x60bf) s+=("INTDS"); // nioc0
  else if (v==0x673f) s+=("SKPBN CPU");
  else if (v==0x677f) s+=("SKPBZ CPU");
  else if (v==0x67bf) s+=("SKPDN CPU");
  else if (v==0x67ff) s+=("SKPDZ CPU");
  else if ((v&0xe73f)==0x643f) {s+=("MSKO");s+=String(mref&3);}//dob#
  else if ((v&0xe73f)==0x633f) {s+=("INTA");s+=String(mref&3);}//dib#

  // data access instructions
  else if (mref==0) { s+=("JMP"); s+=printTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if (mref==1) { s+=("JMS"); s+=printTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if (mref==2) { s+=("ISZ"); s+=printTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if (mref==3) { s+=("DSZ"); s+=printTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if ((mref&0x1c)==4) { s+=("LDA"); s+=String(mref&3); s+=printTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if ((mref&0x1c)==8) { s+=("STA"); s+=String(mref&3); s+=printTarget((v&0x700)>>8, v&0xff, octalmode); }

  // IOT instructions
  else if ((mref&0x1c)==12)
  { s+=printIOT((v&0x700)>>8, (v&0xc0)>>6); 
    s+=String(mref&3); 
    s+=(" "); 
    s+=String(v&0x3f,octalmode); 
  }

  // operate instructions
  else if ((mref&0x10)==16)
  { if (opcode==0)  s+=("CMP");
    if (opcode==1)  s+=("NEG");
    if (opcode==2)  s+=("MOV");
    if (opcode==3)  s+=("INC");
    if (opcode==4)  s+=("ADC");
    if (opcode==5)  s+=("SUB");
    if (opcode==6)  s+=("ADD");
    if (opcode==7)  s+=("AND");
    s+=printNoload((v&0x8)>>3);
    s+=String((mref&12)>>2); // source
    s+=String(mref&3);	     // destination
    s+=printShift((v&0xc0)>>6);
    s+=printCarry((v&0x30)>>4);
    s+=printSkip(v&7);
  }
  else 
  { s+=('?'); s+=String(v); } // unknown instruction

  return s;
}

// go from 16 bits to 21 bits mini assembler instruction format
unsigned long unpackassemble(unsigned int v)
{ unsigned int mref = (v>>11)&0x1f;
  unsigned long opcode = (v>>8)&0x7;
  unsigned long insttype;
  unsigned long op=0;
  unsigned long a=0;
  unsigned long b=0;
  unsigned long dest=0;

  if      ((v&0xff3f)==0x653f)  { insttype=1; op=0;}
  else if ((v&0xe73f)==0x663f)  { insttype=1; op=1;}
  else if ((v&0xe73f)==0x613f)  { insttype=1; op=2; a=mref&3; }
  else if (v==0x607f)           { insttype=1; op=3;}
  else if (v==0x60bf)           { insttype=1; op=4;}
  else if ((v&0xe73f)==0x643f)  { insttype=1; op=5; a=mref&3; }
  else if ((v&0xe73f)==0x633f)  { insttype=1; op=6; a=mref&3 ;}

  if      (mref==0)        { insttype=2; op=mref;   b=(v&0x700)>>8; dest=v&0xff; } //jmp`
  else if (mref==1)        { insttype=2; op=mref;   b=(v&0x700)>>8; dest=v&0xff; } //jms
  else if (mref==2)        { insttype=4; op=mref-2; b=(v&0x700)>>8, dest=v&0xff; } // isz
  else if (mref==3)        { insttype=4; op=mref-2; b=(v&0x700)>>8, dest=v&0xff; } // dsz
  else if ((mref&0x1c)==4) { insttype=3; op = ((mref&0x1c)-4)/4; a=mref&3; b=(v&0x700)>>8; dest= v&0xff;} // lda
  else if ((mref&0x1c)==8) { insttype=3; op = ((mref&0x1c)-4)/4; a=mref&3; b=(v&0x700)>>8; dest= v&0xff;} // sta
  else if ((mref&0x10)==16)
  { insttype=5;
    a = (mref&12)>>2;
    b = mref&3;
    unsigned long noload = (v&0x8)>>3;
    unsigned long shift = (v&0xc0)>>6;
    unsigned long carry = (v&0x30)>>4;
    unsigned long skip = v&7;
    return (insttype<<18)+(opcode<<15)+(a<<12)+(b<<9)+(shift<<6)+(noload<<5)+(carry<<3)+skip;
  }
  else if ((mref&0x1c)==12)
  { op   = (v&0x700)>>8;
    a    = (v&0x1800)>>11; 
    b    = (v&0xc0)>>6; 
    dest =  v&0x3f; 
    insttype=6;
    if (op==7) { insttype=7; op=0; a=0; }
  }
  return (insttype<<18)+(op<<15)+(a<<12)+(b<<9)+dest;
}

unsigned int I1[7] = {0x653f, 0x663f, 0x613f, 0x607f, 0x60bf, 0x643f, 0x633f};

// go from 21 bits mini assembler instruction format to 16 bit native format
unsigned int packassemble(unsigned long v)
{ unsigned int insttype = (v&07000000)>>18;
  unsigned int op       = (v&00700000)>>15;
  unsigned int a        = (v&00070000)>>12;
  unsigned int b        = (v&00007000)>>9;
  unsigned int dest     = (v&00000777);
  unsigned int shft     = (v&00000700)>>6;
  unsigned int crry     = (v&00000030)>>3;
  unsigned int nold     = (v&00000040)>>5;
  unsigned int skip     = (v&00000007);


  if (insttype==1 && op==2) return I1[op]+(a<<11);
  if (insttype==1 && op <7) return I1[op];
  if (insttype==2) return ((op  )<<11)+(b<<8)+dest;
  if (insttype==3) return ((op+1)<<13)+(a<<11)+(b<<8)+dest;
  if (insttype==4) return ((op+2)<<11)+(b<<8)+dest;
  if (insttype==5) return 0x8000+((op)<<8)+(a<<13)+(b<<11)+(shft<<6)+(crry<<4)+(nold<<3)+skip;
  if (insttype==6) return 0x6000+((op)<<8)+(a<<11)+(b<<6)+(dest&077);
  if (insttype==7) return 0x6000+((7)<<8)+(0<<11)+(b<<6)+(dest&077);
  return 0;
}

// Disassemble 21 bits instruction, mini assembler style, and print to lcd
String printMiniDisas(unsigned long v, int octalmode) {
  return printDisas(packassemble(v), octalmode);
} 

void printAssemblyHelp(unsigned long v, int noctal)
{ int type = (v&07000000)>>18;
  printHelp("");

  if (noctal==0)            printHelp(F("1-7= special jmp ld/st inc ops I/O skip"));

  if (noctal==1 && type==1) printHelp(F("0-6 IORST HLT READS INTEN INTD MSKO INTA"));
  if (noctal==1 && type==2) printHelp(F("0=JMP 1=JMS"));
  if (noctal==1 && type==3) printHelp(F("0=LDA 1=STA"));
  if (noctal==1 && type==4) printHelp(F("0=ISZ 1=DSZ"));
  if (noctal==1 && type==5) printHelp(F("0..7=CMP NEG MOV INC ADC SUB ADD AND"));
  if (noctal==1 && type==6) printHelp(F("0..6=NIO DIA DOA DIB DOB DIC DOC"));
  if (noctal==1 && type==7) printHelp(F("IO status 0=skip"));

  if (noctal==2 && (type==1||type==3)) 
                            printHelp(F("accu 0..3 = ac0..ac3"));
  if (noctal==2 && type==5) printHelp(F("source accu 0..3 = ac0..ac3"));

  if (noctal==3 && type>=2 && type<=4) 
                            printHelp(F("0=abs 1=pc+ 2=ac2+ 3=ac3+ +4=@"));
  if (noctal==3 && type==5) printHelp(F("dest accu 0..3 = ac0..ac3"));
  if (noctal==3 && type==6) printHelp(F("IO func 0=none 1=set 2=clr 3=puls"));
  if (noctal==3 && type==7) printHelp(F("skip 0=busy 1=!busy 2=done 3=!done"));
  
  if (noctal==4 && type==5) printHelp(F("shift 0=none 1=L 2=R 3=bswap"));
  if (noctal==5 && type==5) printHelp(F("carry 0=as is 1=Z 2=O 3=C +4=noload"));
  if (noctal==6 && type==5) printHelp(F("skip 0..7=no yes NC C Z NZ NCZ CZ"));
  
  if (noctal>=4 && type>1 && type<=4) 
                            printHelp(F("offset +0..177 pos 377..200 neg"));
  if (noctal>=4 && type>=6) printHelp(F("device 000..077"));
}

////////////////////////////////////////////////////
// mini debugger display on lcd in single step mode
void lcdPrintDebug(void) {
  lcdclear();
  unsigned int pc = readAddr()&0x7fff;
  unsigned int carry = readAddr()&0x8000;
  unsigned int in = readData(), a0, a1, a2=0, a3=0;
  lcdsetCursor(0,0);		// print 4 accumulators
  lcdprint("ac0:");
  lcdPrintOctal(a0=examineAC(0));
  lcdprint(" 1:");
  lcdPrintOctal(a1=examineAC(1));
  lcdprint(" 2:");
  lcdPrintOctal(a2=examineAC(2));
  lcdprint(" 3:");
  lcdPrintOctal(a3=examineAC(3));
  lcdprint(" ");
  if (carry)
    lcdprint("C");
  else
    lcdprint(".");
  in = examine(pc);
  lcdsetCursor(0,1);
  lcdPrintOctal(pc);    // print address
  lcdsetCursor(6,1);
  lcdprint(printDisas(in, OCT));    // disassemble instruction

  int mref = (in>>11)&0x1f;
  int b=(in&0x700)>>8;
  int dest=in&0xff;
  bool target=false;
  int indirect=0;
  unsigned int t=0;
    
  if      (mref<=1)        { target = true; } // jmp, jms
  else if (mref<=3)        { target = true; indirect=1; } // isz, dsz
  else if ((mref&0x1c)==4) { target = true; indirect=1; } // lda
  else if ((mref&0x1c)==8) { target = true; } // sta

  if (target)
  { switch(b)
    { case 1: case 5: if (dest&0x80) t = pc-(128-(dest&127)); else t=pc+dest; break;
      case 2: case 6: if (dest&0x80) t = a2-(128-(dest&127)); else t=a2+dest; break;
      case 3: case 7: if (dest&0x80) t = a3-(128-(dest&127)); else t=a3+dest; break;
      case 0: case 4: t=dest; break;     
    }

    if (b>3) indirect++;
    lcdprint(" ");
    if (b>3) lcdprint("@");
    if (b>0) lcdPrintOctal(t);
    for (int i=0; i<indirect; i++)
    { if (b>3) do t=examine(t); while (t&0x8000);
      else t=examine(t);
    }
         
    if (indirect)
    { lcdprint("->");
      lcdPrintOctal(t);
    }
  }

  examine(pc);
}

// mini debugger display on serial interface
void serialDebug(int mode) {
  unsigned int pc = readAddr()&0x7fff;
  unsigned int carry = readAddr()&0x8000;
  unsigned int in = readData(), a0, a1, a2, a3;
  a0=examineAC(0);
  a1=examineAC(1);
  a2=examineAC(2);
  a3=examineAC(3);
  if (mode&1)
  { Serial.print("ac0:");
    Serial.print(toOct(a0));
    Serial.print(" 1:");
    Serial.print(toOct(a1));
    Serial.print(" 2:");
    Serial.print(toOct(a2));
    Serial.print(" 3:");
    Serial.print(toOct(a3));
    Serial.print(" ");

    if (carry)
      Serial.println("C");
    else
      Serial.println(".");
  }

  if (mode&2)
  { in = examine(pc);
    Serial.print(toOct(pc));    // print address
    Serial.print(printDisas(in, OCT));    // disassemble instruction
  
    int mref = (in>>11)&0x1f;
    int b=(in&0x700)>>8;
    int dest=in&0xff;
    bool target=false;
    int indirect=0;
    unsigned int t=0;
  
    if      (mref<=1)        { target = true; } // jmp, jms
    else if (mref<=3)        { target = true; indirect=1; } // isz, dsz
    else if ((mref&0x1c)==4) { target = true; indirect=1; } // lda
    else if ((mref&0x1c)==8) { target = true; } // sta
  
    if (target)
    { switch(b)
      { case 1: case 5: if (dest&0x80) t = pc-(128-(dest&127)); else t=pc+dest; break;
        case 2: case 6: if (dest&0x80) t = a2-(128-(dest&127)); else t=a2+dest; break;
        case 3: case 7: if (dest&0x80) t = a3-(128-(dest&127)); else t=a3+dest; break;
        case 0: case 4: t=dest; break;     
      }
  
      if (b>3) indirect++;
      Serial.print(" ");
      if (b>3) Serial.print("@");
      if (b>0) Serial.print(toOct(t));
  
      for (int i=0; i<indirect; i++)
      { if (b>3) do t=examine(t); while (t&0x8000);
        else t=examine(t);
      }
           
      if (indirect)
      { Serial.print("->");
        Serial.print(toOct(t));
      }
    }
  }
  examine(pc);
}

////////////////////////////////////////////////////
// scan 4x3 keyboard array using 4 digital and one analog line
int readKeyBoard(void)
{ 
#ifdef TEENSY
  digitalWrite(A6, 1); 
  delay(1);
  int a = analogRead(A4);
  digitalWrite(A6, 0);
  digitalWrite(A7, 1);
  delay(1);
  int b = analogRead(A4);
  digitalWrite(A7, 0);
  digitalWrite(A8, 1);
  delay(1);
  int c = analogRead(A4);
  digitalWrite(A8, 0);
  digitalWrite(A9, 1);
  delay(1);
  int d = analogRead(A4);
  digitalWrite(A9, 0);
#else
  digitalWrite(A2, 1); 
  delay(1);
  int a = analogRead(A6);
  digitalWrite(A2, 0);
  digitalWrite(A3, 1);
  delay(1);
  int b = analogRead(A6);
  digitalWrite(A3, 0);
  digitalWrite(A4, 1);
  delay(1);
  int c = analogRead(A6);
  digitalWrite(A4, 0);
  digitalWrite(A5, 1);
  delay(1);
  int d = analogRead(A6);
  digitalWrite(A5, 0);
#endif

  int row=0, col=0;
  if (a>100 or b>100 or c>100 or d>100)
  { 
#ifdef CONTROLSWITCHES
    if (a>100) row = 3, col = a; // hand wired board
    if (b>100) row = 2, col = b;
    if (c>100) row = 1, col = c;
    if (d>100) row = 4, col = d;
#else
    if (a>100) row = 1, col = a; // nova1200rev2
    if (b>100) row = 4, col = b;
    if (c>100) row = 3, col = c;
    if (d>100) row = 2, col = d;
#endif
    return row*1024+col;
  }
  return 0;
}

// translate keyboard to key, returns -key when keeping pressed
int testKeyBoard(int prev)
{ int row1=0, row2=0, row3=0, row=0, col=0;

  row1 = readKeyBoard();
  if (row1)
  { delay(10);
    row2 = readKeyBoard();
    if (abs(row1-row2)<70)
    { delay(10);
      row3 = readKeyBoard();
    }

    if (abs(row1-row3)<70)
    { row = (row1+row2+row3)/3;
      col = row&1023;
      row = row/1024;
    
      if (col > 750) col = 2;
      else if (col>400 && col<600) col = 1;
      else if (col>260 && col<400) col = 3;
      else col=0;

      if (prev==0 && row>0 && col>0)
        return (row-1)*3+col;
      if (row>0 && col>0)
        return -((row-1)*3+col);
    }
  }

  return 0;
}

void tests(int func)
{ // register test: disconnect boards from nova and connect data req or inst req to gnd
  if (func==0) // 1st switch up
  { for(int count=0; count<65536; count++)
    { lcdsetCursor(0,1);
      lcdPrintOctal(count);
      lcdprint("  reg test ");
      lcdPrintOctal(readLights());
      writeData(32768>>(count&15));
      writeInst(1<<(count&15));
      WriteReg(10, (count&7)); // keep connected to reg
      WriteReg(11, count/16);

      // test that read data gives back data (only if hand enabled)
      digitalWrite(13, readData()!=((unsigned)32768>>(count&15)));
      delay(100);
      //debugPrint(a0, a1, 11, 11, readAddr(), readData()); 
    }
  }

  // full memory dump
  else if (func==1) // 2nd switch up
  { stopNova();
    lcdsetCursor(0,1);
    lcdprint("     dump active");
    lcdsetCursor(0,1);
    for(int a=0; a<memsize; a++)
    { if ((a&511)==511) lcdprint("#");
      int v = examine(a);
      Serial.print(toHex(a));
      Serial.print(":");
      Serial.println(toHex(v));
    }
    lcdsetCursor(0,1);
    lcdprint("dump ready      ");
  }

  // small memory test, address 0 only
  else if (func==2) // 3rd switch up
  { stopNova();
    lcdsetCursor(0,1);
    lcdprint("     mt0 active");
    for(unsigned int a=0; a<65535*32; a++)
    { unsigned int v = random(65535); // a&1?0x5555:0xaaaa;
      unsigned int ad = 0; // a&16383;
      deposit(ad, v);
      unsigned int c = examine(ad);
      digitalWrite(13, v!=c);
    }
    lcdsetCursor(0,1);
    lcdprint("mt0 ready      ");
  }

  // small memory test, address 0 only
  else if (func==3) // 4rd switch up
  { stopNova();
    lcdsetCursor(0,1);
    lcdprint("     mt1 active");
    for(unsigned int a=0; a<65535*32; a++)
    { deposit(0, 0); // drive inhibit
    }
    lcdsetCursor(0,1);
    lcdprint("mt1 ready      ");
  }

  // small memory test, address 0 only
  else if (func==4) // 5th switch up
  { stopNova();
    lcdsetCursor(0,1);
    lcdprint("     mt2 active");
    for(unsigned int a=0; a<65535*32; a++)
    { deposit(0, 0xffff); // no inhibits driven
    }
    lcdsetCursor(0,1);
    lcdprint("mt2 ready      ");
  }
  else if (func==5) // test write/read all memory
  { while(1) {
      int i=0; String s="";
      for (i=8192; i<memsize; i+=64) {
        deposit(i, i);
      }
      for (i=8192; i<memsize; i+=64) {
        int v = examine(i);
        if (v==i) s+='.'; else s+='X';
      }  
      Serial.println(s);
    }
  }
  else if (func==6) // Continue passing virtual switches
  { continueNovaSw(0x6666);
    lcdsetCursor(0,1);
    lcdprint("sent 6666 to READS");
    delay(200);
  }
  else if (func==7) // Copy prog1..4 to NOVA
  { stopNova();
    assemble(0, prog1);
    assemble(050, prog2);
    assemble(0100, prog3);
    assemble(0140, prog4);
    // assemble(0400, prog4); // force PC relative addressing
    lcdsetCursor(0,1);
    lcdprint("loaded prog1..4");
    delay(200);
  }
  else if (func==8)
  { stopNova();
    lcdsetCursor(0,1);
    lcdprint("     mtfull active");
    for(unsigned int a=0; a<65535*32; a++)
    { unsigned int ad = a; // a&16383;
      while(ad>=memsize) ad-=memsize;
      unsigned int org = examine(ad);
      unsigned int v = random(65535); // a&1?0x5555:0xaaaa;
      deposit(ad, v);
      unsigned int c = examine(ad);
      digitalWrite(13, v!=c);
      deposit(ad, org);
    }
    lcdsetCursor(0,1);
    lcdprint("mtfull ready      ");
  }
}

// -------------------- keyboard interface description ---------------------
//
// 9 mode (press long from in decimal or I/O mode)
// 0: examine mode
// 1: decimal mode
// 2: mini assembler mode
// 3: debug mode
// 4: setup mode --> 0=small LCD, 1=large LCD
// 5: I/O mode: keyboard and lcd connected to memory mapped I/O of Nova
// 6: Test mode
// 7: Save core mode
// 8: Load core mode
// 9: Run

// 0..7 --> enter address mode; just display address (kbmode==1)
// *    --> examine ac (0..3) or examime memory (00..03..4..high)
// 8 backspace (not in decimal mode)

// * examine next; display address and disassembly
// 8 examine previous (not in decimal mode)
    
// #    --> enter data mode; display cursor on data value (kbmode==2)
// 8 backspace (not in decimal mode)
// 9 forward space (not in decimal mode)
// # deposit
// * ignore

// debug mode #; display all registers and next instruction
// 8 step back; move to previous instruction

// setup mode
// 0..6 sets mode
// 7=save to EEPROM, 8=load from EEPROM, 9=run/stop

// IO mode: 9 (long) enters setup mode
// any character on keypad is written to Nova location 040
// character written to Nova location 042 is displayed on LCD and serial
    
// test mode
// 0..7 selects tests (7=copy test program to nova)

int kbmode=0, noctal;               // mode for keypad 1=address, 2=data
unsigned int octalval;             // currently entered octal data
unsigned short octaladdress;          // last used octal address from keypad/front panel
unsigned int opmode=0;              // opmode, set with key 9
int debugging=0;                    // set after single instruction step
unsigned short haltInstruction, haltA0, haltAddress;
unsigned int core_lengths[] = {4, 128, 128, 128, 128, 512}; // in blocks of 64 words

void processkey(short kbkey)
{ /////////////////////////// keypad pressed /////////////////////////
  // octal key or numerical key in decimal mode?
  bool isoct = ((kbkey>=1 && kbkey<=7) or kbkey==11);
  if (opmode==1)
    isoct = isoct or kbkey==8 or kbkey==9;

  if (isoct && kbmode<=1 && opmode<=3) // octal key: start or continue address mode
  { if (kbmode==0)
    { kbmode=1;
      octalval=0;
      noctal=0;
      printHelp("* = goto; 8 = backspace");
    }

    int kb=kbkey;
    if (kb==11) kb=0;

    if (noctal==0)
    { octalval=0;
      lcdclearline(0); 
      lcdnoCursor();
      if (opmode==1)
        printHelp("* = goto");
      else
        printHelp("* = goto; 8 = backspace");
    }

    lcdsetCursor(noctal,0);
    lcdprint(kb); octalval = (opmode==1?10:8)*octalval+kb; noctal++; 
    lcdnoCursor();
  }
  else if (kbkey==8 && kbmode==1 && noctal>0 && opmode<3) // backspace in address mode
  { noctal--;
    octalval = octalval>>3;
    lcdsetCursor(noctal,0);
    lcdprint(" ");
  }

  else if (isoct && kbmode==2 && opmode==0)   //  octal key in edit data mode
  { int kb=kbkey;
    if (kb==11) kb=0;
    int pos=(5-noctal)*3;
    octalval = (octalval&(~(7<<pos)))+(kb<<pos); noctal++; 
    octalval = octalval & 0xffff;
    lcdsetCursor(0,0);
    lcdPrintOctal(octaladdress);
    lcdprint(":");
    lcdPrintOctal(octalval);
    lcdclearline(1);
    lcdsetCursor(0,1);
    if (octaladdress<0xfffc) 
    { lcdprint("      :");
      lcdprint(printDisas(octalval, 8));
    }
    else 
    { lcdprint("ac");
      lcdprint(octaladdress-0xfffc);
    }
    if (noctal>5) noctal=0;
    lcdsetCursor(7+noctal,0);
    lcdcursor();
  }

  else if (isoct && kbmode==2 && opmode==1)   //  numeric key in decimal mode
  { int kb=kbkey;
    if (kb==11) kb=0;
    octalval = 10*octalval + kb;
    noctal++; 
    octalval = octalval & 0xffff;
    lcdsetCursor(0,0);
    lcdPrintDecimal(octaladdress);
    lcdprint(":");
    lcdprint(octalval);
    lcdclearline(1);
    lcdsetCursor(0,1);
    if (octaladdress>=0xfffc) 
    { lcdprint("ac");
      lcdprint(octaladdress-0xfffc);
    }
    if (noctal>6) noctal=0;
    lcdnoCursor();
  }

  else if (isoct && kbmode==2 && opmode==2)   //  octal key in assembly mode
  { unsigned long kb=kbkey;
    if (kb==11) kb=0;
    int pos=(6-noctal)*3;
    octalval = (octalval&(~((unsigned long)7<<pos)))+(kb<<pos); noctal++; 
    octalval = octalval & 0xffffff;
    lcdsetCursor(0,0);
    lcdPrintOctal(octaladdress);
    lcdprint(":");
    lcdPrintLongOctal(octalval);
    lcdclearline(1);
    lcdsetCursor(0,1);
    if (octaladdress<0xfffc) 
    lcdprint("      :");
    lcdprint(printMiniDisas(octalval, 8));
    if (noctal>6) noctal=0;
    printAssemblyHelp(octalval, noctal);
    lcdsetCursor(7+noctal,0);
    lcdcursor();
  }

  else if (kbkey==8 && kbmode==2 && opmode<=2)  // arrow back data mode
  { if (noctal==0) noctal=opmode?6:5; else noctal--;
    lcdsetCursor(7+noctal,0);
    lcdcursor();
    if (opmode==2) printAssemblyHelp(octalval, noctal);
  }
  else if (kbkey==9 && kbmode==2 && opmode<=2)  // arrow forward data mode
  { noctal++; if (noctal>(opmode?6:5)) noctal=0;
    lcdsetCursor(7+noctal,0);
    lcdcursor();
    if (opmode==2) printAssemblyHelp(octalval, noctal);
  }

  else if ((kbkey==10 && opmode<=3) or                        // * = examine / examine next
           (kbkey==8 && kbmode==1 && noctal==0 && opmode<=2)) // 8 = examine previous
  { if (kbmode==2) ; //
    else if (kbkey==8) octaladdress--;
    else if (noctal==0) octaladdress++;
    else if (noctal==1 && octalval<4) octaladdress = 0xfffc + (octalval&3);
    else octaladdress = octalval;
	lcdclearline(0);

    lcdsetCursor(0,0);
    if (opmode==1)
      lcdPrintDecimal(octaladdress); 
    else
      lcdPrintOctal(octaladdress); 
    stopNova();
    lcdprint(":"); 
    if (opmode==1)
      lcdPrintDecimal(examine(octaladdress));
    else
      lcdPrintOctal(examine(octaladdress));
    kbmode=1;
    noctal=0;
    octalval=0;
    lcdclearline(1);
    lcdsetCursor(0,1);
    if (octaladdress<0xfffc) 
    { lcdprint("      :"); 
      lcdprint(printDisas(examine(octaladdress), 8));
    }
    else 
    { lcdprint("ac");
      lcdprint(octaladdress-0xfffc);
    }
    lcdnoCursor();
    if (opmode==0 or opmode==2) printHelp("*/8 = next/prev address; # = edit");
    if (opmode==1) printHelp("* = next address; # = edit");
    if (opmode==3) printHelp("#=step, 8=step back");
  }
  else if (kbkey==12 && kbmode<2 && opmode==0)  // # = start edit mode
  { if (kbmode==1 && noctal==1 && octalval<4) octaladdress = 0xfffc + (octalval&3);
    else if (kbmode==1 && noctal>=1) octaladdress=octalval; // allow e.g. 123#: edit 123
    kbmode = 2;
    noctal=0;
    octalval = examine(octaladdress);
    lcdclearline(0);
    lcdsetCursor(0,0);
    lcdPrintOctal(octaladdress); 
    lcdprint(":"); 
    lcdPrintOctal(octalval);
    lcdclearline(1);
    lcdsetCursor(0,1);
    if (octaladdress<0xfffc) 
    { lcdprint("      :"); 
      lcdprint(printDisas(examine(octaladdress), 8));
    }
    else 
    { lcdprint("ac");
      lcdprint(octaladdress-0xfffc);
    }
    printHelp("8/9 = left/right; # = store; * = cancel");
    lcdsetCursor(7+noctal,0);
    lcdcursor();
  }
  else if (kbkey==12 && kbmode<2 && opmode==1)  // # = start decimal edit mode
  { kbmode = 2;
    noctal=0;
    //octalval = examine(octaladdress);
    octalval=0;
    lcdclearline(0);
    lcdsetCursor(0,0);
    lcdPrintDecimal(octaladdress); 
    lcdprint(":"); 
    lcdprint(octalval);
    lcdclearline(1);
    lcdsetCursor(0,1);
    stopNova();
    if (octaladdress<0xfffc) 
    { lcdprint("      :"); 
      lcdprint(printDisas(examine(octaladdress), 8));
    }
    else 
    { lcdprint("ac");
      lcdprint(octaladdress-0xfffc);
    }
    printHelp("decimal mode # = store; * = cancel");
    //lcdsetCursor(6+noctal,0);
    lcdnoCursor();
  }
  else if (kbkey==12 && kbmode==2 && (opmode==0 or opmode==2))  // # = edit mode deposit
  { if (opmode==2) octalval=packassemble(octalval);
    stopNova();
    deposit(octaladdress, octalval);
    lcdclearline(0);
	lcdsetCursor(0,0);
    lcdPrintOctal(octaladdress); 
    lcdprint("<"); 
    lcdPrintOctal(examine(octaladdress));
    kbmode=1;
    noctal=0;
    lcdclearline(1);
    lcdsetCursor(0,1);
    if (octaladdress<0xfffc) 
    { lcdprint("      <");
      lcdprint(printDisas(examine(octaladdress), 8));
    }
    else 
    { lcdprint(" ac");
      lcdprint(octaladdress-0xfffc);
    }
    printHelp("");
    lcdnoCursor();
  }
  else if (kbkey==12 && kbmode==2 && opmode==1)   // # = decimal mode deposit
  { stopNova();
    deposit(octaladdress, octalval);
    lcdclearline(0);
	lcdsetCursor(0,0);
    lcdPrintDecimal(octaladdress); 
    lcdprint("<"); 
    lcdPrintDecimal(examine(octaladdress));
    lcdprint(makespc(40)); 
    kbmode=1;
    noctal=0;
    lcdclearline(1);
    lcdsetCursor(0,1);
    if (octaladdress>=0xfffc) 
    { lcdprint(" ac");
      lcdprint(octaladdress-0xfffc);
    }
    printHelp("");
    lcdnoCursor();
  }
  else if (kbkey==12 && kbmode<2 && opmode==2)   // # = start assembly mode
  { stopNova();
    kbmode = 2;
    noctal=0;
    octalval = examine(octaladdress);
    octalval=unpackassemble(octalval);
    lcdclearline(0);
    lcdsetCursor(0,0);
    lcdPrintOctal(octaladdress); 
    lcdprint(":"); 
    lcdPrintLongOctal(octalval);
    lcdclearline(1);
    lcdsetCursor(0,1);
    lcdprint("      :"); 
    lcdprint(printMiniDisas(octalval, 8));
    printAssemblyHelp(octalval, noctal);
    lcdsetCursor(7+noctal,0);
    lcdcursor();
  }
  else if (opmode==3 && kbkey==12)      // step instruction
  { stopNova();
    prevpc = readAddr() & 0x7fff;
    kbmode=0;
    noctal=0;
    octalval=0;
    lcdnoCursor();
    stepNova();
    lcdPrintDebug();
    debugging=1;
  } 
  else if (opmode==3 && kbkey==8 && debugging)    // 'step back' instruction
  { stopNova();
    octaladdress = prevpc;
    prevpc = octaladdress-1;
    examine(octaladdress);
    lcdsetCursor(0,1);
    lcdPrintDebug(); 
    lcdnoCursor();
    kbmode=1;
    noctal=0;
    octalval=0;
    lcdnoCursor();
  }
  else if (opmode==5 && kbkey>0)  // io mode
  { unsigned int a, kb=kbkey;
    if (kb==10) kb='*';
    else if (kb==11) kb='0';
    else if (kb==12) kb='#';
    else kb='0' + kb;
    bool runn = readLights()&1;
    if (runn) a=stopNova();
    deposit(CHARIN, kb);
    if (runn) 
      startNova(a);
    else 
    { if (haltInstruction == GETC)
      { depositAC(0, kb);
        examine(haltAddress);
        continueNova();
      }
      else
      { examine(haltAddress);
        continueNovaSw(kb);
      }
    }
  }
  else if (kbmode<3 && kbkey==9)      // select opmode
  { kbmode = 3;
    lcdsetCursor(0, 1);
    lcdprint("0-9=exa dec as dbg su io tst sav lod run");
    printHelp("Select run mode");
    while(readKeyBoard()!=0);
    delay(50);
    lcdnoCursor();
  }
  else if (kbmode==3 && kbkey>0)      // select opmode
  { int kb=kbkey;
    if (kb==11) kb=0;
    kbmode = 0;
    if (kb<=8) opmode=kb;
    if (kb==9) 
    { if ((readLights()&1)==0)
      { printHelp(F("RUN")); startNova(octaladdress); return; }
      else
      { printHelp(F("STOP")); octaladdress=stopNova(); return; }
    }
    
    lcdclearline(1);
    if (opmode==0) printHelp(F("N*=AC NN*=exam mem #=edit 9=setup"));
    if (opmode==1) printHelp(F("N*=AC NN*=exam mem #=edit dec 9!=setup"));
    if (opmode==2) printHelp(F("N*=AC NN*=exam mem #=edit asm 9=setup"));
    if (opmode==3) printHelp(F("debug #=step 8=step back 9=setup"));
    if (opmode==4) printHelp(F("setup unused"));
    if (opmode==5) { printHelp(F("I/O mode 9(long)=exit")); lcdclear(); }
    if (opmode==6) { printHelp(F("0=regtest 1=dmp 2-5=mtst 7=pld 8=fullmtst")); lcdclear(); }
    if (opmode==7) { printHelp(F("Save core to eeprom bank N")); lcdclear(); }
    if (opmode==8) { printHelp(F("Load core from eeprom bank N")); lcdclear(); }
    lcdnoCursor();
  }
  else if (opmode==4 && kbkey>0)      // setup (not used)
  { int kb=kbkey;
    if (kb==11) kb=0;
    lcdclear();
    kbmode = 0;
    opmode = 0;
    printHelp("");
    lcdnoCursor();
  }
  else if (opmode==6 && kbkey>0)      // test menu
  { int kb=kbkey;
    if (kb==11) kb=0;
    tests(kb);
    lcdclear();
    kbmode = 0;
    opmode = 0;
    printHelp("");
    lcdnoCursor();
  }
  else if (opmode==7 && kbkey>0)      // save core
  { int kb=kbkey;
    if (kb==11) kb=0;
    if (kb<=5)
    { unsigned int b=0;
      for (int i=0; i<kb; i++) b += core_lengths[i];
      for (unsigned int i=0; i<core_lengths[kb]; i++) 
        writeBlocktoEEPROM(b+i, i*64);
    }
    lcdclear();
    kbmode = 0;
    opmode = 0;
    printHelp("");
    lcdnoCursor();
  }
  else if (opmode==8 && kbkey>0)      // load core
  { int kb=kbkey;
    if (kb==11) kb=0;
    if (kb<=5)
    { unsigned int b=0;
      for (int i=0; i<kb; i++) b += core_lengths[i];
      for (unsigned int i=0; i<core_lengths[kb]; i++) 
        readBlockfromEEPROM(b+i, i*64);
    }
    lcdclear();
    kbmode = 0;
    opmode = 0;
    printHelp("");
    lcdnoCursor();
  }
  else if (kbkey>0)       // unrecognised key
  { kbmode=0;
    noctal=0;
    octalval=0;
    lcdnoCursor();
  }
}

// Array with contents of punched tape; there is space for several tapes
#include "novabasic1.tape.h"

String tapeloader(const unsigned char*p, int len, String name)
{  Serial.println("Loading 'tape' from code memory: "+name);
   //Serial.println("Block:Address");
   int i=0, startaddress=0xffff;
   i=0;

   // skip zero preamble
   while (i<len)
   { if (p[i]!=0) break;
     i++;
   }
   while (i<len)
   { short wc = p[i] + p[i+1]*256;
     wc = -wc;
     i+=2;

     // data block
     if (wc>0 && wc<=16)
     { short adr = p[i] + p[i+1]*256;
       Serial.println(toOct(wc)+':'+toOct(adr));
       i+=2;
       //short cs = p[i] + p[i+1]*256;
       i+=2;
       for (int j=0; j<wc; j++)
       { short data = p[i] + p[i+1]*256;
         deposit(adr+j, data);
         i+=2;
       }
     }
     // constant block
     else if (wc>16 && wc <= 32767)
     { short adr = p[i] + p[i+1]*256;
       Serial.println(toOct(wc)+' '+toOct(adr));
       i+=2;
       //short cs = p[i] + p[i+1]*256;
       i+=2;
       short data = p[i] + p[i+1]*256;
       i+=2;
       for (int j=0; j<wc; j++)
       { deposit(adr+j, data);
       }
     }
     // start address block
     else if (wc == -1)
     { unsigned short adr = p[i] + p[i+1]*256;
       startaddress = adr;
              
       i+=2;
       i+=2;
       break;
     }
   }

   if (startaddress<32768) 
   { Serial.println("start adress "+toOct(startaddress));
     return "run "+toOct(startaddress);
   }

   return "";
}

// ------------------------------------------------------------
// Serial interface description, interactive mode all numbers in octal

String helpstring=
String("Interactive debugger/supervisor for Nova1200:\r\n")+
String("acN [ val]            read/write register\r\n")+
String("pc [ val]             read/write program counter\r\n")+
String("reg                   show all registers, pc and carry\r\n")+
String("mem addr [ val]       read/write memory location\r\n")+
String("Kaddr/val             write memory location(s)\r\n")+
String("val                   write next\r\n")+
String("K                     stop write mode\r\n")+
String("dump address[ length] dump memory\r\n")+
String("list address[ length] list assembly (numbers are octal)\r\n")+
String("asm addr INST         assemble to memory (slow, INST exactly as disassembled)\r\n")+
String("go addr               run at memory location\r\n")+
String("run addr              run at memory location, serial input passed to program\r\n")+
String("fp val                Set front panel switches\r\n")+
String("step [ count]         debug step count instructions\r\n")+
String("reset                 reset nova\r\n")+
String("stop                  stop nova\r\n")+
String("continue              continue nova\r\n")+
String("clear addr cnt [ val] clear block of memory with value\r\n")+
String("load addr block cnt   load RAM from eeprom blocks\r\n")+
String("save addr block cnt   save RAM to eeprom blocks\r\n")+
String("store                 store all RAM to highest eeprom blocks\r\n")+
String("restore               restore RAM from highest eeprom blocks\r\n")+
String("tape filename         Load tape data from eeprom\r\n")+
String("tape list             List all tapes in eeprom\r\n")+
String("test n                run test function\r\n")+
String("version               print version\r\n")+
String("lcd line text         display line 0..3 on LCD\r\n")+
String("led N                 program GPIO and Arduino LED (400)\r\n")+
String("adc                   read A0\r\n")+
String("dac N                 write A14\r\n")+
String("eeprom block          hex dump eeprom block to output\r\n")+
String("memory address        hex dump memory block to output\r\n")+
String("plot address range    plot 80 values (+/- range)\r\n")+
String("vis address count     visualize memory as gray scale, default 1600 chars\r\n")+
String("init                  init Arduino supervisor\r\n")+
String(":nnaaaa[hhhh]         Deposit nn memory locations (a and h hex)\r\n")+
String(";naaaa[hh*16]         Write line n in eeprom block (a and h hex)\r\n")+
String("? $                   Write in hex ?lights ADSS $regs 1234pC\r\n")+
String("~                     Switch to non-interactive mode\r\n")+
String("help                  Show this text\r\n")+
String("(enter)               automatically propose next command");

// suggested named programs, search from start block (e.g. 4):
// @@ name loadaddress length startaddress
// CODE 
// block 4 + length/4 + 2:
// @@ name loadaddress length startaddress
// CODE
// other values (stops search)
//
// required support commands:
// save name addr length [start]
// save name (re-save same block)
// load name (suggests run start address as next command)
// run name
// go name

// old non-interactive mode (all numbers in hex):

// S              Stop
// O              Continue
// ossss          Continue passing s as switches value (experimental)
// I              Instruction step
// Y              Memory step
// T              Reset
// Ghhhh          Start at address hhhh
// Mhhhh          Read memory at address hhhh
// Dhhhhhdddd     Deposit dddd at address hhhh
// En             Examine register n
// Fnhhhh         File register n with data hhhh
// Xn             Read lights (0=data, 1=address/carry, 2=lights)
// LprTEXT[cr]    Display text on LCD panel p, row r
// :nnaaaa[hhhh]  Deposit nn memory locations
// ?nnaaaa        Read nn memory locations
// eaaaa          Read eprom block a
// wnaaaa[hh]     Write line n in eprom block a (pass 32 characters)
// /              enter memory mapped serial IO mode, @=exit
// V              display version information

// lnnaaaa        Disassemble nn memory locations
// r              Show registers
// i              Show disassembly then instruction step
// s              Show disassembly then instruction step then registers
// !              Initialize arduino
// ~              Toggle interactive mode (commands below, all number below in octal)

bool SerialIO=false;               // if set serial is sent/recieved to Nova

#ifdef TEENSY
bool InteractiveDebugger=true;     // if set serial debugger is more interactive
String line;
String nextcmd;
#define NHISTORY 40
String history[NHISTORY];
int nhist=0;
#else
bool InteractiveDebugger=false;     // if set serial debugger is more interactive
#endif

bool kmode=false;
int kaddress=0;

void processSerial(int count)
{ byte b = Serial.read();
  char m[50];
  int pc;
  int a;

  if (SerialIO or opmode==5) 
  { if (b=='@') 
    { SerialIO=false;
      opmode=0;
      Serial.println("interactive mode");
    }
    else if (b==3)
    { SerialIO=false;
      opmode=0;
      stopNova();
      Serial.println("ctrl-C");
      Serial.print(">");
      return;
    }
    else
    { 
#ifdef TEENSY
      Serial2.write(b);
#else
      bool runn = readLights()&1;
      if (runn) a=stopNova();
      deposit(CHARIN, b);
      if (runn) 
       startNova(a);
      else 
      { if (haltInstruction == GETC)
        { depositAC(0, b);
          examine(haltAddress);
          continueNova();
        }
        else
        { examine(haltAddress);
          continueNovaSw(b);
        }
      }
#endif
    }
    return;
  }

  // ctrl-C
  if (b==3)
  { SerialIO=false;
    opmode=0;
    stopNova();
    Serial.println("ctrl-C");
    Serial.print(">");
    return;
  }

  // old non-interactive mode
  else if (!InteractiveDebugger)
  { switch(b)
    { case 'T': resetNova(); // fall through to stop io mode
      case 'S': stopNova(); 
                if(SerialIO or opmode==5) 
                { Serial.println("interactive mode");
                  SerialIO=false; opmode=0; 
                }  
                break;
      case 'I': stepNova(); break;
      case 'Y': memstepNova(); break;
      case 'O': continueNova(); break;
      case 'o': Serial.readBytes(m, 4); // continue put data on switches
                continueNovaSw(readHex(m,4)); 
                break;
      case 'G': Serial.readBytes(m, 4);
                startNova(readHex(m,4));
                break;
      case 'M': Serial.readBytes(m, 4); // read one Memory location 
                Serial.println(toHex(examine(readHex(m,4)))); 
                break;
      case 'D': Serial.readBytes(m, 8); // Deposit one memory location
                deposit(readHex(m,4), readHex(m+4,4)); break;
      case 'E': Serial.readBytes(m, 1); // Examine AC
                Serial.println(toHex(examineAC(m[0]-'0'))); break;
      case 'F': Serial.readBytes(m, 5); // Fill AC
                depositAC(m[0]-'0', readHex(m+1,4)); break;
      case 'X': Serial.readBytes(m, 1); // Read frontpanel LEDs
                if (m[0]=='0') Serial.println(toHex(readData())); 
                if (m[0]=='1') Serial.println(toHex(readAddr())); 
                if (m[0]=='2') Serial.println(toHex(readLights())); 
                break;
      case 'L': {int l=Serial.readBytesUntil(0x0d, m, 42); // write LCD text
                if (m[0]=='0') 
                { lcdsetCursor(0, m[1]-'0');
                  lcdprint(makespc(40));
                  lcdsetCursor(0, m[1]-'0');
                  for (int i=2; i<l; i++) lcdwrite(char(m[i]));
                }
                if (m[0]=='1') 
                { lcd2.setCursor(0, m[1]-'0');
                  lcd2.print(makespc(40));
                  lcd2.setCursor(0, m[1]-'0');
                  for (int i=2; i<l; i++) lcd2.write(char(m[i]));
                }
                break;
                }
      case ':': { // write set of memory locations
                Serial.readBytes(m, 6);
                unsigned short n = readHex(m,2);
                unsigned short a=readHex(m+2,4);
                Serial.readBytes(m, n*4); 
                for (unsigned short i=0; i<n; i++) deposit(a++, readHex(m+i*4,4));
                break;
                }
      case '?': { // read set of memory locations
                Serial.readBytes(m, 6);
                unsigned short n = readHex(m,2);
                unsigned short a=readHex(m+2,4);
                for (unsigned short i=0; i<n; i++) 
                  if ((i&7)==7) Serial.println(toHex(examine(a++))); 
                  else Serial.print(toHex(examine(a++)));
                break;
                }
      case 'l': { // disassemble memory locations
                Serial.readBytes(m, 6);
                unsigned short n = readHex(m,2);
                unsigned short a = readHex(m+2,4);
                for (unsigned short i=0; i<n; i++)
                { Serial.print(toOct(a));
                  unsigned short v=examine(a++);
                  Serial.print(" ");
                  Serial.print(toOct(v));
                  Serial.println(printDisas(v, OCT));
                }
                break;
                }
      case 'r': serialDebug(1); // show registers
                break;
      case 'i': serialDebug(2); // instruction step
                Serial.println();
                stepNova();
                break; 
      case 's': serialDebug(1); // debug step; show registers, then execute instruction
                serialDebug(2); 
                Serial.println();
                stepNova();
                break;
      case '/': SerialIO=true; // enter IO mode
                Serial.println("IO mode; @=exit");
                lcdsetCursor(0, 0);
                lcdprint(makespc(40));
                lcdsetCursor(0, 1);
                lcdprint(makespc(40));
                lcdsetCursor(0, 0);
                break;
      case 'e': { // read 128 byte block a from eeprom
                Serial.readBytes(m, 4);
                unsigned short a=readHex(m,4);
                int deviceaddress = 0x53;                  
                if (a>515) deviceaddress+=4;
                for (short i=0; i<128; i++)
                { String s;
                  if (a<4) s = toHex2(EEPROM.read(a*128+i));
                  else     s = toHex2(i2c_eeprom_read_byte(deviceaddress, (a-4)*128+i));
                  if ((i&15)==15) Serial.println(s); else Serial.print(s);
                }
                break;
                }
      case 'w': { // write 16 byte part n(0..7) of 128 byte block a to eeprom
                Serial.readBytes(m, 5);
                unsigned short n = readHex(m,1);
                unsigned short a=readHex(m+1,4);
                int deviceaddress = 0x53;
                if (a>515) deviceaddress+=4;
                Serial.readBytes(m, 32);
                m[32]=m[33]='0';
                for (short i=0; i<16; i++) m[i]=readHex(m+i*2,2);
                if (a<4) for (short j=0; j<16; j++) EEPROM.write(a*128+n*16+j, m[j]);
                else     i2c_eeprom_write_page(deviceaddress, (a-4)*128+n*16, (byte *)m, 16);
                break;
                }
      case '.': Serial.println("."); // acknowledge
                break;
      case 'V': Serial.println("Arduino code Marcel van Herk " __DATE__);
                break;
      case '!': delay(100);
                setup();
                delay(100);
                break;
#ifdef TEENSY
      case '~': InteractiveDebugger = !InteractiveDebugger;
                Serial.println("Interactive mode");
                Serial.print(">");
                break;
#endif
    }
  }

#ifdef TEENSY
  // new interactive mode
  else 
  { if (b==27) // read escape key sequences (up and down)
    { int k = Serial.read();
      if (k=='[')
      { k = Serial.read();
        if (k=='A')
        { nhist--;
          if (nhist<0) nhist=0;
          line = history[nhist];
          Serial.write("\33[2K\r");
          Serial.print(">");
          Serial.print(line);
        }
        else if (k=='B')
        { nhist++;
          if (nhist>=NHISTORY) nhist=NHISTORY-1;
          line = history[nhist];
          Serial.write("\33[2K\r");
          Serial.print(">");
          Serial.print(line);
        }
      }
      else Serial.write(k);
    }
    else if (b==127 || b==8) // backspace
    { if (line.length()>0) 
      { line.remove(line.length()-1);
        Serial.write(8);
        Serial.write(32);
        Serial.write(8);
      }
    }
    else if (b != '\r' && b != '\n') 
    { line.concat(char(b)); 
      Serial.write(b);
    }

    // enter
    if (b == '\r') 
    { // auto suggest next command
      if (line=="" && nextcmd!="")
      { line=nextcmd;
        Serial.write("\33[2K\r");
        Serial.print(">");
        Serial.print(line);
        nextcmd="";
        return;
      }
  
      Serial.println();

      // search place to store history and store it
      nhist=-1;
      for (int i=0; i<NHISTORY; i++)
      { if (history[i]=="") 
        { history[i]=line;
          nhist=i+1;
          break;
        }
      }
      if (nhist==-1)
      { for (int i=1; i<NHISTORY; i++) 
        { history[i-1]=history[i];
        }
        nhist=NHISTORY-1;
        history[nhist++]=line;
      }

      // stop Nova when running
      if (readLights()&1) a=stopNova();
      pc=readAddr()&0x7fff;

      // show help text
      if (line.startsWith("help"))
      { Serial.println(helpstring);
      }

      // Nova virtual console deposit (Kaddress/data)
      else if (line.startsWith("K"))
      { int pos1 = 1;
        int pos2 = line.indexOf('/', 0);
        if (pos2<0)
        { kmode=false;
        }
        else
        { int a = readOct(line, pos1);
          int b = readOct(line, pos2+1);
          deposit(a, b);
          kmode=true;
          kaddress=a+1;
        }
      }
      else if (kmode && line[0]>'0' && line[0]<='7')
      { int b=readOct(line, 0);
        deposit(kaddress, b);
        kaddress++;
      }
      // disassemble
      else if (line.startsWith("list "))
      { int pos1 = 5;
        int pos2 = line.indexOf(' ', 6);
        unsigned short a = readOct(line, pos1);
        int b = pos2<0 ? 16 : readOct(line, pos2+1);
        for (unsigned short i=0; i<b; i++)
        { Serial.print(toOct(a));
          unsigned short v=examine(a++);
          Serial.print(" ");
          Serial.print(toOct(v));
          Serial.println(printDisas(v, OCT));
        }
        nextcmd = "list "+toOct(a)+" "+toOct(b);
        examine(pc);
      }
        
      // dump memory
      else if (line.startsWith("dump "))
      { int pos1 = 5;
        int pos2 = line.indexOf(' ', 6);
        unsigned short a = readOct(line, pos1);
        int b = pos2<0 ? 16 : readOct(line, pos2+1);
        for (unsigned short i=0; i<b; i++)
        { Serial.print(toOct(a));
          unsigned short v=examine(a++);
          Serial.print(" ");
          Serial.print(toOct(v));
          Serial.print(" ");
          Serial.print(char(max(v>>8,32)));
          Serial.print(char(max(v&255,32)));
          Serial.println("");
        }
        nextcmd = "dump "+toOct(a)+" "+toOct(b);
        examine(pc);
      }

      // read or set accumulator
      else if (line.startsWith("ac"))
      { int pos1 = 2;
        int pos2 = line.indexOf(' ', 3);
        unsigned short a = readOct(line, pos1);
        if (pos2>=0) depositAC(a, readOct(line, pos2+1));
        Serial.println("ac"+String(a)+" = "+toOct(examineAC(a)));
        if (pos2>=0) nextcmd = "ac"+String((a+1)%4)+" "+toOct(readOct(line, pos2+1));
        else nextcmd = "ac"+String((a+1)%4);
      }

      // read or set pc
      else if (line.startsWith("pc"))
      { int pos2 = line.indexOf(' ', 2);
        if (pos2>=0) examine(readOct(line, pos2+1));
        Serial.println("pc = "+toOct(readAddr()&0x7fff));
        nextcmd = "step";
      }

      // show all registers
      else if (line.startsWith("reg"))
      { for (int a=0; a<4; a++)
          Serial.println("ac"+String(a)+" = "+toOct(examineAC(a)));
        Serial.println("pc = "+toOct(readAddr()&0x7fff));
        Serial.println("carry = "+String(readAddr()>>15));
        nextcmd = "";
      }

      // read or set memory location
      else if (line.startsWith("mem "))
      { int pos1 = 4;
        int pos2 = line.indexOf(' ', 5);
        unsigned short a = readOct(line, pos1);
        if (pos2>=0) deposit(a, readOct(line, pos2+1));
        Serial.println("mem "+toOct(a)+" = "+toOct(examine(a)));
        examine(pc);
        if (pos2>=0) nextcmd = "mem "+toOct(a+1)+" "+toOct(readOct(line, pos2+1));
        else nextcmd = "mem "+toOct(a+1);
      }

      // assemble instruction
      else if (line.startsWith("asm "))
      { int pos1 = 4;
        int pos2 = line.indexOf(' ', 5);
        unsigned short a = readOct(line, pos1);
        if (pos2>=0) {
          String t=line.substring(pos2).toLowerCase();

	        // shortcuts to frequent routines, must be in memory to work
	        if      (t==" .mul"   ) deposit(a, 0x043e); // JMS @3e etc
          else if (t==" .div"   ) deposit(a, 0x043f);
          else if (t==" .sys"   ) deposit(a, 0x0440);
          else if (t==" .memclr") deposit(a, 0x045b);
          else if (t==" .memcpy") deposit(a, 0x045c);
          else if (t.startsWith(" .db "))     // .db num or .db "string"
          { int pos3 = t.indexOf('"');
            if (pos3>0)
            { int pos4=t.indexOf('"',pos3+1);
              t = t.substring(pos3+1, pos4);
              int j=t.length()+1;
              if (j&1) j++;
              for (int i=0; i<j; i+=2, a++)
                deposit(a, ((t[i]<<8)+t[i+1]));
              a--;
            }
            else
            { pos3 = t.indexOf(' ', 4);
              unsigned short b = readOct(t, pos3+1);
              deposit(a, b);
            }
          }

          else for(int b=0; b<=65535;)
          { String s = printDisas(b, OCT).toLowerCase();
            if (s.substring(0, 4)!=t.substring(0, 4) && (((b&63)!=63)))
              b+=63; 
            // must be different instruction (all coded in high 10 bits)
            else if (s.equals(t))
            { deposit(a, b);
              break;
            }
            else 
              b++;
          }
          nextcmd = "asm "+toOct(a+1)+" ";
        }
        Serial.print(toOct(a));
        unsigned short v=examine(a++);
        Serial.print(" ");
        Serial.print(toOct(v));
        Serial.println(printDisas(v, OCT));
        examine(pc);
      }

      // run without serial IO
      else if (line.startsWith("go "))
      { int pos1 = 3;
        unsigned short a = readOct(line, pos1);
        startNova(a);
        Serial.println("running");
        nextcmd = "stop";
      }

      // run with serial IO
      else if (line.startsWith("run "))
      { int pos1 = 4;
        SerialIO=true;
        unsigned short a = readOct(line, pos1);
        startNova(a);
        Serial.println("running, use @ to return to command mode");
        nextcmd = "stop";
      }

      else if (line.startsWith("reset"))
      { resetNova();
        stopNova(); 
        if(SerialIO or opmode==5) 
        { Serial.println("interactive mode");
          SerialIO=false; opmode=0; 
        }  
        nextcmd = "";
      }

      else if (line.startsWith("stop"))
      { stopNova(); 
        if(SerialIO or opmode==5) 
        { Serial.println("interactive mode");
          SerialIO=false; opmode=0; 
        }  
        nextcmd = "continue";
      }

      else if (line.startsWith("continue"))
      { continueNova(); 
        nextcmd = "stop";
      }

      else if (line.startsWith("init")) {
        setup();
        delay(100);
        nextcmd = "";
      }

      else if (line.startsWith("version")) {
        Serial.println("Arduino code Marcel van Herk " __DATE__);
        nextcmd = "";
      }

      // load memory from eeprom
      else if (line.startsWith("load "))
      { int pos1 = 5;
        int pos2 = line.indexOf(' ', 6);
        int pos3 = line.indexOf(' ', pos2+1);
        if (pos2>=0 && pos3>=0)
        { unsigned short a = readOct(line, pos1);
          unsigned short b = readOct(line, pos2+1);
          unsigned short c = readOct(line, pos3+1);
          unsigned short s=a;
          for (int block=b; block<b+c; block++,a+=64)
            readBlockfromEEPROM(block, a);
          Serial.println("loaded memory "+toOct(s)+ " with " + c + " blocks");
          nextcmd = "run "+toOct(a);
        }
        else
          nextcmd = "";
        examine(pc);
      }

      // restore memory from eeprom
      else if (line.startsWith("restore"))
      { unsigned short a = 0;
        unsigned short b = 1028-memsize/128;
        unsigned short c = memsize/128;
        unsigned short s=a;
        for (int block=b; block<b+c; block++,a+=64)
          readBlockfromEEPROM(block, a);
        Serial.println("loaded memory "+toOct(s)+ " with " + c + " blocks");
        nextcmd = "run "+toOct(a);
        examine(pc);
      }

      // save memory to eeprom
      else if (line.startsWith("save "))
      { int pos1 = 5;
        int pos2 = line.indexOf(' ', 6);
        int pos3 = line.indexOf(' ', pos2+1);
        if (pos2>=0 && pos3>=0)
        { unsigned short a = readOct(line, pos1);
          unsigned short b = readOct(line, pos2+1);
          unsigned short c = readOct(line, pos3+1);
          unsigned short s=a;
          for (int block=b; block<b+c; block++,a+=64)
            writeBlocktoEEPROM(block, a);
          Serial.println("saved memory "+toOct(s)+ " total of " + c + " blocks");
        }
        nextcmd = "";
        examine(pc);
      }

      // store memory to eeprom
      else if (line.startsWith("store"))
      { unsigned short a = 0;
        unsigned short b = 1028-memsize/128;
        unsigned short c = memsize/128;
        unsigned short s=a;
        for (int block=b; block<b+c; block++,a+=64)
          writeBlocktoEEPROM(block, a);
        Serial.println("saved memory "+toOct(s)+ " total of " + c + " blocks");
        nextcmd = "";
        examine(pc);
      }

      // debug step
      else if (line.startsWith("step"))
      { int pos2 = line.indexOf(' ', 4);
        unsigned short a = pos2>0 ? readOct(line, pos2+1) : 1;
        for (int i=0; i<a; i++) {
          serialDebug(1);
          serialDebug(2); 
          Serial.println();
          stepNova();                  
        }
        nextcmd = "step "+toOct(a);
      }

      // print lights status
      else if (line.startsWith("lights")) {
        Serial.println("Data="+toOct(readData())); 
        Serial.println("Addr="+toOct(readAddr())); 
        Serial.println("Status="+toOct(readLights()));
        nextcmd = "";
      }

      // run test function (may not return)
      else if (line.startsWith("test "))
      { int pos1 = 5;
        unsigned short a = readOct(line, pos1);
        tests(a);
      }

      // set line of LCD
      else if (line.startsWith("lcd "))
      { int pos1 = 4;
        int pos2 = line.indexOf(' ', pos1+1);
        if (pos2>=0)
        { unsigned short a = readOct(line, pos1);
          if (a<2) 
          { lcdsetCursor(0, a);
            lcdprint(makespc(40));
            lcdsetCursor(0, a);
            for (unsigned int i=pos2+1; i<line.length(); i++) lcdwrite(line[i]);
          }
          else
          { lcd2.setCursor(0, a-2);
            lcd2.print(makespc(40));
            lcd2.setCursor(0, a-2);
            for (unsigned int i=pos2+1; i<line.length(); i++) lcd2.write(line[i]);
          }
          nextcmd = "lcd "+String((a+1)%4)+" "+line.substring(pos2+1);
        }
      }

      // program leds
      else if (line.startsWith("led "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
        gpio(a & 255);
        digitalWrite(13, (a>>8)&1);
        digitalWrite(12, (a>>9)&1);
        digitalWrite(11, (a>>10)&1);
        nextcmd = "led "+toOct(a+1);
      }
          
      // program DAC
      else if (line.startsWith("dac "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
        analogWrite(A14, a);
        nextcmd = "dac "+toOct(a+1);
      }

      // read ADC
      else if (line.startsWith("adc"))
      { 
#ifdef TEENSY
        pinMode(A0, INPUT);
        Serial.println("adc0="+toOct(analogRead(A0)));
#else
        pinMode(A1, INPUT);
        Serial.println("adc1="+toOct(analogRead(A1)));
#endif
        nextcmd = "adc";
      }

      // list eeprom block in hex
      else if (line.startsWith("eeprom "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        int deviceaddress = 0x53;                  
        if (a>515) deviceaddress+=4;
        for (short i=0; i<128; i++)
        { String s;
          if (a<4) s = toHex2(EEPROM.read(a*128+i));
          else     s = toHex2(i2c_eeprom_read_byte(deviceaddress, (a-4)*128+i));
          if ((i&15)==15) Serial.println(s); else Serial.print(s);
        }
        nextcmd = "eeprom "+toOct(a+1);
      }

      // list memory block in hex
      else if (line.startsWith("memory "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        for (short i=0; i<64; i++)
        { String s;
          s = toHex(examine(a+i));
          if ((i&7)==7) Serial.println(s); else Serial.print(s);
        }
        nextcmd = "memory "+toOct(a+64);
      }

      // clear block of memory
      else if (line.startsWith("clear "))
      { int pos1 = 6;
        int pos2 = line.indexOf(' ', 7);
        int pos3 = line.indexOf(' ', pos2+1);
        if (pos2>=0)
        { unsigned short a = readOct(line, pos1);
          unsigned short b = readOct(line, pos2+1);
          unsigned short c = pos3<0 ? 0 : readOct(line, pos3+1);
          unsigned short s=a;
          for (int i=0; i<b; i++)
            deposit(s+i, c);
          Serial.println("cleared memory "+toOct(s)+ " " + toOct(b) + " words to " + toOct(c));
          nextcmd = "dump "+toOct(a);
        }
        else
          nextcmd = "";
        examine(pc);
      }

      // load hex data to memory
      else if (line.startsWith(":"))
      { line.toCharArray(m, 50);
        unsigned short n = readHex(m+1,2);
        unsigned short a=readHex(m+3,4);
        if (n<=10) for (unsigned short i=0; i<n; i++) deposit(a++, readHex(m+7+i*4,4));
        nextcmd = "";
      }

      // load hex data (16 bytes) to eeprom
      else if (line.startsWith(";"))
      { line.toCharArray(m, 50);
        unsigned short n = readHex(m+1,1);
        unsigned short a=readHex(m+2,4);
        int deviceaddress = 0x53;
        if (a>515) deviceaddress+=4;
        for (short i=0; i<16; i++) m[i]=readHex(m+6+i*2,2);
        if (a<4) for (short j=0; j<16; j++) EEPROM.write(a*128+n*16+j, m[j]);
        else     i2c_eeprom_write_page(deviceaddress, (a-4)*128+n*16, (byte *)m, 16);
	      nextcmd = "";
      }

      else if (line.startsWith("?"))
      { Serial.print(toHex(readAddr())); 
        Serial.print(toHex(readData())); 
        Serial.println(toHex2(readLights()));
        nextcmd = "?";
      }

      // dump all registers in hex
      else if (line.startsWith("$"))
      { for (int a=0; a<4; a++)
          Serial.print(toHex(examineAC(a)));
        Serial.println(toHex(readAddr()));
        nextcmd = "";
      }

      // enable previous serial mode
      else if (line.startsWith("~"))
      { InteractiveDebugger = !InteractiveDebugger;
        Serial.println("Normal mode");
        nextcmd = "";
      }

      // visualize memory as gray scale a=address b=length of visualization (default 1600 = 20 lines)
      else if (line.startsWith("vis "))
      { int pos1 = 4;
        int pos2 = line.indexOf(' ', 5);
        unsigned short a = readOct(line, pos1);
        int b = pos2<0 ? 1600 : readOct(line, pos2+1);
        for (unsigned short i=0; i<b; i++)
        { unsigned short v=examine(a++);
          char visual[]=" .:-=+*#%@";
          short w = v/2048;
          if (v>=32768) Serial.print("-");
          else if (w>=(int)strlen(visual)) Serial.print("+");
          else Serial.write(visual[w]);
          if (i%80==79) Serial.println("");
        }
        nextcmd = "vis "+toOct(a)+" "+toOct(b);
        examine(pc);
      }

      // plot memory a=address b=value range (applied positive and negative)
      else if (line.startsWith("plot "))
      { int pos1 = 5;
        int pos2 = line.indexOf(' ', 6);
        unsigned short a = readOct(line, pos1);
        int b = pos2<0 ? 32767 : readOct(line, pos2+1);
        short vals[80];
        for (unsigned short i=0; i<80; i++) 
          vals[i] = examine(a++);
        for (short i=10; i>=-10; i--)
        { for (unsigned short j=0; j<80; j++)
          {  short v = (10*(int)vals[j] + b/2)/b;
             if (v==i) Serial.print("*");
             else if (i==0) Serial.print("-");
             else Serial.print(" ");
           }
          Serial.println("");
        }
        nextcmd = "plot "+toOct(a)+" "+toOct(b);
        examine(pc);
      }

      // write serial character
      else if (line.startsWith("serw "))
      { int pos1 = 5;
        unsigned short a = readOct(line, pos1);
        Serial2.write(a);
        nextcmd = "serw "+toOct(a+1);
      }

      // set front panel switches
      else if (line.startsWith("fp "))
      { int pos1 = 3;
        unsigned short a = readOct(line, pos1);
        CurrentSwitchValue=a;
      }

      // absolute tape loader using data in eeprom; tape data is stored after 16 bytes $$llFILENAME@; where ll is its 16 bits length
      else if (line.startsWith("tape "))
      { int pos1 = 5;
        String filename = line.substring(pos1, 99).toUpperCase(); // +"@";

        // absolute tape loader using program data
        if (filename==".BASIC")
        { nextcmd=tapeloader(basictape, sizeof(basictape), ".basic");
          Serial.print(">");
          line = "";
          return;
        }
		
        if (filename=="LIST")
        { Serial.println(".BASIC in code memory; length "+toOct(sizeof(basictape)));
        }

        byte fname[16], ename[16];
        filename.getBytes(fname, 16);
        int deviceaddress = 0x53;                  
          
        // search all 128 character blocks on external eeprom (address 4 and up)
        int block=-1;
        for(int a=4; a<1028; a++)
        { if (a>515) deviceaddress+=4;
          block = -1;
          if (i2c_eeprom_read_byte(deviceaddress, (a-4)*128+0)=='$')
          { if (i2c_eeprom_read_byte(deviceaddress, (a-4)*128+1)=='$')
            { block = a;
              for (unsigned short i=0; i<16; i++)
              { ename[i] = i2c_eeprom_read_byte(deviceaddress, (a-4)*128+i+4);
                if (ename[i]==0) 
                { break;
                }
                if (fname[i]!=ename[i] && block!=-1) 
                { block=-1;
                }
              }
              if (filename=="LIST")
              { String str((char *)ename);
                int len = i2c_eeprom_read_byte(deviceaddress, (a-4)*128+2) + i2c_eeprom_read_byte(deviceaddress, (a-4)*128+3)*256;
                Serial.println(str+" at block "+toOct(a)+"; length "+toOct(len)+" next block: "+toOct(a+(int)ceil((len+16)/128)));
              }
            }
          }
          if (block>=4) break;
        }
        if (block>=0 && filename!="LIST")
        { block-=4;
          int len = i2c_eeprom_read_byte(deviceaddress, block*128+2) + i2c_eeprom_read_byte(deviceaddress, block*128+3)*256;
          Serial.println("Loading tape "+filename+" at eeprom block "+toOct(block)+"; length "+toOct(len)+" next block: "+toOct(block+(int)ceil((len+16)/128)));
          Serial.println("Block:Address");
          int a=block*128+16, i=0, startaddress=0xffff;
          i=0;
          // skip zero preamble
          while (i<len)
          { if (i2c_eeprom_read_byte(deviceaddress, a+i)!=0) break;
            i++;
          }
          while (i<len)
          { short wc = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
            wc = -wc;
            i+=2;

            // data block
            if (wc>0 && wc<=16)
            { short adr = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
              Serial.println(toOct(wc)+':'+toOct(adr));
              i+=2;
              //short cs = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
              i+=2;
              for (int j=0; j<wc; j++)
              { short data = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
                deposit(adr+j, data);
                i+=2;
              }
            }
            // constant block
            else if (wc>16 && wc <= 32767)
            { short adr = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
              Serial.println(toOct(wc)+' '+toOct(adr));
              i+=2;
              //short cs = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
              i+=2;
              short data = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
              i+=2;
              for (int j=0; j<wc; j++)
              { deposit(adr+j, data);
              }
            }
            // start address block
            else if (wc == -1)
            { unsigned short adr = i2c_eeprom_read_byte(deviceaddress, a+i) + i2c_eeprom_read_byte(deviceaddress, a+i+1)*256;
              startaddress = adr;
              
              i+=2;
              i+=2;
              break;
            }
          }
          if (startaddress<32768) 
          { Serial.println("start adress "+toOct(startaddress));
            nextcmd = "run "+toOct(startaddress);
          }
        }
        else if(filename!="LIST")
          Serial.println("Tape "+filename+" not found");
      }

      Serial.print(">");
      line = "";
    }
  }
#endif
}

// adapted from I2C eeprom library from https://playground.arduino.cc/code/I2CEEPROM
// added Wire.begin and Wire.end to allow sharing with lcd data
 
// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page(int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) 
{ Wirebegin();
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  for (byte c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
  delay(5);
  WireEnd();
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer(int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) 
{ Wirebegin();
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,length);
  for (int c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
  WireEnd();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) 
{ byte rdata = 0xFF;
  Wirebegin();
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  WireEnd();
  return rdata;
}

////////////////////////////////////////////////////////////////////////
// table for control keys on MSB board
//                    dep0,  dep1, dep2, dep3, exa0, exa1, exa2, exa3, reset, stop
#ifdef CONTROLSWITCHES
short highvals[10] = {505,  405,  305,  205,   602, 723,  804,  950, 104, 1023}; 
short highinst[10] = {0x23, 0x2B, 0x33, 0x3B, 0x67, 0x6F, 0x77, 0x7F, 0x23, 0x00}; 
short highfuncs[10] = {1,    1,   1,    1,    1,     1,     1,   1,   2,   3};          
                      // 1,2,3=keys reset stop

// table for control keys on LSB board
//                   start  cont  depo depnex  exa  exanext memstep inststep n/a   pl
short lowvals[10]  = {448, 560,   338, 672,    227,  784,   118,    950,    32767, 1023};
short lowinst[10]  = {0xfb, 0xfb, 0xdd, 0xdc,  0xf9, 0xfc,  0xff,   0xff,    0,   0xfd}; 
short lowfuncs[10] = {1,   23,    1,    1,     1,       1,  21,     22,      0,    7};    
                     //4,5,6,7= N/A memstep instep pl, +16 generate con+ signal
unsigned long keydownMillis = 0;    // will store time frontpanel key was pressed
#endif

const long interval = 1;            // interval at which to process (milliseconds)
int keyon = 0;                      // set when key is down
unsigned long previousMillis = 0;   // will store last time LED was updated
unsigned long kbMillis = 0xfffffff; // will store time keypad key was pressed
int kbkey=0;                        // detected key 1..12 on keypad

// write character to LCD interpreting minimal control codes
void putLCD(byte b)
{ lcdpos = lcdpos % 160;
  if      (b==12) { lcdclear(); lcd2.clear(); lcdpos=0; }
  else if (b==13) { lcdpos = lcdpos - lcdpos%40; clearline=true; }
  else if (b==10) { lcdpos = lcdpos + 40; clearline=true; }
  else 
  { if (lcdpos<80)
    { lcdsetCursor(lcdpos%40, floor(lcdpos/40));
      if (clearline) 
      { for (int i=0; i<40; i++) lcdwrite(' ');
        lcdsetCursor(lcdpos%40, floor(lcdpos/40));
      }
      lcdwrite(b);
    }
    else
    { lcd2.setCursor(lcdpos%40, floor(lcdpos/40)-2);
      if (clearline) 
      { for (int i=0; i<40; i++) lcd2.write(' ');
        lcd2.setCursor(lcdpos%40, floor(lcdpos/40)-2);
      }
      lcd2.write(b);
    }
    lcdpos = (lcdpos+1)%160;
    clearline=false;
  }
}

// read 128 byte block from EEPROM to Nova memory A
void readBlockfromEEPROM(int block, unsigned int A)
{ if (block<4)
  { unsigned int address=block*128;
    for (int i=0; i<64; i++)
    { deposit(A+i, EEPROM.read(address+i*2)<<8|EEPROM.read(address+i*2+1));
    }
  }
  else
  { int deviceaddress = 0x53;
    if (block>515) deviceaddress+=4;
    unsigned int address=(block-4)*128; // overflow is OK needs 16 bit
    for (int i=0; i<64; i++)
    { deposit(A+i, i2c_eeprom_read_byte(deviceaddress, address+i*2)<<8|i2c_eeprom_read_byte(deviceaddress, address+i*2+1));
    }
  }
}

// write 128 byte block from EEPROM to Nova memory A
void writeBlocktoEEPROM(int block, unsigned int A)
{ if (block<4)
  { int address=block*128;
    for (int i=0; i<64; i++)
    { unsigned short s=examine(A+i);
      EEPROM.write(address+i*2, s>>8);
      EEPROM.write(address+i*2+1, s&255);
    }
  }
  else
  { int deviceaddress = 0x53;
    if (block>515) deviceaddress+=4;
    unsigned int address=(block-4)*128; // overflow is OK needs 16 bit
    byte buffer[26];
    for (int j=0; j<8; j++)
    { for (int i=0; i<8; i++)
      { unsigned short s=examine(A+j*8+i);
        buffer[i*2]=s>>8;
        buffer[i*2+1]=s&255;
      }
      i2c_eeprom_write_page(deviceaddress, address+j*16, buffer, 16);
      delay(5);
    }
  }
}

void gpio(unsigned int A)
{ byte d=A>>8;
  Wirebegin();
  delay(1);
  Wire.beginTransmission(0x20);
  Wire.write(0x00);
  Wire.write(d);        // direction bits
  Wire.endTransmission();
  Wire.beginTransmission(0x20);
  Wire.write(0x09);
  Wire.write(A);
  Wire.endTransmission();
  if (d)
  { Wire.beginTransmission(0x20);
    Wire.write(0x06);
    Wire.write(d);        // pull up resistors (ON for inputs)
    Wire.endTransmission();
    Wire.beginTransmission(0x20);
    Wire.write(0x09);
    Wire.endTransmission();
    Wire.requestFrom(0x20, 1);
    depositAC(0, Wire.read());
  }
  WireEnd();
}

///////////////////////////////////////// loop ////////////////////////////////////
void loop() {
  unsigned long currentMillis = millis();

#ifdef TEENSY
  if (Serial2.available())
  { byte b = Serial2.read();
    //Serial.println(toHex2(b));
    Serial.write(b&0x7f);
    //Serial.write('\r');
    return;
  }
#endif

  // wait and detect halt condition; special bits in HALT are interpreted as 'OS' instructions
  while (millis() - currentMillis < 100)
  { bool runLight = readLights()&1;
    if (novaRunning && !runLight) {
      haltAddress     = readAddr() & 0x7fff;
      haltInstruction = examine(haltAddress-1);
      haltA0          = examineAC(0);
      novaRunning     = false;

      if (kbmode==3) haltInstruction=0; // allow key 9 to stop running

      if (haltInstruction==INFO)
      { lcdPrintDebug();
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==PUTC) // write character
      { byte b=haltA0&255;
        Serial.write(b);
        putLCD(b);
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==MESSAGE) // write character string from address in A2
      { int a = examineAC(2);
        bool flag=false;
        for (int i=0; i<40; i++)
        { unsigned short s=examine(a+i);
          unsigned short s2=examine(a+i+1);
          byte b = s>>8;
          if (b==0) break;
          if (!flag)
          { if (b=='%') { Serial.print(String((short)examineAC(s&3))); flag=true; }
            else Serial.write(b);
            putLCD(b);
          }
          else
            flag=false;
          b = s&255;
          if (b==0) break;
          if (!flag)
          { if (b=='%') { Serial.print(String((short)examineAC((s2>>8)&3))); flag=true; }
            else Serial.write(b);
            putLCD(b);
          }
          else
            flag=false;
        }
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==WRITELED)
      { 
#ifndef TEENSY
        digitalWrite(6, 1);  // poor attempt to keep LCD tidy as LED doubles as LCD2 select
        digitalWrite(A2, 1);
        digitalWrite(A3, 1);
        digitalWrite(A4, 1);
        digitalWrite(A5, 1);
#endif
        digitalWrite(13, (haltA0&1)!=0);
#ifdef TEENSY
        digitalWrite(12, (haltA0&2)!=0);
        digitalWrite(11, (haltA0&4)!=0);
	analogWrite(A14, haltA0>>4);
#endif
#ifndef TEENSY
        delay(50); // must be shorter than 100 ms above!
        digitalWrite(13, 0);
#endif
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==READBLOCK) // stored BIGENDIAN in eeprom
      { unsigned int A2=examineAC(2);
        gpio(1);
        readBlockfromEEPROM(haltA0, A2);
        depositAC(0, haltA0+1);
        depositAC(2, A2+64);
        gpio(0);
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==SKIPBUSYZ) // skip when character available
      { if (true)
          examine(haltAddress+1);
        else
          examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==WRITEBLOCK) // read BIGENDIAN from eeprom
      { unsigned int A2=examineAC(2);
        gpio(4);
        writeBlocktoEEPROM(haltA0, A2);
        gpio(0);
        depositAC(0, haltA0+1);
        depositAC(2, A2+64);
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==GPIO)     // GPIO through MCP20008
      { gpio(haltA0);
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==DELAY)   // delay on arduino
      { examineAC(1); // show ac1 on lights
        delay(haltA0);
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==ADC)   // adc on teensy
      { 
#ifdef TEENSY
        pinMode(A0, INPUT);
        depositAC(0, analogRead(A0));
#else
        pinMode(A1, INPUT);
        depositAC(0, analogRead(A1));
#endif
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==GETC)
      { break; // handled in serial and keyboard code
      }
      else if (haltInstruction==HALT)
      { Serial.println("HALT at address: "+toOct(haltAddress));
        Serial.print(">");
        SerialIO=false;
        return; // Normal halt: nova ready for input
      }
    }

    if (SerialIO==false && opmode!=5) break;
  }
  
  int count=0;

  if ((count=Serial.available()))
  { // Serial.write("S");
    processSerial(count);
    if (SerialIO) return;
  }

  // loop frequency (go faster when in serial IO mode)
  if (currentMillis - previousMillis >= interval or SerialIO or opmode==5) {
    previousMillis = currentMillis; 	// save the last time you processed 
    
    // read keypad (disabled in serial IO mode), auto repeat after 1 sec
    if (SerialIO) kbkey=0; else kbkey=testKeyBoard(kbkey);
    if (kbkey<0 && currentMillis-kbMillis>1000) 
    { if ((opmode==1 or opmode==5) && abs(kbkey)==9) 
      { opmode=0;  // push 9 long for mode select to exit decimal or i/o mode
        kbmode=0;
        kbkey=9;
      }
      else kbkey=-kbkey;
    }
    else if (kbkey>0) kbMillis= currentMillis;

    ////////////////// process numeric keyboard keys /////////////////////////
    if (kbkey)
      processkey(kbkey);

    /////////////////////////// read switch network /////////////////////////
#ifdef CONTROLSWITCHES
    int a0=0, a1=0;

    // read toggle switches using resistor network, ignore LSB switches in serial mode
    a1 = (analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1))/6;
    if (SerialIO or opmode==5)
      a0=0;
    else
      a0 = (analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0))/6;

    int orga1=a1;/
    int orga0=a0;

    /////////////////////////// console control key pressed /////////////////////////
    // a switch on the MSB board was pressed, detect with 2 x 20ms debounce
    if (a1>1024/20)
    { delay(20);
      a1 = (analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1))/6;
      a0 = (analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0))/6;
      if (abs(orga1-a1)>20) return;
      delay(20);
      a1 = (analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1))/6;
      a0 = (analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0))/6;
      if (abs(orga1-a1)>20) return;

      int minval=9999;

      // PL down + any MSB switch: special functions; room for 10 functions; 5 used
      if (a0>1000)
      { int func=-1;
        for (int i=0; i<10; i++)
        { if (abs(highvals[i] - a1)<minval) 
          { minval = abs(highvals[i] - a1);
            func = i;
          }
        }
        
        // run tests
        tests(func);
        return;
      }

      // find value closest in high board table
      for (int i=0; i<10; i++)
      { if (abs(highvals[i] - a1)<minval) 
        { minval = abs(highvals[i] - a1);
          inst = highinst[i];
          func = highfuncs[i];
        }
      }

      // debug output
      if (keyon==0)
      { //debugPrint(a0, a1, inst, func, readAddr(), readData()); 
        keydownMillis = millis();
      }
    }

    // a switch on the LSB board was pressed, detect with 2 x 20ms debounce
    else if (a0>1024/20)
    { delay(20);
      a1 = (analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1))/6;
      a0 = (analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0))/6;
      if (abs(orga0-a0)>20) return;
      delay(20);
      a1 = (analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1))/6;
      a0 = (analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0))/6;
      if (abs(orga0-a0)>20) return;

      // find clostest value in low table
      int minval=9999;
      for (int i=0; i<10; i++)
      { if (abs(lowvals[i] - a0)<minval) 
        { minval = abs(lowvals[i] - a0);
          inst = lowinst[i];
          func = lowfuncs[i];
        }
      }

      // debug output
      if (keyon==0)
      { //debugPrint(a0, a1, inst, func, readAddr(), readData()); 
        keydownMillis = millis();
      }
    }

    // no frontpanel key pressed, keyboard not active: show model information
    else 
    if (kbmode==0)
    { keyon=0;
      if (opmode!=5)
      { lcdsetCursor(0,0);
        if (!debugging) lcdprint("DGC    Nova 1210");
        if (!debugging) lcdprint(makespc(24));
      }
    }

    // auto repeat all functions after 1 second
    if (func!=0 && millis()-keydownMillis > 1000)
    { keyon=0;
      delay(100);
      if (a0>1000) // display hint for test functions
      { lcdsetCursor(0,0);
        lcdprint(makespc(40));
        lcdsetCursor(0,0);
        lcdprint("TST DMP MT0 1 2");
      }  
    }
      
    // frontpanel keys force keypad to go to inactive mode
    if (func>0 && func!=22) kbmode=0;

    //////////////////////////////// slow serial IO ///////////////////////////
    /*
    unsigned int a=0;
    if (SerialIO or opmode==5)
    { if (readLights()&1)  // Do not use novaRunning, interferes with detection above!
      { a=stopNova();
        if (examine(CHAROUT)>2) 
        { Serial.write(examine(CHAROUT));
          if (opmode==5) 
            lcdwrite(examine(CHAROUT));
          deposit(CHAROUT, 0);
        }
        startNova(a);
      }
    }
    */

    //////////////////// process all frontpanel keys //////////////////
    if (func!=0 && keyon==0)
    { keyon = 1;

      lcdsetCursor(0,1);

      writeInst(inst);	// set instruction register on Nova
      int pulselen=1;

      // regular keys work via conreq (sw request) / coninst (hw response)
      if (func==1 or func==33 or func==23)
      { if (func==33) WriteReg(10, 0);	// connect register to data bus
        else WriteReg(10, 8); // connect keys to data bus
        WriteReg(11, 4);	// pulse conreq

        // debug output
        if (inst==0xfb&&func!=23)      Serial.println("start");
        else if (inst==0xfb&&func==23) Serial.println("continue");
        else if (inst==0xfc) Serial.println("examine next");
        else if (inst==0xf9) Serial.println("examine");
        else if (inst==0xdc) Serial.println("deposit next");
        else if (inst==0xdd) Serial.println("deposit");
        else if (inst==0x7f) Serial.println("exam ac3");
        else if (inst==0x77) Serial.println("exam ac2");
        else if (inst==0x6f) Serial.println("exam ac1");
        else if (inst==0x67) Serial.println("exam ac0");
        else if (inst==0x3b) Serial.println("dep ac3");
        else if (inst==0x33) Serial.println("dep ac2");
        else if (inst==0x2b) Serial.println("dep ac1");
        else if (inst==0x23) Serial.println("dep ac0");
        else Serial.println(inst, BIN);

        WriteReg(11, 0);	// end conreq
        delay(pulselen);
        if (func==33) WriteReg(10, 0);  // connect keys to data bus
        
        Serial.print(" Data=");	// read and debug output lights
        Serial.print(readData(), HEX);
        Serial.print(" Addr=");
        Serial.println(readAddr(), HEX);

        // update lcd for examine/deposit keys; note interaction with keypad address
        lcdsetCursor(0,1);
        lcdprint(makespc(40));
        lcdsetCursor(0,1);
        if      ((inst&0xe0)==0x60) 
        { lcdprint("ac"); lcdprint((inst>>3)&3); lcdprint(":"); octaladdress = 0xfffc+((inst>>3)&3);}
        else if ((inst&0xe0)==0x20) 
	      { lcdprint("ac"); lcdprint((inst>>3)&3); lcdprint("<"); octaladdress = 0xfffc+((inst>>3)&3); }
        else if ((inst&0xf0)==0xf0)  
        { lcdPrintOctal(readAddr()&0x7fff); lcdprint(":"); octaladdress = readAddr()&0x7fff; }
        else if ((inst&0xf0)==0xd0) 
        { lcdPrintOctal(readAddr()&0x7fff); lcdprint("<"); octaladdress = readAddr()&0x7fff; }
        lcdPrintOctalAscii(readData());
        debugging=0;		// stop single step debug output
      }
      if (func==2)
      { resetNova();
        debugging=0;
      }
      if (func==3)
      { stopNova();
        Serial.println("stop");
        if(SerialIO or opmode==5) 
        { Serial.println("interactive mode");
          SerialIO=false; opmode=0; 
        }  
        lcdsetCursor(0,1);
        lcdprint(makespc(40));
        lcdsetCursor(0,1);
        lcdprint("#");
        lcdPrintOctal(readAddr()&0x7fff); lcdprint(":");
        lcdPrintOctal(readData());      
        debugging=0;
      }
      if (func==21)
      { stopNova();
        memstepNova();
        Serial.println("memory step");
        lcdsetCursor(0,1);
        lcdprint(makespc(40));
        lcdsetCursor(0,1);
        lcdprint("$");
        lcdPrintOctal(readAddr()&0x7fff); lcdprint(":");
        lcdPrintOctal(readData());      
        debugging=0;
      }
      if (func==22)		// single step shows debugger output
      { stopNova();
        stepNova();
        lcdPrintDebug();
        debugging=1;
      }
      if (func==7)
      { WriteReg(10, 10);
        delay(pulselen);
        Serial.println("program load");
        WriteReg(10, 8);
        lcdsetCursor(0,1);
        lcdprint(makespc(40));
        lcdsetCursor(0,1);
        lcdprint("progload/special"); 
        debugging=0;
      }
    }
    else if (func==0)
      keyon=0;
#endif
  }
}
