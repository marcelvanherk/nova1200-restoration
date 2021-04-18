// Novarev3PCB frontpanel monitor software
// Marcel van Herk

// 20200705: first version with most functionality
// put keyboard based display on LCD 3 (move to 0?)
// EEPROM address = 50..53
// switch banks not yet implemented
// uses Serial1 to connect to Nova
// Tested: LCDs leds keyboards regs eeprom0
// TODO lights: eepromread eepromwrite memrd memwr serialrd serialwr
// TODO implement Nova switches A--T (T=test)
// TODO possible tests: LCD, keys&switch, lights, leds, novaregs, serial, eeproms, analog, 
// (spi, extlcd, wl433.92, int, lightint), 
// 20200705: adc N, tested dac, lcd writes big, led does color lights and teensy led
//           display 0=keypad monitor, 3=serial out, readk, reads commands
// 20200707: added Nova virtual console K mode to load dgnasm output
//           removed old interactive mode; fix keypad mode display with cursor; allow empty help
// 20200708: modified test 9 6 7: test lights unconnected to Nova (was assemble)
// 20200709: Added SIMULATED, key repeat, buffer lcd help string; implement most switches
// 20200710: Added wireFail flag: detects if no hardware connected and switches of I2C to enable serial control only; blink led on boot
// 20200711: Added basic command (loads basic code program memory); lights for eeprom read and write; start on simulator
// 20200712: Removed basic command now tape .basic; also works without eeprom; cleaned nova.simh; started on TTO/TTI
//           Commands step and run now do something on simulator
// 20200712: First working version
// 20200713: TTO works when calling from nova.simh; lights: C, ION, RUN,FETCH,DEFER=running, EXEC=off
//           Prog load is restore (down) store (up); Todo: step key
//           Accelerated EEPROM read; basic2 tape; fix simulated step;
// 20200714: Allow to work on Teensy3.5 with few lights and switches and single I2C display
// 20200717: Removed c_str casts, few TEENSY35 fixes, I2C delay to 1us (0us works too, but 1 more stable)
// 20210411: Moved basic to header files, simh is  now .h file, few merged changes
// 20210411: Some work for not SIMULATED mode; fix readData, readAddr, readLights polarity; tested on Nova OK
// 20210414: Fix reading of carry, is on readLights not readAddr in rev3; port 6 bit 3 does not need control in rev3 pcb
// 20210418: Added animation of color leds in testmode 7

#include <U8g2lib.h>
#include <EEPROM.h>
#define TEENSY

#ifdef __MK64FX512__
#define TEENSY35
#endif

// graphics library
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, A9, A8); 
U8G2LOG u8g2log;

#define SIMULATED

int NovaPC=0;

#ifdef SIMULATED
  unsigned short NovaMem[16384];
  int NovaRegs[4];
  int NovaC=0;

void Serialwrite(int a)
{ Serial.write(a);
#ifdef TEENSY35
  selectDisplay(0);
  u8g2log.write(a);
#endif
}

#define MEMSIZE 16384
#define M NovaMem
#define AC NovaRegs
#define PC NovaPC
#define C NovaC
#include "nova.simh.h"

#endif

#include <SoftWire.h>
SoftWire softwarei2c(A8, A9);
#define Wire softwarei2c

// I2C suppport routines
bool wireFail=false;

void WireEnd() {
  if (!wireFail);
    Wire.end();
}

void WireBegin() {
  static bool configured = false;
  static byte rxbuffer[64];
  static byte txbuffer[64];
  if (!configured) {
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
    if (analogRead(A8)<1000) wireFail=true;
    if (analogRead(A8)<1000) wireFail=true;
    if (wireFail) return;
    pinMode(A8, OUTPUT);
    pinMode(A9, OUTPUT);
    Wire.setDelay_us(1);
    Wire.setRxBuffer(rxbuffer, 64);
    Wire.setTxBuffer(txbuffer, 64);
    configured = true;
  }
  Wire.begin();
}

