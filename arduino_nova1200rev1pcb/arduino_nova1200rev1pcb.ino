// nova control panel arduino controller
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

#include <LiquidCrystal.h>
#include <EEPROM.h>

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
*/

void WriteReg(int n, int value) {
  value=~value;				// negate value to make active high

  digitalWrite(2, value&1);		// output 4bit data
  digitalWrite(3, (value>>1)&1);
  digitalWrite(4, (value>>2)&1);
  digitalWrite(5, (value>>3)&1);
  
  digitalWrite(6, n&1);			// output 4bits address (3 bits used)
  digitalWrite(7, (n>>1)&1);
  digitalWrite(10, (n>>2)&1);
  digitalWrite(11, 0); // A0'

  if (n&8) {				// pulse enable on one of the boards
    digitalWrite(9, 1);
    digitalWrite(9, 0);
    digitalWrite(9, 1);
  }
  else {
    digitalWrite(8, 1);
    digitalWrite(8, 0);
    digitalWrite(8, 1);
  }
}

void writeData(int d)			// set data register
{ WriteReg(0, d&15);        // data L
  WriteReg(1, (d>>4)&15);   // data H
  WriteReg(8, (d>>8)&15);   // data L brd2
  WriteReg(9, (d>>12)&15);  // data H brd2
}

void writeInst(int i)			// set instruction register
{ WriteReg(2, i&15);        // data L
  WriteReg(3, (i>>4)&15);   // data H
  WriteReg(14, (i>>8)&15);   // data L brd2
  WriteReg(15, (i>>12)&15);  // data H brd2
}

unsigned int readReg(int n) {
  int res;
  pinMode(2, INPUT);		// change bus to input
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  digitalWrite(6, n&1);		// write 4 bit address
  digitalWrite(7, (n>>1)&1);
  digitalWrite(10, (n>>2)&1);
  digitalWrite(11, (n>>4)&1); // A0'
  digitalWrite(8, 1);
  digitalWrite(9, 1);		// make sure all boards are disabled

  if (n&8) {			// read from board 2
    digitalWrite(9, 1);
    digitalWrite(9, 0);
    res = 15-(digitalRead(2)|(digitalRead(3)<<1)|(digitalRead(4)<<2)|(digitalRead(5)<<3));
    digitalWrite(9, 1);
  }
  else {			// read from board 1
    digitalWrite(8, 1);
    digitalWrite(8, 0);
    res = 15-(digitalRead(2)|(digitalRead(3)<<1)|(digitalRead(4)<<2)|(digitalRead(5)<<3));
    digitalWrite(8, 1);
  }

  pinMode(2, OUTPUT);		// back to output
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  return res;
}

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

LiquidCrystal lcd (6, 12, A4, A5, A2, A3);
LiquidCrystal lcd2(6, 13, A4, A5, A2, A3);
int largescreen=0;

void setup() {
  largescreen = EEPROM.read(0);

  for (int i=2; i<=11; i++) 
    pinMode(i, OUTPUT);

  digitalWrite(9, 1); 		// disable both board enables
  digitalWrite(8, 1);

  pinMode(A0, INPUT); 		// control switches are connected via register network to 2 analog inputs 
  pinMode(A1, INPUT);

  WriteReg(10, 8);    		// disable all ctl signals, set switch mode
  WriteReg(11, 0);

  lcd.begin(40, 2);		// welcome text on LCD
  lcd.print(F("DGC    Nova 1210"));
  lcd.setCursor(0, 1);
  lcd.print(F("DataGeneral 1971"));
  
  lcd2.begin(40, 2);    // welcome text on LCD
  lcd2.setCursor(0, 1);
  lcd2.print(F("DATA GENERAL CORPORATION       NOVA 1210"));
  printHelp(F("This computer was manufactured in 1971"));

  Serial.begin(19200); 		// usb serial for debug output
}

// read nova accumulator 0..3
unsigned int examineAC(int address) {
  writeInst((0x67<<8)+(address<<11));
  WriteReg(11, 4);
  delayMicroseconds(10);
  WriteReg(11, 0);
  delayMicroseconds(10);
  int v = readData();
  return v;
}

// read single word from nova memory or register
unsigned int examine(int address) {
  if (address>=0xfffc)
    return examineAC(address-0xfffc);
  writeInst(0xf9<<8);
  writeData(address);
  WriteReg(10, 0);	// connect data register to bus
  WriteReg(11, 4);	// pulse CONREQ
  delayMicroseconds(10);
  WriteReg(11, 0);	// revert to idle state
  WriteReg(10, 8);
  delayMicroseconds(10);
  int v = readData();	// read lights
  return v;
}

// write single word to nova accumulator
void depositAC(unsigned int address, unsigned int value) {
  writeInst((0x23<<8)+(address<<11));
  writeData(value);
  WriteReg(10, 0);
  WriteReg(11, 4);
  delayMicroseconds(10);
  WriteReg(11, 0);
  delayMicroseconds(10);
  WriteReg(10, 8);
}

// write single word to nova memory or accumulator
void deposit(unsigned int address, unsigned int value) {
  if (address>=0xfffc) 
  { depositAC(address-0xfffc, value);
    return;
  }
  writeInst(0xf9<<8);
  writeData(address);
  WriteReg(10, 0);
  WriteReg(11, 4);
  delayMicroseconds(10);
  WriteReg(11, 0);
  delayMicroseconds(10);
  writeInst(0xdd<<8);
  writeData(value);
  WriteReg(11, 4);
  delayMicroseconds(10);
  WriteReg(11, 0);
  delayMicroseconds(10);
  WriteReg(10, 8);
}