void writeByte(byte chip, byte reg, byte data) {
  if (wireFail) return;
  Wire.beginTransmission(chip);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte readByte(byte chip, byte reg) {
  if (wireFail) return 0;
  Wire.beginTransmission(chip);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(chip, (byte)1);
  Wire.available();
  return Wire.read();
}

// write Nova Data bus, should not be called when physically connected to Nova
int NovaModeD=0;

void writeData(int A)
{ WireBegin();
  static bool configured=false;
  if (NovaModeD!=2 || !configured) {
    writeByte(0x22, 0x01, 0x00);
    writeByte(0x23, 0x01, 0x00);
    NovaModeD=2; configured=true;
  }
  writeByte(0x22, 0x13, ~A);
  writeByte(0x23, 0x13, ~(A>>8));
  WireEnd();
}

// read Nova Data bus, should only be called when physically connected to Nova
unsigned short readData()
{ WireBegin();
  static bool configured=false;
  if (NovaModeD!=1 || !configured) {
    writeByte(0x22, 0x01, 0xff);
    writeByte(0x23, 0x01, 0xff);
    NovaModeD=1; configured=true;
  }
  unsigned short a=readByte(0x22, 0x13);
  unsigned short b=readByte(0x23, 0x13);
  return ~((b<<8)|a);
  WireEnd();
}

// write Nova Address bus, should not be called when physically connected to Nova
int NovaModeA=0;
void writeAddr(int A)
{ WireBegin();
  static bool configured=false;
  if (NovaModeA!=2 || !configured) {
    writeByte(0x22, 0x00, 0x00);
    writeByte(0x23, 0x00, 0x00);
    NovaModeA=2; configured=true;
  }
  writeByte(0x22, 0x12, ~A);
  writeByte(0x23, 0x12, ~(A>>8));
  WireEnd();
}

// read Nova Address bus, should only be called when physically connected to Nova
unsigned short readAddr()
{ WireBegin();
  static bool configured=false;
  if (NovaModeA!=1 || !configured) {
    writeByte(0x22, 0x00, 0xff);
    writeByte(0x23, 0x00, 0xff);
    NovaModeA=1; configured=true;
  }
  unsigned short a=readByte(0x22, 0x12);
  unsigned short b=readByte(0x23, 0x12);
  WireEnd();
  return ~((b<<8)|a|0x8000);
}

// write Nova status lights, should not be called when physically connected to Nova
int NovaModeL=0;
void writeLights(int A)
{ 
#ifdef TEENSY35
  return;
#endif
  WireBegin();
  static bool configured=false;
  if (NovaModeL!=2 || !configured) {
    writeByte(0x24, 0x01, 0x00);
    NovaModeL=2; configured=true;
  }
  writeByte(0x24, 0x13, ~A);
  WireEnd();
}

// read Nova status lights, should only be called when physically connected to Nova
byte readLights()
{ WireBegin();
  static bool configured=false;
  if (NovaModeL!=1 || !configured) {
    writeByte(0x24, 0x01, 0xff);
    NovaModeL=1; configured=true;
  }
  byte a = readByte(0x24, 0x13);
  WireEnd();
  return ~a;
}

// read switch bank
int readSwitches(void) {
  static bool configured = false;
  WireBegin();
  if(!configured) {
    writeByte(0x20, 0x0c, 0xff);
    writeByte(0x20, 0x0d, 0xff);
    writeByte(0x20, 0x00, 0xff);
    writeByte(0x20, 0x01, 0xff);
    configured = true;
  }
  short a = readByte(0x20, 0x12);
  short b = readByte(0x20, 0x13);
  WireEnd();
  return (~((a<<8)+b))&0xffff;
}

// enable ONE color led (1..10) from matrix next to control switches, 0 = disable, -1=restore output
void writeColor(int i) {
#ifdef TEENSY35
  if (i<0) return;
  i = ~i;
  for (byte a=18;  a<22; a++) pinMode(a, OUTPUT); 
  if (i&1) pinMode(18, INPUT); 
  if (i&2) pinMode(19, INPUT); 
  if (i&4) pinMode(20, INPUT); 
  if (i&8) pinMode(21, INPUT); 
  digitalWrite(18, i&1); // output 4bit data
  digitalWrite(19, (i>>1)&1);
  digitalWrite(20, (i>>2)&1);
  digitalWrite(21, (i>>3)&1);
  return;
#endif
  static bool configured = false;
  static int prevcolor=0;
  if (i<0) {
    writeByte(0x21, 0x13, prevcolor);
    return;
  }

  WireBegin();
  if (!configured) {
    writeByte(0x21, 0x01, 0x00);
    configured = true;
  }

  i %= 11;
  if (i==0)     ;
  else if (i>5) i = 64+(1<<((i-1)%5));
  else          i = 128+(1<<((i-1)%5));
  writeByte(0x21, 0x13, i);
  prevcolor=i;
  WireEnd();
}

// adapted from I2C eeprom library from https://playground.arduino.cc/code/I2CEEPROM
// added Wire.begin and Wire.end to allow sharing with u8g2
 
// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page(int chip, unsigned int eeaddresspage, byte* data, byte length ) 
{ if (wireFail) return;
  writeColor(1);
  WireBegin();
  Wire.beginTransmission(chip);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  for (byte c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
  delay(5);
  WireEnd();
  writeColor(0);
}

void i2c_eeprom_read_buffer(int chip, unsigned int eeaddress, byte *buffer, int length) 
{ if (wireFail) return;
  writeColor(2);
  WireBegin();
  Wire.beginTransmission(chip);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(chip,length);
  for (int c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
  WireEnd();
  writeColor(0);
}

int counteRead=0;
byte i2c_eeprom_read_byte(int chip, unsigned int eeaddress) 
{ if (wireFail) return 0;
  byte rdata = 0xFF;
  if (++counteRead>10) writeColor(2);
  WireBegin();
  Wire.beginTransmission(chip);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(chip,1);
  if (Wire.available()) rdata = Wire.read();
  WireEnd();
  if (counteRead>10) 
  { writeColor(0);
    counteRead=0;
  }
  return rdata;
}

// 5 x 8 keycodes of keypad and switch bank of Nova version 3 frontpanel
// switch bank layout A  C  E  G  I   K  M  O  Q  S
//                    B  D  F  H  J   L  N  P  R  T

char keyCodes[] = {'T', 'R', 'P', 'N', 'L',   'S', 'Q', 'O', 'M', 'K', 
                   'J', 'H', 'F', 'D', 'B',   'I', 'G', 'E', 'C', 'A',
                    0,  '8', '5', '0', '2',    0,  '7', '4', '*', '1',
                    0,  '9', '6', '#', '3',    0,  'W', 'V', 'X', 'U'};
                 // 0=N/A                          7+9  4+6  *+#  1+3 (when pressing multiple keys at once)

// read keyboard and/or control switches matrix, note keyboard matrix drive multiplexed with color leds
int readKeys(void) {
#ifdef TEENSY35
  for (byte a=2; a<11; a++) pinMode(a, INPUT_PULLUP);
  if(digitalRead(2)==0) {delay(50); if(digitalRead(2)==0) return '0'; writeColor(1);}
  if(digitalRead(4)==0) {delay(50); if(digitalRead(4)==0) return '1'; writeColor(2);}
  if(digitalRead(7)==0) {delay(50); if(digitalRead(7)==0) return '2'; writeColor(4);}
  if(digitalRead(9)==0) {delay(50); if(digitalRead(9)==0) return '3'; writeColor(0);}
  if(digitalRead(5)==0) {delay(50); if(digitalRead(5)==0) return '4'; }
  if(digitalRead(8)==0) {delay(50); if(digitalRead(8)==0) return '5'; }
  if(digitalRead(10)==0) {delay(50); if(digitalRead(10)==0) return '6'; } 
  if(digitalRead(3)==0) {delay(50); if(digitalRead(3)==0) return '7'; } 
  return 0;
#endif
  WireBegin();
  static bool configured = false;
  if (!configured) {
    writeByte(0x21, 0x01, 0x00);
    writeByte(0x21, 0x0c, 0xff);
    writeByte(0x21, 0x00, 0xff);
    configured = true;
  }

  // any switch pressed (6 rows) ?
  writeByte(0x21, 0x13, 0xC0);
  short a = readByte(0x21, 0x12);
  if (a!=255) {
    short s;
    writeColor(-1);
    for (int i=0; i<5; i++) {   // scan 6 rows
      writeByte(0x21, 0x13, ~(1<<i));
      s = readByte(0x21, 0x12);
      writeColor(-1);
      if (s!=255) {
        a=0;
        switch(s)               // test 5 columns
        { case 0xfe: a= i+0; break;
          case 0xfd: a= i+5; break;
          case 0xfb: a= i+10; break;
          case 0xf7: a= i+15; break;
          case 0xef: a= i+20; break;
          case 0xdf: a= i+25; break;
          case 0xbf: a= i+30; break;
          default:   a= i+35; break; // multiple switches
        }
        writeColor(-1);
        WireEnd();
        return keyCodes[a];
      }
    }
  }
  writeColor(-1);
  WireEnd();
  return 0;
}

/*
Register list on 74LS173 chips:
0 = write lsb data 12-15
1 = write lsb data 8-11
2 = write msb data 4-7 (in rev2 was 8)
3 = write msb data 0-3 (was 9)
4 = write inst 12-15 (was 2)
5 = write inst 8-11 (was 3)
6 = write control L (was 10)
7 = write control H (was 11)
*/

// write Nova Data or Instruction register nibble to 74LS173 register
void WriteReg(byte chip, byte value) {
#ifdef TEENSY35
  return;
#endif
  value=~value;      // negate value to make active high
  static bool configured = false;
  if (!configured) {
    for (byte i=6;  i<9; i++) pinMode(i, OUTPUT); 
    for (byte i=18; i<22; i++) pinMode(i, OUTPUT); 
    pinMode(3, OUTPUT);
    digitalWrite(3, 1);
    configured = true;
  }

  digitalWrite(18, value&1); // output 4bit data
  digitalWrite(19, (value>>1)&1);
  digitalWrite(20, (value>>2)&1);
  digitalWrite(21, (value>>3)&1);
  
  digitalWrite(6, chip&1);     // output 3bits address
  digitalWrite(7, (chip>>1)&1);
  digitalWrite(8, (chip>>2)&1);

  delayMicroseconds(1);     // pulse SELR1 low
  digitalWrite(3, 0);
  delayMicroseconds(1);
  digitalWrite(3, 1);
}

void writeDataReg(unsigned short d)    // set data register
{ WriteReg(0, d&15);        // data L LSB
  WriteReg(1, (d>>4));      // data H LSB
  WriteReg(2, (d>>8));      // data L MSB
  WriteReg(3, (d>>12));     // data H MSB
}

void writeInstReg(byte i)    // set instruction register (only 8 Nova MSB bits used)
{ WriteReg(4, i);           // data L MSB
  WriteReg(5, (i>>4));      // data H MSB
}

void writeControlReg(byte i) // set control register (unused, always use WriteReg directly)
{ WriteReg(6, i);      // ISTP/PL/RST/X
  WriteReg(7, i>>4);   // MSTP/CON+/CONREQ/STOP
}

void wait(void) {
  delayMicroseconds(2);
}

unsigned int CurrentSwitchValue=0;

// read nova accumulator 0..3
unsigned int examineAC(int address) {
#ifdef SIMULATED
  writeData(NovaRegs[address&3]);
  return NovaRegs[address&3];
#endif
  writeInstReg(0x67+(address<<3));
  WriteReg(7, 4);
  wait();
  WriteReg(7, 0);
  wait();
  int v = readData();
  return v;
}

// read single word from nova memory or register
unsigned int examine(int address) {
  if (address>=0xfffc)
    return examineAC(address-0xfffc);
#ifdef SIMULATED
  unsigned short int w=0;
  writeAddr(address);
  if (address<16384) w = NovaMem[address];
  writeData(w);
  NovaPC = address;
  return w;
#endif
  writeInstReg(0xf9);
  writeDataReg(address);
  WriteReg(7, 4);  // pulse CONREQ
  wait();
  WriteReg(7, 0);  // revert to idle state
  wait();
  int v = readData(); // read lights
  writeDataReg(CurrentSwitchValue);
  return v;
}

// write single word to nova accumulator
void depositAC(unsigned int address, unsigned int value) {
#ifdef SIMULATED
  writeData(value);
  NovaRegs[address&3] = value;
  return;
#endif
  writeInstReg(0x23+(address<<3));
  writeDataReg(value);
  WriteReg(7, 4);
  wait();
  WriteReg(7, 0);
  wait();
  writeDataReg(CurrentSwitchValue);
}

// write single word to nova memory or accumulator
void deposit(unsigned int address, unsigned int value) {
  if (address>=0xfffc) 
  { depositAC(address-0xfffc, value);
    return;
  }
#ifdef SIMULATED
  writeAddr(address);
  writeData(value);
  if (address>=16384) return;
  NovaMem[address] = value;
  return;
#endif
  writeInstReg(0xf9);
  writeDataReg(address);
  WriteReg(7, 4);
  wait();
  WriteReg(7, 0);
  wait();
  writeInstReg(0xdd);
  writeDataReg(value);
  WriteReg(7, 4);
  wait();
  WriteReg(7, 0);
  wait();
  writeDataReg(CurrentSwitchValue);
}

bool novaRunning=false;
unsigned int prevpc=0;              // used when stepping back in debugging
int count9=0;
bool SerialIO=false;               // if set serial is sent/recieved to Nova

unsigned int stopNova(void)
{ 
#ifdef SIMULATED
  novaRunning=false;
  return saved_PC;
#endif
  
  WriteReg(7, 8);
  wait();
  WriteReg(7, 0);
  wait();
  novaRunning=false;
  return readAddr();
}

// number of instructions per update of front panel
#define SIMINTERVAL 2000

// start at address a
void startNova(unsigned int a)
{
#ifdef SIMULATED
  int count;
  count9=0;
  novaRunning=true;
  prevpc=a;
  saved_PC=a;
  writeLights(1);
  while(novaRunning)
  { sim_instr(SIMINTERVAL);
    NovaPC=saved_PC;
    writeAddr(saved_PC);
    writeData(examine(saved_PC));
    writeLights(
      (novaRunning?1:0)|
      (int_req & INT_ION?2:0)|
      (novaRunning?4:0)|
      (novaRunning?8:0)|
      (0)|
      (NovaC&CBIT?32:0));

    // push '9' for about ~1 second to stop the Nova
    int k=readKeys();
    if (k=='9') 
    { count9++;
      if (count9==250000/SIMINTERVAL) break; 
    }
    if (k) processkey(k);

    if (Serial.availableForWrite())
    { //if (DEV_IS_BUSY(INT_TTO))
      if (false)
      { int a=ttobuf&0x7f;
        if (a==10 || a==13 || a>=' ') 
          Serial.write(a);
        else
        { //Serial.print("?");
          //Serial.println(a);
        }
        DEV_CLR_BUSY(INT_TTO);
        DEV_SET_DONE(INT_TTO);
        DEV_UPDATE_INTR ;
        continue;
      }
    }

    // write to TTI and respond to ctrl-c
    if ((count=Serial.available()))
    { processSerial(count);
      //if (SerialIO) return;
    }
  }

  novaRunning=false;
  SerialIO=false;
  writeLights(0);
  return;
#endif

  writeDataReg(a);
  writeInstReg(0xfb);
  WriteReg(7, 4);
  wait();
  WriteReg(7, 0);
  wait();
  writeDataReg(CurrentSwitchValue);
  novaRunning=true;
}              

// continue
// note: examine and deposit affect continue address
void continueNova(void)
{ 
#ifdef SIMULATED
  startNova(NovaPC);
  return;
#endif
  writeInstReg(0xfb);
  WriteReg(7, 6);
  WriteReg(7, 0);
  wait();
  novaRunning=true;
}              

// continue but pass value of sw to switch register (disabling hardware switches)
// note: examine and deposit affect continue address, restore it by a=readAddr() .... examine(a); continueNovaSw(int sw)
void continueNovaSw(int sw)
{ CurrentSwitchValue=sw;
#ifdef SIMULATED
  SR = sw;
  startNova(NovaPC);
  return;
#endif
  writeDataReg(CurrentSwitchValue);
  wait();
  writeInstReg(0xfb);
  WriteReg(7, 6);
  WriteReg(7, 0);
  wait();
  novaRunning=true;
}              

void stepNova(void)
{ 
#ifdef SIMULATED
  novaRunning = true;
  writeLights(
      (novaRunning?1:0)|
      (int_req & INT_ION?2:0)|
      (novaRunning?4:0)|
      (novaRunning?8:0)|
      (0)|
      (NovaC&CBIT?32:0));
  prevpc=NovaPC;
  saved_PC=NovaPC;
  sim_instr(1);
  NovaPC=saved_PC;
  novaRunning = false;
  writeAddr(saved_PC);
  writeData(examine(saved_PC));
  writeLights(
      (novaRunning?1:0)|
      (int_req & INT_ION?2:0)|
      (novaRunning?4:0)|
      (novaRunning?8:0)|
      (0)|
      (NovaC&CBIT?32:0));
  return;
#endif

  prevpc=readAddr();
  writeInstReg(0xff);
  WriteReg(6, 1);
  WriteReg(7, 6);
  wait();
  Serial.println(F("instruction step"));
  WriteReg(7, 0);
  WriteReg(6, 0);
}

void memstepNova(void)
{ 
#ifdef SIMULATED
  stepNova();
  return;
#endif
  
  writeInstReg(0xff);
  WriteReg(7, 7);
  wait();
  WriteReg(7, 0);
}

void resetNova(void)
{ stopNova();
  WriteReg(6, 4);
  WriteReg(7, 6);
  delay(100);
  Serial.println("reset");
  WriteReg(7, 0);
  WriteReg(6, 0);
  lcdclearline(1);
  lcdsetCursor(0,1);
  lcdprint("reset"); 
  novaRunning=false;
  examine(0);
}

// assume 4x6 font, define width (128/4) and height (32/6)
#define defaultFont u8g2_font_4x6_mf //u8g2_font_tinytim_tf
#define U8LOG_WIDTH 32
#define U8LOG_HEIGHT 5
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];

// drive 4051 multiplexer for 5 I2C displays 0=left, 4=right
void selectDisplay(byte disp) {
#ifdef TEENSY35
  return;
#endif
  static bool configured = false;
  if (!configured) {
    pinMode(9, OUTPUT);  // ENOLED0
    pinMode(10, OUTPUT); // ENOLED1
    pinMode(15, OUTPUT); // ENOLED2
    configured = true;
  }
    
  digitalWrite(9, disp&1);
  digitalWrite(10, (disp>>1)&1);
  digitalWrite(15, (disp>>2)&1);
}

bool DrawCursor=false;

void lcdprint(String text)
{ selectDisplay(0);
  u8g2.setFont(defaultFont);
  u8g2.print(text);
  u8g2.sendBuffer();
}

void lcdwrite(uint8_t c)
{ selectDisplay(0);
  u8g2.setFont(defaultFont);
  u8g2.write(c);  
  u8g2.sendBuffer();
}

int oldx=0, oldy=0;
void lcdsetCursor(int x, int y)
{ selectDisplay(0);
  u8g2.setCursor(x*4,y*7+5);
  if (DrawCursor)
  { u8g2.setDrawColor(0);
    u8g2.drawLine(oldx*4,oldy*7+6,oldx*4+3,oldy*7+6);
    u8g2.setDrawColor(1);
    u8g2.drawLine(x*4,y*7+6,x*4+3,y*7+6);
    u8g2.sendBuffer();
    oldx=x; oldy=y;
  }
}

void lcdcursor(void)
{ DrawCursor=true;
}

void lcdnoCursor(void)
{ if (DrawCursor)
  { u8g2.setDrawColor(0);
    u8g2.drawLine(oldx*4,oldy*7+6,oldx*4+3,oldy*7+6);
    u8g2.setDrawColor(1);
    u8g2.sendBuffer();
  }
  DrawCursor=false;
}

void lcdclear(void)
{ selectDisplay(0);
  u8g2.clear();
  lcdsetCursor(0,0);
}

void lcdclearline(int y)
{ selectDisplay(0);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0,y*7,128,y*7+6);
  u8g2.setDrawColor(1);
}

// display text with large font on selected arbitrary display
void display(byte disp, String text)
{ selectDisplay(disp);
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.setCursor(8, 29);
  u8g2.print(text);
  u8g2.sendBuffer();
  u8g2.setFont(defaultFont);  
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
  else  // control part
  { if (sk==0) s += ("");
    if (sk==1) s += ("S");
    if (sk==2) s += ("C");
    if (sk==3) s += ("P");
  }
  return s;
}

#define HALT       063077

// System calls (these all are functional HALT instructions, interpreted by Arduino program)
#define INFO       077377 // show debug information on LCD
#define PUTC       077277 // output character to LCD and serial
#define GETC       077177 // get character from serial (/ mode) or keypad (9-5 mode)
#define READBLOCK  077077 // read 64 word block A0 from device (EEPROM) to address A2
#define WRITEBLOCK 073377 // write 64 word block A0 to device (EEPROM) from address A2
#define WRITELED   073277 // A0 bit 0 sets Arduino LED
#define MESSAGE    073177 // Write text string with %0..%3 substituted for AC0..3
#define GPIO       073077 // High bits define data direction (O=out), low bits value
//#define SKIPBUSYZ  (MOV(3,3)+NOLOAD+SKP) // Always skip
#define SKIPDONE   067377 // skip when character available
#define DELAY      067277 // Delay A0 ms, showing A1 on lights
#define ADC        067177 // Read A0 from teensy
// 6 are open 06[3,7][1-3]77 except 063077; maybe add 
// HWMUL, HWDIV, DEVICESEL, SKIPBUSYZ (always skip)

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
//  else if (v==SKIPBUSYZ)        s+=(".SKIPBUSYZ");
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
    s+=String(mref&3);       // destination
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

String saveHelp="xx";
void printHelp(String a)
{ if (a==saveHelp) return;
  saveHelp=a;
  lcdclearline(3);
  lcdsetCursor(0, 3);
  if (a!="") lcdprint(a.substring(0, 32));
  lcdclearline(4);
  lcdsetCursor(0, 4);
  if (a!="") lcdprint(a.substring(32));
}

////////////////////////////////////////////////////
// mini debugger display on lcd in single step mode
void lcdPrintDebug(void) {
  lcdclear(); // clear screen
#ifdef SIMULATED
  unsigned int pc = NovaPC;
  unsigned int carry = NovaC!=0;
  unsigned int in = NovaMem[NovaPC], a0, a1, a2=0, a3=0;
#else
  unsigned int pc = readAddr()&0x7fff;
  unsigned int carry = readLights()&0x20;
  unsigned int in = readData(), a0, a1, a2=0, a3=0;
#endif
  lcdsetCursor(0,0);    // print 4 accumulators
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
#ifdef SIMULATED
  unsigned int pc = NovaPC;
  unsigned int carry = NovaC!=0;
  unsigned int in = NovaMem[NovaPC], a0, a1, a2, a3;
#else
  unsigned int pc = readAddr()&0x7fff;
  unsigned int carry = readLights()&0x20;
  unsigned int in = readData(), a0, a1, a2, a3;
#endif
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

// TODO: detect
unsigned short memsize=16384;

void tests(int func)
{ while (readKeys());

  // register test: disconnect boards from nova and connect data req or inst req to gnd
  if (func==0) // 1st switch up
  { for(int count=0; count<65536; count++)
    { lcdsetCursor(0,1);
      lcdPrintOctal(count);
      lcdprint("  reg test ");
      lcdPrintOctal(readLights());
      writeDataReg(32768>>(count&15));
      writeInstReg(1<<(count&15));
      WriteReg(6, count&15);
      WriteReg(7, count/16);

      // test that read data gives back data (only if hand enabled)
      digitalWrite(13, readData()!=((unsigned)32768>>(count&15)));
      delay(100);
      //debugPrint(a0, a1, 11, 11, readAddr(), readData()); 
      if (readKeys() != 0) break;
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
      if (readKeys()) break;
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
      if (readKeys()) break;
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
      if (readKeys()) break;
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
      if (readKeys()) break;
    }
  }
  else if (func==6) // Continue passing virtual switches
  { continueNovaSw(0x6666);
    lcdsetCursor(0,1);
    lcdprint("sent 6666 to READS");
    delay(200);
  }
  else if (func==17) // Copy prog1..4 to NOVA
  { stopNova();
    //assemble(0, prog1);
    //assemble(050, prog2);
    //assemble(0100, prog3);
    //assemble(0140, prog4);
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
      if (readKeys()) break;
    }
    lcdsetCursor(0,1);
    lcdprint("mtfull ready      ");
  }
  else if (func==7)
  { int i=0;
    while (1)
    { int sw = readSwitches();

      // also enabled for non SIMULATED mode
      writeData((0x8000>>(i&15)) ^ sw);
      writeAddr(1<<(i&15));
      writeLights(1<<(i&7));
      writeDataReg(i<<8);
      writeInstReg(i);
      writeColor(i%11);
      i++;
      display(i%5, String(i));
      //display(1, String(i+123));
      //display(2, String(i+456));
      //display(3, String(i+789));
      //display(4, String(1000-i));
      delay (50);
      if (readKeys()) break;
    }
  }
}

// read 128 byte block from EEPROM to Nova memory A
void readBlockfromEEPROM(int block, unsigned int A)
{ writeColor(2);
  if (block<4)
  { unsigned int address=block*128;
    for (int i=0; i<64; i++)
    { deposit(A+i, EEPROM.read(address+i*2)<<8|EEPROM.read(address+i*2+1));
    }
  }
  else
  { int deviceaddress = 0x50;
    if (block>515) deviceaddress+=4;
    unsigned int address=(block-4)*128; // overflow is OK needs 16 bit
    unsigned char buffer[16];
    for(int j=0; j<8; j++)
    { i2c_eeprom_read_buffer(deviceaddress, address+j*16, buffer, 16);
      for (int i=0; i<8; i++)
      { deposit(A+i+j*8, (buffer[i*2]<<8)|buffer[i*2+1]);
      }
    }
  }
  writeColor(0);
}

// write 128 byte block from EEPROM to Nova memory A
void writeBlocktoEEPROM(int block, unsigned int A)
{ writeColor(1);
  if (block<4)
  { int address=block*128;
    for (int i=0; i<64; i++)
    { unsigned short s=examine(A+i);
      EEPROM.write(address+i*2, s>>8);
      EEPROM.write(address+i*2+1, s&255);
    }
  }
  else
  { int deviceaddress = 0x50;
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
  writeColor(0);
}

String store()
{ unsigned short a = 0;
  unsigned short b = 1028-memsize/64;
  unsigned short c = memsize/64;
  unsigned short s=a;
  for (int block=b; block<b+c; block++,a+=64)
    writeBlocktoEEPROM(block, a);
  Serial.println("saved memory "+toOct(s)+ " total of " + c + " blocks");
#ifdef SIMULATED
  examine(NovaPC);
#endif
  return "";
}

String restore()
{ unsigned short a = 0;
  unsigned short b = 1028-memsize/64;
  unsigned short c = memsize/64;
  unsigned short s=a;
  for (int block=b; block<b+c; block++,a+=64)
    readBlockfromEEPROM(block, a);
  Serial.println("loaded memory "+toOct(s)+ " with " + c + " blocks");
#ifdef SIMULATED
  examine(NovaPC);
#endif
  return "run "+toOct(a);
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

// Addresses for memory mapped I/O
#define CHARIN 040   // input to nova
#define CHAROUT 042  // output from nova

void writeDataSim(int D) {
#ifdef SIMULATED
  writeData(D);
#endif
}
 
void writeAddrSim(int A) {
#ifdef SIMULATED
  writeAddr(A);
#endif
}

void processswitches(short kbkey)
{ if (kbkey=='A') depositAC(0, readSwitches());
  if (kbkey=='C') depositAC(1, readSwitches());
  if (kbkey=='E') depositAC(2, readSwitches());
  if (kbkey=='G') depositAC(3, readSwitches());
  if (kbkey=='B') writeDataSim(examineAC(0));
  if (kbkey=='D') writeDataSim(examineAC(1));
  if (kbkey=='F') writeDataSim(examineAC(2));
  if (kbkey=='H') writeDataSim(examineAC(3));
  if (kbkey=='I') resetNova();
  if (kbkey=='J') writeAddrSim(stopNova());
  if (kbkey=='K') {SerialIO=true; startNova(readSwitches()); }
  if (kbkey=='L') {SerialIO=true; continueNova(); }
  if (kbkey=='M') {deposit(NovaPC,readSwitches()); writeAddrSim(NovaPC); writeDataSim(readSwitches());}
  if (kbkey=='N') {deposit(++NovaPC,readSwitches()); writeAddrSim(NovaPC); writeDataSim(readSwitches());}
  if (kbkey=='O') {writeDataSim(examine(NovaPC=readSwitches())); writeAddrSim(NovaPC);}
  if (kbkey=='P') {writeDataSim(examine(++NovaPC)); writeAddrSim(NovaPC);}
  if (kbkey=='Q') memstepNova();
  if (kbkey=='R') stepNova();
  if (kbkey=='S') store();
  if (kbkey=='T') restore();
}

void processkey(short kbkey)
{ /////////////////////////// keypad pressed /////////////////////////

  if (kbkey=='*') kbkey=10;
  else if (kbkey=='#') kbkey=12;
  else if (kbkey=='0') kbkey=11;
  else if (kbkey<='9') kbkey-='0';
  else {
    processswitches(kbkey);
    return;
  }

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
    lcdcursor();
    lcdsetCursor(7+noctal,0);
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
    lcdcursor();
    lcdsetCursor(7+noctal,0);
  }

  else if (kbkey==8 && kbmode==2 && opmode<=2)  // arrow back data mode
  { if (noctal==0) noctal=opmode?6:5; else noctal--;
    lcdcursor();
    lcdsetCursor(7+noctal,0);
    if (opmode==2) printAssemblyHelp(octalval, noctal);
  }
  else if (kbkey==9 && kbmode==2 && opmode<=2)  // arrow forward data mode
  { noctal++; if (noctal>(opmode?6:5)) noctal=0;
    lcdcursor();
    lcdsetCursor(7+noctal,0);
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
    lcdcursor();
    lcdsetCursor(7+noctal,0);
  }
  else if (kbkey==12 && kbmode<2 && opmode==1)  // # = start decimal edit mode
  { kbmode = 2;
    noctal=0;
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
    lcdsetCursor(6+noctal,0);
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
    lcdcursor();
    lcdsetCursor(7+noctal,0);
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
    lcdprint("ex de as db su io ts sv ld rn");
    printHelp("Select run mode");
    while(readKeys()!=0);
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
#include "novabasic2.tape.h"

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
String("tape filename         Load tape data from eeprom or code memory\r\n")+
String("tape list             List all tapes in eeprom and code memory\r\n")+
String("test n                run test function\r\n")+
String("version               print version\r\n")+
String("lcd line text         display line 0..4 on LCD\r\n")+
String("led N                 program GPIO and Arduino LED (400)\r\n")+
String("adc N                 read 2,3=A2,A3\r\n")+
String("dac N                 write A14\r\n")+
String("reads                 read switches\r\n")+
String("readk                 read key\r\n")+
String("writed N              write data (without NOVA)\r\n")+
String("writea N              write address (without NOVA)\r\n")+
String("writel N              write lights (without NOVA)\r\n")+
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

String line;
String nextcmd;
#define NHISTORY 40
String history[NHISTORY];
int nhist=0;
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

#ifdef SIMULATED
      //if (!DEV_IS_DONE(INT_TTI))
      { ttibuf= b;
        DEV_CLR_BUSY( INT_TTI ) ;
        DEV_SET_DONE( INT_TTI ) ;
        DEV_UPDATE_INTR ;
        return;
      }
#else
      Serial1.write(b);
#endif      

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
#ifdef SIMULATED
      pc=NovaPC&0x7fff;
#else
      pc=readAddr()&0x7fff;
#endif

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
#ifdef SIMULATED
        NovaPC = readOct(line, pos2+1);
        Serial.println("pc = "+toOct(NovaPC&0x7fff));
#else
        Serial.println("pc = "+toOct(readAddr()&0x7fff));
#endif
        nextcmd = "step";
      }

      // show all registers
      else if (line.startsWith("reg"))
      { for (int a=0; a<4; a++)
          Serial.println("ac"+String(a)+" = "+toOct(examineAC(a)));
#ifdef SIMULATED
        Serial.println("pc = "+toOct(NovaPC&0x7fff));
        Serial.println("carry = "+String(NovaC!=0));
#else
        Serial.println("pc = "+toOct(readAddr()&0x7fff));
        Serial.println("carry = "+String(readLights()>>5));
#endif
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
        Serial.println("running");
        startNova(a);
        nextcmd = "stop";
      }

      // run with serial IO
      else if (line.startsWith("run "))
      { int pos1 = 4;
        SerialIO=true;
        unsigned short a = readOct(line, pos1);
        Serial.println("running, use ctrl-c to return to command mode");
        startNova(a);
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
        Serial.println("Nova1210 front panel code Marcel van Herk " __DATE__);
#ifdef SIMULATED
        Serial.println("Simulated mode: Contains parts of simhv3-9");
#endif
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

      // restore memory from eeprom
      else if (line.startsWith("restore"))
        nextcmd = restore();

      // store memory to eeprom
      else if (line.startsWith("store"))
        nextcmd = store();

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
        nextcmd = "lights";
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
          display(a, line.substring(pos2+1));
          nextcmd = "lcd "+String((a+1)%5)+" "+line.substring(pos2+1);
        }
      }

      // program leds
      else if (line.startsWith("led "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
        //gpio(a & 255);
        digitalWrite(13, (a>>8)&1);
        //digitalWrite(12, (a>>9)&1);
        //digitalWrite(11, (a>>10)&1);
        writeColor(a&15);
        nextcmd = "led "+toOct(a+1);
      }
          
      // program DAC
      else if (line.startsWith("dac "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
#ifdef A14
        analogWrite(A14, a);
#endif
        nextcmd = "dac "+toOct(a+1);
      }

      // read switches bank
      else if (line.startsWith("reads"))
      { Serial.println("Switches="+toOct(readSwitches())); 
        nextcmd = "reads";
      }

      // read key
      else if (line.startsWith("readk"))
      { Serial.println("Key="+toOct(readKeys())); 
        nextcmd = "readk";
      }

      // write data bus (for unconnected nova, also enabled if non SIMULATED)
      else if (line.startsWith("writed "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        writeData(a);
        nextcmd = "writed "+toOct(a+1);
      }

      // write address bus (for unconnected nova, also enabled if non SIMULATED)
      else if (line.startsWith("writea "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        writeAddr(a);
        nextcmd = "writea "+toOct(a+1);
      }

      // write lights (for unconnected nova, also enabled if non SIMULATED)
      else if (line.startsWith("writel "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        writeLights(a);
        nextcmd = "writel "+toOct(a+1);
      }

      // read ADC
      else if (line.startsWith("adc"))
      { 
        int pos1 = 4;
        int pins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
        int pin=readOct(line, pos1);
        pinMode(pins[pin&7], INPUT);
        Serial.println("adc"+String(pin)+"="+toOct(analogRead(pins[pin&7])));
        nextcmd = "adc";
      }

      // list eeprom block in hex
      else if (line.startsWith("eeprom "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        int deviceaddress = 0x50;                  
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
        int deviceaddress = 0x50;
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
        Serial1.write(a);
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
        if (filename==".BASIC2")
        { nextcmd=tapeloader(basic2tape, sizeof(basic2tape), ".basic2");
          Serial.print(">");
          line = "";
          return;
        }

        if (filename=="LIST")
        { Serial.println(".BASIC in code memory; length "+toOct(sizeof(basictape)));
        }

        byte fname[16], ename[16];
        filename.getBytes(fname, 16);
        int deviceaddress = 0x50;                  
          
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
}

void setup() {
  delay(10);

  pinMode(13, OUTPUT); // LED doubles with SSCLK for SPI

  digitalWrite(13, HIGH);

  // reset all I2C buffer chips
  WireBegin();
  for (int i=0; i<0x16; i++)
  { writeByte(0x20, i, 0x00);
    writeByte(0x21, i, 0x00);
    writeByte(0x22, i, 0x00);
    writeByte(0x23, i, 0x00);
    writeByte(0x24, i, 0x00);
  }
  WireEnd();

  digitalWrite(13, LOW);

  // program all lights to be input
  readAddr();
  readData();
  readLights();

  digitalWrite(13, LOW);

  // you MUST initialize all displays
  for (int i=0; i<5; i++)
  { selectDisplay(i);
    u8g2.begin();
  }

  digitalWrite(13, HIGH);

  display(1,"DATA GENERAL");
  display(2, "CORPORATION");
#ifdef SIMULATED
  display(4, "NOVA 1210 S");
#else
  display(4, "NOVA 1210");
#endif

  digitalWrite(13, LOW);

  writeControlReg(0); // all NOVA control signals inactive

  // setup terminal window emulator
  selectDisplay(3);
  u8g2.setFont(defaultFont);  
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer); // connect to u8g2, assign buffer
  u8g2log.setLineHeightOffset(0); // set extra space between lines in pixel, this can be negative
  u8g2log.setRedrawMode(0);   // 0: Update screen with newline, 1: Update screen for every char  

  digitalWrite(13, HIGH);

  selectDisplay(3);
  u8g2log.print("Nova 1210 front panel");
  u8g2log.print("\n");
  u8g2log.print("By Marcel van Herk " __DATE__ "\n");
  u8g2log.print("Memory size ");
  u8g2log.print(memsize);
  u8g2log.print(" words");
  u8g2log.print("\n");
#ifdef SIMULATED
  u8g2log.print("Contains parts of simhv39-0\n");
#endif

// keep last screen on Teensy3.5 (no front panel)
#ifndef TEENSY35
  selectDisplay(0);
  u8g2.clear();
#endif

  digitalWrite(13, LOW);

  writeDataReg(0);
  writeInstReg(0); // TODO: correct start status

  // TODO: memory size test

  Serial.begin(115200);     // usb serial for debug output
  Serial.setTimeout(5000);

  Serial1.setTX(1); // serial to nova
  Serial1.setRX(0);
  Serial1.begin(4800,SERIAL_8N1);
}

short i=0;
int prev=0;
int startKey=0;
void loop(void) {
  unsigned long currentMillis = millis();
  
  if (Serial1.available())
  { byte b = Serial1.read();
    Serial.write(b&0x7f);
    return;
  }

  // wait and detect halt condition; special bits in HALT are interpreted as 'OS' instructions
#ifdef SIMULATED
  while (false)
#else
  while (millis() - currentMillis < 100)
#endif
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
        //u8g2log.write(b);
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
            //u8g2log.write(b);
          }
          else
            flag=false;
          b = s&255;
          if (b==0) break;
          if (!flag)
          { if (b=='%') { Serial.print(String((short)examineAC((s2>>8)&3))); flag=true; }
            else Serial.write(b);
            //u8g2log.write(b);
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
        //digitalWrite(6, 1);  // poor attempt to keep LCD tidy as LED doubles as LCD2 select
        //digitalWrite(A2, 1);
        //digitalWrite(A3, 1);
        //digitalWrite(A4, 1);
        //digitalWrite(A5, 1);
#endif
        //digitalWrite(13, (haltA0&1)!=0);
#ifdef TEENSY
        //digitalWrite(12, (haltA0&2)!=0);
        //digitalWrite(11, (haltA0&4)!=0);
        writeColor(haltA0&15);
#ifdef A14
        analogWrite(A14, haltA0>>4);
#endif
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
        //gpio(1);
        readBlockfromEEPROM(haltA0, A2);
        depositAC(0, haltA0+1);
        depositAC(2, A2+64);
        //gpio(0);
        examine(haltAddress);
        continueNova();
        continue;
      }
      //else if (haltInstruction==SKIPBUSYZ) // skip when character available
      //{ if (true)
      //    examine(haltAddress+1);
      //  else
      //    examine(haltAddress);
      //  continueNova();
      //  continue;
     // }
      else if (haltInstruction==WRITEBLOCK) // read BIGENDIAN from eeprom
      { unsigned int A2=examineAC(2);
        //gpio(4);
        writeBlocktoEEPROM(haltA0, A2);
        //gpio(0);
        depositAC(0, haltA0+1);
        depositAC(2, A2+64);
        examine(haltAddress);
        continueNova();
        continue;
      }
      else if (haltInstruction==GPIO)     // GPIO through MCP20008
      { //gpio(haltA0);
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
  { processSerial(count);
    if (SerialIO) return;
  }

  int kbkey;
  if ((kbkey=readKeys()))
  { int k=millis();
    if (startKey==0) startKey=millis();
    processkey(kbkey);
    while (readKeys()) {
      if ((count=Serial.available()))
      { processSerial(count);
        if (SerialIO) return;
      }
      if (millis()-k>100 && millis()-startKey>1000) return;
    }
  }
  else
    startKey=0;
}