unsigned int stopNova(void)
{ WriteReg(11, 8);
  delayMicroseconds(10);
  WriteReg(11, 0);
  delayMicroseconds(10);
  return readAddr();
}

void continueNova(void)
{ writeInst(0xff<<8);
  WriteReg(11, 4);
  delayMicroseconds(10);
  WriteReg(11, 0);
  delayMicroseconds(10);
}              

void startNova(unsigned int a)
{ writeData(a);
  writeInst(0xfe<<8);
  WriteReg(11, 4);
  delayMicroseconds(10);
  WriteReg(11, 0);
  delayMicroseconds(10);
}              

////////////////////////////////////////////////////
// make string of given number of spaces
String makespc(int n)
{ String s;
  for (int i=0; i<n; i++) s.concat(" ");
  return s;
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

// convert 4 digit hex string in buffer
unsigned int makehex(byte *buf)
{ unsigned int r;
  for (int i=0; i<4; i++)
  { r = r<<4;
    if (buf[i]>'a') r = r+buf[i]-'a'+10;
    else if (buf[i]>'A') r = r+buf[i]-'A'+10;
    else r = r+buf[i]-'0';
  }
  return r;
}

////////////////////////////////////////////////////
// print octal on LCD with leading zeros
void lcdPrintOctal(unsigned int v) {
 lcd.print(toOct(v));
}

// print octal on LCD with leading zeros followed by space and two ascii characters
void lcdPrintOctalAscii(unsigned int v) {
 lcdPrintOctal(v);
 lcd.print(char(32));
 lcd.print(char(v>>8));
 lcd.print(char(v&255));
}

// print hex on LCD with leading zeros
void lcdPrintHex(unsigned int v) {
 lcd.print(toHex(v));
}

// print decimal on LCD with leading zeros
void lcdPrintDecimal(unsigned int v) {
 if (v<10000) lcd.print(0);
 if (v<1000) lcd.print(0);
 if (v<100) lcd.print(0);
 if (v<10) lcd.print(0);
 lcd.print(v);
}

// print long octal (21 bits) on LCD with leading zeros
void lcdPrintLongOctal(unsigned long v) {
 if (v<0x40000) lcd.print(0);
 if (v<0x08000) lcd.print(0);
 if (v<0x01000) lcd.print(0);
 if (v<0x00200) lcd.print(0);
 if (v<0x00040) lcd.print(0);
 if (v<0x00008) lcd.print(0);
 lcd.print(v, OCT);
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
String lcdPrintShift(unsigned int s) {
  if (s==0) return("");
  else if (s==1) return String("L");
  else if (s==2) return String("R");
  else if (s==3) return String("S"); // byte swap
}

// part of disassembler - print carry op in operate instruction
String lcdPrintCarry(unsigned int s) {
  if (s==0) return String("");
  else if (s==1) return String("Z");
  else if (s==2) return String("O");
  else if (s==3) return String("C");
}

// part of disassembler - print noload in operate instruction
String lcdPrintNoload(unsigned int s) {
  if (s==0)      return String("");
  else if (s==1) return String("#");
}

// part of disassembler - print skip part of operate instruction
String lcdPrintSkip(unsigned int s) {
  if (s==0) return String("");
  if (s==1) return String("skp");
  if (s==2) return String("szc");
  if (s==3) return String("snc");
  if (s==4) return String("szr");
  if (s==5) return String("snr");
  if (s==6) return String("sez");
  if (s==7) return String("sbn");
}

// part of disassembler - print target of memory access instruction
String lcdPrintTarget(unsigned int mode, unsigned int disp, int octalmode) {
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
String lcdPrintIOT(unsigned int tr, unsigned int sk) {
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

// Disassemble entire instruction and print to lcd
String lcdPrintDisas(unsigned int v, int octalmode) {
  String s(" ");
  unsigned int mref = (v>>11)&0x1f;
  unsigned int opcode = (v>>8)&0x7;

  // cpu control instructions
  if      ((v&0xff3f)==0x653f)  s+=("IORST"); //dicc0
  else if ((v&0xff3f)==0x663f)  s+=("HALT");//doc0
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
  else if (mref==0) { s+=("JMP"); s+=lcdPrintTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if (mref==1) { s+=("JMS"); s+=lcdPrintTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if (mref==2) { s+=("ISZ"); s+=lcdPrintTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if (mref==3) { s+=("DSZ"); s+=lcdPrintTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if ((mref&0x1c)==4) { s+=("LDA"); s+=String(mref&3); s+=lcdPrintTarget((v&0x700)>>8, v&0xff, octalmode); }
  else if ((mref&0x1c)==8) { s+=("STA"); s+=String(mref&3); s+=lcdPrintTarget((v&0x700)>>8, v&0xff, octalmode); }

  // IOT instructions
  else if ((mref&0x1c)==12)
  { s+=lcdPrintIOT((v&0x700)>>8, (v&0xc0)>>6); 
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
    s+=lcdPrintNoload((v&0x8)>>3);
    s+=String((mref&12)>>2); // source
    s+=String(mref&3);	     // destination
    s+=lcdPrintShift((v&0xc0)>>6);
    s+=lcdPrintCarry((v&0x30)>>4);
    s+=lcdPrintSkip(v&7);
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
  unsigned long op;
  unsigned long a=0;
  unsigned long b=0;
  unsigned long dest=0;

  if      ((v&0xff3f)==0x653f)  { insttype=1; op=0;}
  else if ((v&0xff3f)==0x663f)  { insttype=1; op=1;}
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
String lcdPrintMiniDisas(unsigned long v, int octalmode) {
  return lcdPrintDisas(packassemble(v), octalmode);
} 

void printAssemblyHelp(unsigned long v, int noctal)
{ int type = (v&07000000)>>18;
  printHelp("");

  if (noctal==0)            printHelp(F("1-7= special jmp ld/st inc ops I/O skip"));

  if (noctal==1 && type==1) printHelp(F("0-6 IORST HLT READS INTEN INTD MSKO INTA"));
  if (noctal==1 && type==2) printHelp(F("jumps 0=jmp 1=jms"));
  if (noctal==1 && type==3) printHelp(F("load/store 0=lda 1=sta"));
  if (noctal==1 && type==4) printHelp(F("inc/decrement skip if zero 0=isz 1=dsz"));
  if (noctal==1 && type==5) printHelp(F("ops 0..7=cmp neg mov inc adc sub add and"));
  if (noctal==1 && type==6) printHelp(F("I/O 0..6=nio dia doa dib dob dic doc"));
  if (noctal==1 && type==7) printHelp(F("skip on IO status 0=skip"));

  if (noctal==2 && type==1) printHelp(F("accumulator 0..3 = ac0..ac3"));
  if (noctal==2 && type==3) printHelp(F("accumulator 0..3 = ac0..ac3"));
  if (noctal==2 && type==5) printHelp(F("source accumulator 0..3 = ac0..ac3"));

  if (noctal==3 && type>=2 && type<=4) 
                            printHelp(F("0=abs 1=pc+ 2=ac2+ 3=ac3+ +4=indirect"));
  if (noctal==3 && type==5) printHelp(F("destination accumulator 0..3 = ac0..ac3"));
  if (noctal==3 && type==6) printHelp(F("IO function 0=none 1=set 2=clear 3=pulse"));
  if (noctal==3 && type==7) printHelp(F("skip 0=busy 1=nonbusy 2=done 3=notdone"));
  
  if (noctal==4 && type==5) printHelp(F("shift 0=none 1=L 2=R 3=byte swap"));
  if (noctal==5 && type==5) printHelp(F("carry 0=as is 1=Z 2=O 3=C +4=noload"));
  if (noctal==6 && type==5) printHelp(F("skip 0..7=never always NC C Z NZ NCZ CZ"));
  
  if (noctal>=4 && type>1 && type<=4) 
                            printHelp(F("offset +0..177 positive 377..200 negatve"));
  if (noctal>=4 && type>=6) printHelp(F("IO device 000..077"));
}

////////////////////////////////////////////////////
// mini debugger display on lcd in single step mode
void lcdPrintDebug(void) {
  lcd.clear();
  unsigned int pc = readAddr()&0x7fff;
  unsigned int carry = readAddr()&0x8000;
  unsigned int in = readData(), a0, a1, a2, a3;
  lcd.setCursor(0,0);		// print 4 accumulators
  if (largescreen==0)
  { lcdPrintHex(examineAC(0));
    lcdPrintHex(examineAC(1));
    lcdPrintHex(examineAC(2));
    lcdPrintHex(examineAC(3));
  }
  else if (largescreen==1)
  { lcd.print("ac0:");
    lcdPrintOctal(a0=examineAC(0));
    lcd.print(" 1:");
    lcdPrintOctal(a1=examineAC(1));
    lcd.print(" 2:");
    lcdPrintOctal(a2=examineAC(2));
    lcd.print(" 3:");
    lcdPrintOctal(a3=examineAC(3));
    lcd.print(" ");
    if (carry)
      lcd.print("C");
    else
      lcd.print("C");
  }
  in = examine(pc);
  lcd.setCursor(0,1);
  if (largescreen==0)
  { lcdPrintHex(pc);		// print address
    lcd.setCursor(4,1);
    lcd.print(lcdPrintDisas(in, HEX));		// disassemble instruction
  }
  else if (largescreen==1)
  { lcdPrintOctal(pc);    // print address
    lcd.setCursor(6,1);
    lcd.print(lcdPrintDisas(in, OCT));    // disassemble instruction

    int mref = (in>>11)&0x1f;
    int b=(in&0x700)>>8;
    int dest=in&0xff;
    bool target=false;
    int indirect=0;
    unsigned int t;
    
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
      lcd.print(" ");
      if (b>3)lcd.print("@");
      if (b>0)lcdPrintOctal(t);

      for (int i=0; i<indirect; i++)
      { do t=examine(t); while (t&0x8000);
      }
         
      if (indirect)
      { lcd.print("->");
        lcdPrintOctal(t);
      }
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
    Serial.print(lcdPrintDisas(in, OCT));    // disassemble instruction
  
    int mref = (in>>11)&0x1f;
    int b=(in&0x700)>>8;
    int dest=in&0xff;
    bool target=false;
    int indirect=0;
    unsigned int t;
  
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
      { do t=examine(t); while (t&0x8000);
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
// output debug info for arduino interface to serial port
void debugPrint(int a0, int a1, int inst, int func, unsigned int addr, unsigned int data) {
  Serial.print("a0: ");
  Serial.print(a0);
  Serial.print(" a1: ");
  Serial.print(a1);
  Serial.print(" inst: ");
  Serial.print(inst, HEX);
  Serial.print(" func: ");
  Serial.print(func);
  Serial.print(" Addr=");
  Serial.print(addr, HEX);
  Serial.print(" Data=");
  Serial.println(data, HEX);
}

////////////////////////////////////////////////////
// scan 4x3 keyboard array using 4 digital and one analog line
int readKeyBoard(void)
{ digitalWrite(A2, 1);
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

  int row=0, col=0;
  if (a>100 or b>100 or c>100 or d>100)
  { if (a>100) row = 3, col = a; 
    if (b>100) row = 2, col = b;
    if (c>100) row = 1, col = c;
    if (d>100) row = 4, col = d;
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

////////////////////////////////////////////////////////////////////////
// table for control keys on MSB board
//                    dep0,  dep1, dep2, dep3, exa0, exa1, exa2, exa3, reset, stop
short highvals[10] = {505,  405,  305,  205,   602, 723,  804,  950, 104, 1023}; 
short highinst[10] = {0x23, 0x2B, 0x33, 0x3B, 0x67, 0x6F, 0x77, 0x7F, 0x23, 0x00}; 
short highfuncs[10] = {1,    1,   1,    1,    1,     1,     1,   1,   2,   3};          
                      // 1,2,3=keys reset stop

// table for control keys on LSB board
//                   start  cont  depo depnex  exa  exanext memstep inststep n/a   pl
short lowvals[10]  = {448, 560,   338, 672,    227,  784,   118,    950,    32767, 1023};
short lowinst[10]  = {0xfb, 0xff, 0xdd, 0xdc,  0xf9, 0xfc,  0xff,   0xff,    0,   0xfd}; 
short lowfuncs[10] = {1,   1,    1,    1,     1,       1,  21,     22,      0,    7};    
                     //4,5,6,7= N/A memstep instep pl, +16 generate con+ signal

const long interval = 1;            // interval at which to process (milliseconds)
int debugging=0;                    // set after single instruction step
int count=0;                        // only used for running lights register test
int keyon = 0;                      // set when key is down
unsigned long previousMillis = 0;   // will store last time LED was updated
unsigned long keydownMillis = 0;    // will store time frontpanel key was pressed
unsigned long kbMillis = 0xfffffff; // will store time keypad key was pressed
int kbkey=0;			                  // detected key 1..12 on keypad
int kbmode=0, noctal;		            // mode for keypad 1=address, 2=data
unsigned long octalval;             // currently entered octal data
unsigned int octaladdress;          // last used octal address from keypad/front panel
unsigned int prevpc=0;              // used when stepping back in debugging
unsigned int opmode=0;              // opmode, set with key 9
                                    // 0=examine 1=decimal 2=asm 3=debug 4=setup 5=io
bool SerialIO=false;                // if set serial is sent/recieved to Nova

// -------------------- keyboard interface description ---------------------
//
// 9 mode (press long from in decimal or I/O mode)
// 0: examine mode
// 1: decimal mode
// 2: mini assembler mode
// 3: debug mode
// 4: setup mode --> 0=small LCD, 1=large LCD
// 5: I/O mode: keyboard and lcd connected to memory mapped I/O of Nova

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
// 0..5 sets mode

// IO mode: 9 (long) enters setup mode
// any character on keypad is written to Nova location 010
// character written to Nova location 011 is displayed on LCD
    
// ------------------------------------------------------------
// Serial interface description
//
// S              Stop
// O              Continue
// I              Instruction step
// Y              Memory step
// T              Reset
// Ghhhh          Start at address hhhh
// Mhhhh          Read memory at address hhhh
// Dhhhhhdddd     Deposit dddd at address hhhh
// En             Examine register n
// Fnhhhh         File register n with data hhhh
// Xn             Read lights (0=data, 1=address/carry, 2=lights)
// Pffhhhh        Panel function instruction ff data hhhh
// LprTEXT[cr]    Display text on LCD panel p, row r
// :nnaaaa[hhhh]  Deposit nn memory locations
// ?nnaaaa        Read nn memory locations
// /              enter memory mapped serial IO mode, @=exit
// V              display version information

// lnnaaaa        Disassemble nn memory locations
// r              Show registers
// i              Show disassembly then instruction step

void loop() {
  unsigned long currentMillis = millis();

  int inst=0;		// instruction to Nova
  int func=0;		// internal function number

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

    /////////////////////////// keypad pressed /////////////////////////
    // respond to keypad keys
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
        lcd.setCursor(0,0);
        lcd.print(makespc(40)); 
        lcd.noCursor();
        if (opmode==1)
          printHelp("* = goto");
        else
          printHelp("* = goto; 8 = backspace");
      }

      lcd.setCursor(noctal,0);
      lcd.print(kb); octalval = (opmode==1?10:8)*octalval+kb; noctal++; 
      lcd.noCursor();
    }
    else if (kbkey==8 && kbmode==1 && noctal>0 && opmode<3) // backspace in address mode
    { noctal--;
      octalval = octalval>>3;
      lcd.setCursor(noctal,0);
      lcd.print(" ");
    }

    else if (isoct && kbmode==2 && opmode==0) 	//  octal key in edit data mode
    { int kb=kbkey;
      if (kb==11) kb=0;
      int pos=(5-noctal)*3;
      octalval = (octalval&(~(7<<pos)))+(kb<<pos); noctal++; 
      octalval = octalval & 0xffff;
      lcd.setCursor(0,0);
      lcdPrintOctal(octaladdress);
      lcd.print(":");
      lcdPrintOctal(octalval);
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (octaladdress<0xfffc) 
      { if (largescreen) lcd.print("      :");
        lcd.print(lcdPrintDisas(octalval, largescreen?8:16));
      }
      else 
      { lcd.print("ac");
        lcd.print(octaladdress-0xfffc);
      }
      if (noctal>5) noctal=0;
      lcd.setCursor(7+noctal,0);
      lcd.cursor();
    }

    else if (isoct && kbmode==2 && opmode==1) 	//  numeric key in decimal mode
    { int kb=kbkey;
      if (kb==11) kb=0;
      int pos=6-noctal;
      octalval = 10*octalval + kb;
      noctal++; 
      octalval = octalval & 0xffff;
      lcd.setCursor(0,0);
      lcdPrintDecimal(octaladdress);
      lcd.print(":");
      lcd.print(octalval);
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (octaladdress>=0xfffc) 
      { lcd.print("ac");
        lcd.print(octaladdress-0xfffc);
      }
      if (noctal>6) noctal=0;
      lcd.noCursor();
    }

    else if (isoct && kbmode==2 && opmode==2)   //  octal key in assembly mode
    { unsigned long kb=kbkey;
      if (kb==11) kb=0;
      int pos=(6-noctal)*3;
      octalval = (octalval&(~((unsigned long)7<<pos)))+(kb<<pos); noctal++; 
      octalval = octalval & 0xffffff;
      lcd.setCursor(0,0);
      lcdPrintOctal(octaladdress);
      lcd.print(":");
      lcdPrintLongOctal(octalval);
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (octaladdress<0xfffc) 
      if (largescreen) lcd.print("      :");
      lcd.print(lcdPrintMiniDisas(octalval, largescreen?8:16));
      if (noctal>6) noctal=0;
      printAssemblyHelp(octalval, noctal);
      lcd.setCursor(7+noctal,0);
      lcd.cursor();
    }

    else if (kbkey==8 && kbmode==2 && opmode<=2) 	// arrow back data mode
    { if (noctal==0) noctal=opmode?6:5; else noctal--;
      lcd.setCursor(7+noctal,0);
      lcd.cursor();
      if (opmode==2) printAssemblyHelp(octalval, noctal);
    }
    else if (kbkey==9 && kbmode==2 && opmode<=2) 	// arrow forward data mode
    { noctal++; if (noctal>(opmode?6:5)) noctal=0;
      lcd.setCursor(7+noctal,0);
      lcd.cursor();
      if (opmode==2) printAssemblyHelp(octalval, noctal);
    }
 
    else if ((kbkey==10 && opmode<=3) or                        // * = examine / examine next
             (kbkey==8 && kbmode==1 && noctal==0 && opmode<=2)) // 8 = examine previous
    { if (kbmode==2) ; //
      else if (kbkey==8) octaladdress--;
      else if (noctal==0) octaladdress++;
      else if (noctal==1 && octalval<4) octaladdress = 0xfffc + (octalval&3);
      else octaladdress = octalval;
      lcd.setCursor(0,0);
      if (opmode==1)
        lcdPrintDecimal(octaladdress); 
      else
        lcdPrintOctal(octaladdress); 
      stopNova();
      lcd.print(":"); 
      if (opmode==1)
        lcdPrintDecimal(examine(octaladdress));
      else
        lcdPrintOctal(examine(octaladdress));
      lcd.print(makespc(40)); 
      kbmode=1;
      noctal=0;
      octalval=0;
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (octaladdress<0xfffc) 
      { if (largescreen) lcd.print("      :"); 
        lcd.print(lcdPrintDisas(examine(octaladdress), largescreen?8:16));
      }
      else 
      { lcd.print("ac");
        lcd.print(octaladdress-0xfffc);
      }
      lcd.noCursor();
      if (opmode==0 or opmode==2) printHelp("*/8 = next/prev address; # = edit");
      if (opmode==1) printHelp("* = next address; # = edit");
      if (opmode==3) printHelp("#=step, 8=step back");
    }
    else if (kbkey==12 && kbmode<2 && opmode==0) 	// # = start edit mode
    { kbmode = 2;
      noctal=0;
      octalval = examine(octaladdress);
      lcd.setCursor(0,0);
      lcd.print(makespc(40));
      lcd.setCursor(0,0);
      lcdPrintOctal(octaladdress); 
      lcd.print(":"); 
      lcdPrintOctal(octalval);
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (octaladdress<0xfffc) 
      { if (largescreen) lcd.print("      :"); 
        lcd.print(lcdPrintDisas(examine(octaladdress), largescreen?8:16));
      }
      else 
      { lcd.print("ac");
        lcd.print(octaladdress-0xfffc);
      }
      printHelp("8/9 = left/right; # = store; * = cancel");
      lcd.setCursor(7+noctal,0);
      lcd.cursor();
    }
    else if (kbkey==12 && kbmode<2 && opmode==1) 	// # = start decimal edit mode
    { kbmode = 2;
      noctal=0;
      //octalval = examine(octaladdress);
      octalval=0;
      lcd.setCursor(0,0);
      lcd.print(makespc(40));
      lcd.setCursor(0,0);
      lcdPrintDecimal(octaladdress); 
      lcd.print(":"); 
      lcd.print(octalval);
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      stopNova();
      if (octaladdress<0xfffc) 
      { if (largescreen) lcd.print("      :"); 
        lcd.print(lcdPrintDisas(examine(octaladdress), largescreen?8:16));
      }
      else 
      { lcd.print("ac");
        lcd.print(octaladdress-0xfffc);
      }
      printHelp("decimal mode # = store; * = cancel");
      //lcd.setCursor(6+noctal,0);
      lcd.noCursor();
    }
    else if (kbkey==12 && kbmode==2 && (opmode==0 or opmode==2)) 	// # = edit mode deposit
    { if (opmode==2) octalval=packassemble(octalval);
      stopNova();
      deposit(octaladdress, octalval);
      lcd.setCursor(0,0);
      lcdPrintOctal(octaladdress); 
      lcd.print("<"); 
      lcdPrintOctal(examine(octaladdress));
      lcd.print(makespc(40)); 
      kbmode=1;
      noctal=0;
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (octaladdress<0xfffc) 
      { if (largescreen) lcd.print("      <");
        lcd.print(lcdPrintDisas(examine(octaladdress), largescreen?8:16));
      }
      else 
      { lcd.print(" ac");
        lcd.print(octaladdress-0xfffc);
      }
      printHelp("");
      lcd.noCursor();
    }
    else if (kbkey==12 && kbmode==2 && opmode==1)   // # = decimal mode deposit
    { stopNova();
      deposit(octaladdress, octalval);
      lcd.setCursor(0,0);
      lcdPrintDecimal(octaladdress); 
      lcd.print("<"); 
      lcdPrintDecimal(examine(octaladdress));
      lcd.print(makespc(40)); 
      kbmode=1;
      noctal=0;
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (octaladdress>=0xfffc) 
      { lcd.print(" ac");
        lcd.print(octaladdress-0xfffc);
      }
      printHelp("");
      lcd.noCursor();
    }
    else if (kbkey==12 && kbmode<2 && opmode==2)   // # = start assembly mode
    { stopNova();
      kbmode = 2;
      noctal=0;
      octalval = examine(octaladdress);
      octalval=unpackassemble(octalval);
      lcd.setCursor(0,0);
      lcd.print(makespc(40));
      lcd.setCursor(0,0);
      lcdPrintOctal(octaladdress); 
      lcd.print(":"); 
      lcdPrintLongOctal(octalval);
      lcd.setCursor(0,1);
      lcd.print(makespc(40));
      lcd.setCursor(0,1);
      if (largescreen) lcd.print("      :"); 
      lcd.print(lcdPrintMiniDisas(octalval, largescreen?8:16));
      printAssemblyHelp(octalval, noctal);
      lcd.setCursor(7+noctal,0);
      lcd.cursor();
    }
    else if (opmode==3 && kbkey==12)      // step instruction
    { stopNova();
      prevpc = readAddr() & 0x7fff;
      func=22;
      kbmode=0;
      noctal=0;
      octalval=0;
      lcd.noCursor();
    } 
    else if (opmode==3 && kbkey==8 && debugging)    // 'step back' instruction
    { stopNova();
      octaladdress = prevpc;
      prevpc = octaladdress-1;
      examine(octaladdress);
      lcd.setCursor(0,1);
      lcdPrintDebug(); 
      lcd.noCursor();
      kbmode=1;
      noctal=0;
      octalval=0;
      lcd.noCursor();
    }
    else if (opmode==5 && kbkey>0)  // io mode
    { unsigned int a, kb=kbkey;
      for (int i=0; i<1000; i++) 
      { a=stopNova();
        if (examine(8)==0) break;
        if (examine(9)) 
        { Serial.write(examine(9));
          lcd.write(examine(9));
          deposit(9, 0);
        }
        startNova(a);
        delay(1);
      };
      if (kb==10) kb='*';
      else if (kb==11) kb='0';
      else if (kb==12) kb='#';
      else kb='0' + kb;
      deposit(8, kb);
      startNova(a);
    }
    else if (kbmode<3 && kbkey==9)      // select opmode
    { kbmode = 3;
      lcd.setCursor(0, 1);
      lcd.print("0-5=exam decimal asm debug setup i/o");
      printHelp("Select run mode");
      while(readKeyBoard()!=0);
      delay(50);
      lcd.noCursor();
    }
    else if (kbmode==3 && kbkey>0)      // select opmode
    { int kb=kbkey;
      if (kb==11) kb=0;
      kbmode = 0;
      if (kb<=5) opmode=kb;
      lcd.setCursor(0, 1);
      lcd.print(makespc(40));
      if (opmode==0) printHelp(F("N*=AC NN*=exam mem #=edit 9=setup"));
      if (opmode==1) printHelp(F("N*=AC NN*=exam mem #=edit dec 9!=setup"));
      if (opmode==2) printHelp(F("N*=AC NN*=exam mem #=edit asm 9=setup"));
      if (opmode==3) printHelp(F("debug #=step 8=step back 9=setup"));
      if (opmode==4) printHelp(F("setup 0=small screen 1=large screen"));
      if (opmode==5) { printHelp(F("I/O mode 9(long)=exit")); lcd.clear(); }
      lcd.noCursor();
    }
    else if (opmode==4 && kbkey>0)      // setup
    { int kb=kbkey;
      if (kb==11) kb=0;
      largescreen = min(kb,1);
      EEPROM.write(0, largescreen);
      lcd.clear();
      kbmode = 0;
      opmode = 0;
      printHelp("");
      lcd.noCursor();
    }
    else if (kbkey>0) 			// unrecognised key
    { kbmode=0;
      noctal=0;
      octalval=0;
      lcd.noCursor();
    }

    /////////////////////////// read switch network /////////////////////////
    int a0, a1;
    // read toggle switches using resistor network, ignore LSB switches in serial mode
    a1 = (analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1))/6;
    if (SerialIO or opmode==5)
      a0=0;
    else
      a0 = (analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0)+analogRead(A0))/6;

    int orga1=a1;
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
        
        // register test: disconnect boards from nova and connect data req or inst req to gnd
        if (func==0) // 1st switch up
        { lcd.setCursor(0,1);
          lcdPrintOctal(count);
          lcd.print("  reg test");
          writeData(32768>>(count&15));
          writeInst(1<<(count&15));
          //debugPrint(a0, a1, 11, 11, readAddr(), readData()); 
          count++;
        }

        // full memory dump
        if (func==1) // 2nd switch up
        { stopNova();
          lcd.setCursor(0,1);
          lcd.print("     dump active");
          lcd.setCursor(0,1);
          for(int a=0; a<8192; a++)
          { if ((a&511)==511) lcd.print("#");
            int v = examine(a);
            Serial.print(toHex(a));
            Serial.print(":");
            Serial.println(toHex(v));
          }
          lcd.setCursor(0,1);
          lcd.print("dump ready      ");
        }

        // small memory test, address 0 only
        if (func==2) // 3rd switch up
        { stopNova();
          lcd.setCursor(0,1);
          lcd.print("     mt0 active");
          for(unsigned int a=0; a<65535; a++)
          { unsigned int b = a&1?0x5555:0xaaaa;
            deposit(0, b);
		        unsigned int c = examine(0);
			      digitalWrite(13, b!=c);
          }
          lcd.setCursor(0,1);
          lcd.print("mt0 ready      ");
        }

        // small memory test, address 0 only
        if (func==3) // 4rd switch up
        { stopNova();
          lcd.setCursor(0,1);
          lcd.print("     mt1 active");
          for(unsigned int a=0; a<65535; a++)
          { deposit(0, 0); // drive inhibit
          }
          lcd.setCursor(0,1);
          lcd.print("mt1 ready      ");
        }

        // small memory test, address 0 only
        if (func==4) // 5th switch up (1st down?)
        { stopNova();
          lcd.setCursor(0,1);
          lcd.print("     mt2 active");
          for(unsigned int a=0; a<65535; a++)
          { deposit(0, 0xffff); // no inhibits driven
          }
          lcd.setCursor(0,1);
          lcd.print("mt2 ready      ");
        }
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
    else if (kbmode==0)
    { keyon=0;
      if (opmode!=5)
      { lcd.setCursor(0,0);
        if (!debugging) lcd.print("DGC    Nova 1210");
        if (!debugging) lcd.print(makespc(24));
      }
    }
    count++;

    // auto repeat all functions after 1 second
    if (func!=0 && millis()-keydownMillis > 1000)
    { keyon=0;
      delay(100);
      if (a0>1000) // display hint for test functions
      { lcd.setCursor(0,0);
        lcd.print(makespc(40));
        lcd.setCursor(0,0);
        lcd.print("TST DMP MT0 1 2");
      }  
    }
      
    // frontpanel keys force keypad to go to inactive mode
    if (func>0 && func!=22) kbmode=0;

    //////////////////////////////// serial IO ///////////////////////////
    unsigned int a=0;
    if (SerialIO or opmode==5)
    { a=stopNova();
      if (examine(9)) 
      { Serial.write(examine(9));
        if (opmode==5) 
          lcd.write(examine(9));
        deposit(9, 0);
      }
      startNova(a);
    }

    if (Serial.available())
    { byte b = Serial.read();
      byte m[50];

      if (SerialIO or opmode==5) 
      { if (b=='@') 
        { SerialIO=false;
          Serial.println("interactive mode");
        }
        else
        { for (int i=0; i<1000; i++) 
          { a=stopNova();
            if (examine(8)==0) break;
            if (examine(9)) 
            { Serial.write(examine(9));
              if (opmode==5) lcd.write(examine(9));
              deposit(9, 0);
            }
            startNova(a);
            delay(1);
          };
          deposit(8, b);
          startNova(a);
        }
      }
      else
        switch(b)
      { case 'T': func=2; inst=0; break;
        case 'S': stopNova(); 
                  if(SerialIO or opmode==5) 
                  { Serial.println("interactive mode");
                    SerialIO=false; opmode=0; 
                  }  
                  break;
        case 'I': func=22; inst=0; break;
        case 'Y': func=21; inst=0; break;
        case 'O': continueNova(); break;
        case 'G': Serial.readBytes(m, 4);
                  startNova(makehex(m));
                  //func=33; inst=0xfb; 
                  //writeData(makehex(m)); 
                  break;
        case 'M': Serial.readBytes(m, 4); 
                  Serial.println(toHex(examine(makehex(m)))); 
                  break;
        case 'D': Serial.readBytes(m, 8); 
                  deposit(makehex(m), makehex(m+4)); break;
        case 'E': Serial.readBytes(m, 1); 
                  Serial.println(toHex(examineAC(m[0]-'0'))); break;
        case 'F': Serial.readBytes(m, 5); 
                  depositAC(m[0]-'0', makehex(m+1)); break;
        case 'X': Serial.readBytes(m, 1); 
                  if (m[0]=='0') Serial.println(toHex(readData())); 
                  if (m[0]=='1') Serial.println(toHex(readAddr())); 
                  if (m[0]=='2') Serial.println(toHex(0/*readLights()*/)); 
                  break;
        case 'P': Serial.readBytes(m, 6); 
                  func=33; inst=makehex(m)>>8; 
                  writeData(makehex(m+2)); 
                  break;
        case 'L': {int l=Serial.readBytesUntil(0x0d, m, 42);
                  if (m[0]=='0') 
                  { lcd.setCursor(0, m[1]-'0');
                    lcd.print(makespc(40));
                    lcd.setCursor(0, m[1]-'0');
                    for (int i=2; i<l; i++) lcd.write(char(m[i]));
                  }
                  if (m[0]=='1') 
                  { lcd2.setCursor(0, m[1]-'0');
                    lcd2.print(makespc(40));
                    lcd2.setCursor(0, m[1]-'0');
                    for (int i=2; i<l; i++) lcd2.write(char(m[i]));
                  }
                  break;
                  }
        case ':': {
                  Serial.readBytes(m, 6);
                  unsigned int n = makehex(m)>>8;
                  unsigned int a=makehex(m+2);
                  Serial.readBytes(m, n*4); 
                  for (int i=0; i<n; i++) deposit(a++, makehex(m+i*4));
                  break;
                  }
        case '?': {
                  Serial.readBytes(m, 6);
                  unsigned int n = makehex(m)>>8;
                  unsigned int a=makehex(m+2);
                  for (int i=0; i<n; i++) Serial.println(toHex(examine(a++)));
                  break;
                  }
        case 'l': {
                  Serial.readBytes(m, 6);
                  unsigned int n = makehex(m)>>8;
                  unsigned int a = makehex(m+2);
                  for (int i=0; i<n; i++) 
                  { Serial.print(toOct(a));
                    unsigned int v=examine(a++);
                    Serial.print(" ");
                    Serial.print(toOct(v));
                    Serial.println(lcdPrintDisas(v, OCT));
                  }
                  break;
                  }
        case 'r': serialDebug(1);
                  break;
        case 'i': serialDebug(2);
                  Serial.println();
                  func=22; inst=0;
                  break;
        case '/': SerialIO=true;
                  Serial.println("IO mode; @=exit");
                  break;
        case 'V': Serial.println("Arduino code Marcel van Herk 20170829");
                  break;
      }
    }

    //////////////////// process all frontpanel keys //////////////////
    if (func!=0 && keyon==0)
    { keyon = 1;

      lcd.setCursor(0,1);

      writeInst(inst<<8);	// set instruction register on Nova
  
      int pulselen=1;

      // regular keys work via conreq (sw request) / coninst (hw response)
      if (func==1 or func==33)
      { if (func==33) WriteReg(10, 0);	// connect register to data bus
        else WriteReg(10, 8); // connect keys to data bus
        WriteReg(11, 4);	// pulse conreq

        // debug output
        if (inst==0xfb)      Serial.println("start");
        else if (inst==0xff) Serial.println("continue");
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
        lcd.setCursor(0,1);
        lcd.print(makespc(40));
        lcd.setCursor(0,1);
        if      ((inst&0xe0)==0x60) 
        { lcd.print("ac"); lcd.print((inst>>3)&3); lcd.print(":"); octaladdress = 0xfffc+((inst>>3)&3);}
        else if ((inst&0xe0)==0x20) 
	      { lcd.print("ac"); lcd.print((inst>>3)&3); lcd.print("<"); octaladdress = 0xfffc+((inst>>3)&3); }
        else if ((inst&0xf0)==0xf0)  
        { lcdPrintOctal(readAddr()&0x7fff); lcd.print(":"); octaladdress = readAddr()&0x7fff; }
        else if ((inst&0xf0)==0xd0) 
        { lcdPrintOctal(readAddr()&0x7fff); lcd.print("<"); octaladdress = readAddr()&0x7fff; }
        lcdPrintOctalAscii(readData());
        debugging=0;		// stop single step debug output
      }
      if (func==2)
      { stopNova();
        WriteReg(10, 12);
        WriteReg(11, 6);
        delay(100);
        Serial.println("reset");
        WriteReg(11, 0);
        WriteReg(10, 8);
        lcd.setCursor(0,1);
        lcd.print(makespc(40));
        lcd.setCursor(0,1);
        lcd.print("reset"); 
        debugging=0;
      }
      if (func==3)
      { stopNova();
        Serial.println("stop");
        if(SerialIO or opmode==5) 
        { Serial.println("interactive mode");
          SerialIO=false; opmode=0; 
        }  
        lcd.setCursor(0,1);
        lcd.print(makespc(40));
        lcd.setCursor(0,1);
        lcd.print("#");
        lcdPrintOctal(readAddr()&0x7fff); lcd.print(":");
        lcdPrintOctal(readData());      
        debugging=0;
      }
      if (func==21)
      { stopNova();
        WriteReg(11, 7);
        delay(pulselen);
        Serial.println("memory step");
        WriteReg(11, 0);
        lcd.setCursor(0,1);
        lcd.print(makespc(40));
        lcd.setCursor(0,1);
        lcd.print("$");
        lcdPrintOctal(readAddr()&0x7fff); lcd.print(":");
        lcdPrintOctal(readData());      
        debugging=0;
      }
      if (func==22)		// single step shows debugger output
      { stopNova();
        prevpc=readAddr();
        WriteReg(10, 9);
        WriteReg(11, 6);
        delay(pulselen);
        Serial.println("instruction step");
        WriteReg(11, 0);
        WriteReg(10, 8);
        lcdPrintDebug();
        debugging=1;
      }
      if (func==7)
      { WriteReg(10, 10);
        delay(pulselen);
        Serial.println("program load");
        WriteReg(10, 8);
        lcd.setCursor(0,1);
        lcd.print(makespc(40));
        lcd.setCursor(0,1);
        lcd.print("progload/special"); 
        debugging=0;
      }
    }
    else if (func==0)
      keyon=0;
  }
}
