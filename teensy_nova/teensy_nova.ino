/***************************************************
Teensy Nova 1210 visual simulator program
Expects ILI9341 display connected to Teensy3.0/4.1

Marcel van Herk, 22 July 2020
Marcel van Herk, 4 August 2020 - refactored
Marcel van Herk, 6 August 2020 - added scrolling display; configurable button table
Help text generated from table; added MIPS display; button1=help
Marcel van Herk, 7 August 2020 - long press does not trigger short press, put help on FUNC, cls on ON/OFF
Marcel van Herk, 9 August 2020 - Fine tuned keys, hints and help
Marcel van Herk, 10 August 2020 - Session difference can be stored in eeprom
Marcel van Herk, 12 August 2020 - Added injectSerial to allow input from buttons
                                  Added basic menu and keyboard
Marcel van Herk, 13 August 2020 - Basic menu when running otherwise monitor; extended keyboard
Marcel van Herk, 14 August 2020 - Split menu and keyboard functions; more comments
                                - 32 kW memory; help layout changes
                                - Split basic session into raw session and example code
                                - Updated front panel image from FrontPanel 2.0 Copyright (c) 2007-2008, John Kichury
                                - Added cursor and color control on tft using \F \G \Ccol \Lrow \Bbgcolor and \Pcolor
                                - Note that the values e.g. line in \L can be surrounded by spaces
Marcel van Herk, 15 August 2020 - Added buffering of serial input into injectSerialText
                                - This make copy and paste into terminal work with Nova at high speed
                                - Added session .pong
                                - Added simple graphics terminal; use in BASIC:

PRINT "\G";ROW;COL;";" - set text cursor
PRINT "\C";R;G;B";"    - set text color (RGB range 0..255)
PRINT "\B";R;G;B";"    - set text background color
PRINT "\f";            - clear screen
PRINT "\F";            - clear screen - force update (to also clear graphics)

PRINT "\M";X;Y";"       - move graphics cursor
PRINT "\L";X;Y";"       - draw line (in text color)
PRINT "\R";W;H";"       - draw filled rect (in text color)
PRINT "\r";W;H";"       - draw open rect (in text color)
PRINT "\E";R;";"        - draw filled circle (in text color)
PRINT "\e";R;";"        - draw open circle (in text color)
PRINT "\P";             - draw point
PRINT "\T size [char];";- draw graphics character(s) (pass up to 8 numerical values);

Marcel van Herk, 16 August 2020 - Added proper session hierarchy and error handling
                                - Added about information with reference to sources
                                - clearText also resets text colors and control codes
                                - Added /T option to draw character sprites
Marcel van Herk, 17 August 2020 - auto-->autostart; injects escape if session root is .BASICSESSION
                                - Display special character names in keyboard; added prv key to reload previous 
                                - Extended about; show about and then help on any button if power of.
                                - Made power-of image darker; use bit more color in help and about
Marcel van Herk, 29 August 2020 - Reset Nova lights on power ON; 
                                - support TEENSY35 on small BB with tft overlapping pins 33-39, 32=led
Marcel van Herk, 31 August 2020 - Renamed includes to .h to allow relative paths
                                - Support Teensy3.0..4.1
                                - Added optional PS2 kb on 4 pins (3v3,data,clock,gnd)
Marcel van Herk, 4 September 2020 - Added FEATHERWING_TFT_TOUCH mode
Marcel van Herk, 5 September 2020 - Adjust help for touch mode; disable debounce for touch
                                  - Added SD card to use as 'eeprom' if found
Marcel van Herk, 6 September 2020 - FEATHERWING Teensy4 kb changed pins and added change note
                                  - Added PS2_F1=on/off F2=restore F3=store F4=speed
                                  - Added A1(15) switch; note overlaps bb version lights
Marcel van Herk, 7 September 2020 - Added ps2kb power control and backlight control for FEATHERWING_TFT_TOUCH
                                  - Added F5=about F6=cls F7=stop F8=step F9=continue F10=menu F12=help
								  - Save power by touch checks per 200ms when nova is off
Marcel van Herk, 8 September 2020 - Added ONOFFSLIDER, LAZY_BREADBOARD
Marcel van Herk, 9 September 2020 - Moved ONOFFSLIDER jumper wire, fixed button swap, moved text cursor one pixel left avoids line wrap
                                    Note: block out these lines (2x) in Adafruit_SPITFT.cpp makes LAZY_BREADBOARD TFT 3 times faster!
                                      #if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
                                      for (volatile uint8_t i = 0; i < 1; i++)
                                      ;
Marcel van Herk, 10 September 2020 - Centralized most pin defines; added ps2 keyboard to LAZY_BREADBOARD
Marcel van Herk, 16 September 2020 - Added WIFI through WifiModem on Serial4 or Serial8, set 115200 baud and "do not Show"
Marcel van Herk, 26 October 2020   - Split LAZY_BREADBOARD into LAZY_BREADBOARDBTN (vertical vs horizontal buttons) 
                                     and LAZY_BREADBOARDTFT (TFT overlaps top right pins of large teensy); HASNOSD flag
                                     Disabled ONOFFSLIDER for non FEATHERWING_TFT_TOUCH
Marcel van Herk, 14 March 2021     - Also run monitor from WIFI
Marcel van Herk, 25 March 2021     - Added HALT-instruction based function calls I use on real NOVA
Marcel van Herk, 2 April 2021      - Avoid redraw of unchanged lights and buttons
Marcel van Herk, 4 April 2021      - Refactored configuration; added PICO mode; works best with updated ILI9431 & SPI libraries
                                   - PICO assumes/shows 4 buttons, although inputs for other 4 are provided
                                   - Tested with Teensy4.0, Teensy4.1, RPPico, Featherwing, Teensy3.5
Marcel van Herk, 5 April 2021      - Reenabled optional LEDs on pico, happen to be on same pins!
Marcel van Herk, 6 April 2021      - Added PS2_VCC2, use Bodmer TFT initialisation, fix led 400, reduced SPI clock for reliability
Marcel van Herk, 7 April 2021      - Added some missing EEPROM.commits, fix led command on WIFI
Marcel van Herk, 23 January 2022   - Added support for Pico Display Pack 2.0 (define PICOD2, not PICO)

****************************************************/
// on older IDE's the Teensy type define must be made manually
//#define ARDUINO_TEENSY35

// select breadboard type or Featherwing TFT board (define none or one)
//#define FEATHERWING_TFT_TOUCH
//#define LAZY_BREADBOARDTFT
//#define LAZY_BREADBOARDBTN
#define PICOD2

#include "SPI.h"
#if defined(PICOD2)
#include "Adafruit_ST7789.h"
#define ILI9341_BLACK  ST77XX_BLACK
#define ILI9341_RED    ST77XX_RED
#define ILI9341_YELLOW ST77XX_YELLOW
#define ILI9341_WHITE  ST77XX_WHITE
#else
#if defined(LAZY_BREADBOARDTFT) || defined(ARDUINO_LOLIN32) || defined(PICO)
#include "Adafruit_ILI9341.h"
#else
#include "ILI9341_t3.h"
#endif
#endif

#include "EEPROM.h"
#include "PS2Keyboard.h"
#ifdef FEATHERWING_TFT_TOUCH
#include <Adafruit_STMPE610.h>
#endif

#ifdef PICOD2
#  define TFT_MISO -1
#  define TFT_SCK 18
#  define TFT_MOSI 19
#  define TFT_DC 16
#  define TFT_CS 17
#  define TFT_LED 20
#  define A4 0
#  define A5 0
#  define A6 0
#  define A7 0
#  define BUTTON1 12
#  define BUTTON2 2 // unused
#  define BUTTON3 22 // unused
#  define BUTTON4 10 // unused
#  define BUTTON5 11 // unused
#  define BUTTON6 13
#  define BUTTON7 14
#  define BUTTON8 15
#  define LED_RED   6
#  define LED_GREEN 7
#  define LED_BLUE  8
#endif

#ifdef PICO
#  define TFT_MISO 16
#  define TFT_SCK 18
#  define TFT_MOSI 19
#  define TFT_DC 20
#  define TFT_CS 17
#  define A4 0
#  define A5 0
#  define A6 0
#  define A7 0
#  ifndef BUTTON1
#    define BUTTON1 26
#    define BUTTON2 8 // unused
#    define BUTTON3 7 // unused
#    define BUTTON4 5 // unused
#    define BUTTON5 4 // unused
#    define BUTTONGND4 9
#    define BUTTON6 6
#    define BUTTON7 3
#    define BUTTONGND5 2
#    define BUTTON8 0
#  endif
#  define PS2_CLK 10
#  define PS2_DATA 11
#  define PS2_VCC 12
#  define PS2_VCC2 13
#endif

#ifdef LAZY_BREADBOARDTFT
#  define TFT_MISO 24
#  define TFT_LED 25
#  define TFT_SCK 26
#  define TFT_MOSI 27
#  define TFT_DC 28
#  define TFT_RESET 29
#  define TFT_CS 30
#  define TFT_GND 31
#  define TFT_VCC 32
#  define SD_CS BUILTIN_SDCARD
#  define ONOFFSLIDER 5
#  define PS2_GND 39
#  define PS2_CLK 38
#  define PS2_DATA 37
#ifdef ARDUINO_TEENSY41
#  define WIFI Serial8
#endif
#  define PS2_VCC 36
#  define PS2_VCC2 35
#endif

#ifdef LAZY_BREADBOARDBTN
#  ifndef BUTTON1
#    define BUTTON1 17
#    define BUTTON2 19
#    define BUTTON3 21
#    define BUTTON4 23
#    define BUTTON5 8
#    define BUTTON6 6
#    define BUTTON7 4
#    define BUTTON8 2
#  endif
#endif

#ifdef FEATHERWING_TFT_TOUCH
#  define TOUCH_CS 3
#  define TFT_CS 4
#  define TFT_DC 10
#  define TFT_LED 9
#  define SD_CS 8
#  define PS2_CLK 1
#  define PS2_DATA 0
#  define PS2_VCC 14
#  define PWRBUTTON 15
#  define ONOFFSLIDER 5
#  define WIFI Serial4
   // free pins a2=16, a3=17, a6=20, a14=on/off
#endif

#if defined(ARDUINO_LOLIN32)
#  define TFT_MISO 19
#  define TFT_LED 0
#  define TFT_SCK 18
#  define TFT_MOSI 23
#  define TFT_DC 22
#  define TFT_RESET 0
#  define TFT_CS 5
#  define TFT_GND 0
#  define TFT_VCC 0
#  define A1 0
#  define A2 0
#  define A3 0
#  define A4 0
#  define A5 0
#  define A6 0
#  define A7 0
#  ifndef BUTTON1
#    define BUTTON1 33
#    define BUTTON2 25
#    define BUTTON3 27
#    define BUTTON4 12
#    define BUTTON5 17
#    define BUTTON6 4
#    define BUTTON7 2
#    define BUTTON8 13
#  endif
#endif

// default Teensy layout
#ifndef TFT_CS
#  define TFT_CS 10
#  define TFT_DC 9
#  define ONOFFSLIDER 2
#  if defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41)
#    define SD_CS BUILTIN_SDCARD
#    define TFT_LED 32
#    define PS2_GND 26
#    define PS2_CLK 25
#    define PS2_DATA 24
#  endif
#endif
#ifndef BUTTON1
#  define BUTTONGND1 3
#  define BUTTONGND2 4
#  define BUTTONGND3 5
#  define BUTTONGND4 19
#  define BUTTONGND5 20
#  define BUTTON1 16
#  define BUTTON2 18
#  define BUTTON3 23
#  define BUTTON4 22
#  define BUTTON5 8
#  define BUTTON6 7
#  define BUTTON7 1
#  define BUTTON8 0
#endif

// set up variables using the SD utility library functions:
#ifdef SD_CS
# include <SD.h>
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  File myFile;
#endif

#ifdef TOUCH_CS
Adafruit_STMPE610 touch=Adafruit_STMPE610(TOUCH_CS);
#endif

#ifndef CL // depends on inclusion of ILI9341_t3.h
#define CL(a, b, c) (((a>>3)<<11)+((b>>2)<<5)+(c>>3))
#define writeRect(a, b, c, d, e) drawRGBBitmap(a, b, e, c, d) 
#define USEADAFRUIT
#endif

#if defined(PICOD2)
  Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, -1);
#elif defined(PICO)
  SPIClassRP2040 spi00 = SPIClassRP2040(spi0, TFT_MOSI, TFT_CS, TFT_SCK, TFT_MISO);
  Adafruit_ILI9341 tft = Adafruit_ILI9341(&spi00, TFT_DC);
  //Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, -1, TFT_MISO); // test SW SPI
#elif defined(USEADAFRUIT)
  Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO);
  //Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC); //, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO); // HW SPI, requires extra wires
#elif defined(FEATHERWING_TFT_TOUCH)
  ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
#else
  ILI9341_t3 tft = ILI9341_t3(10, 9);
#endif

bool hasSD = false;
bool hasWIFI = false;

PS2Keyboard ps2kb;

#define MEMSIZE 32768

// button table
// LP*longpress + shortpress + autorepeat
#define LP 256
#define AR 65536
int numButtons=8;

#if defined(PICO) || defined(PICOD2) 
static int keyBank=0, defaultKeyBank=0;
int keys[] =                                                                          // no bank switching, no help
{ 25*LP+49, 25,  9*LP+10, 11*LP+12,    AR+18, 20*LP+19, 21*LP+24, 23*LP+24      // processor control (some auto repeat)
  //cls+pwr                                    str+PL    esc+menu  speed+menu
};
String items[]=
{ " Processor control and menu"
};
#elif true
// 3 banks AC and memory buttons push long for alternate function
static int keyBank=2, defaultKeyBank=2;
int keys[] =                                                                            // 49=power, 48=bank, 25=clear screen
{ 25*LP+49, 26*LP+48, 41*LP+31, 42*LP+32, 43*LP+33, 44*LP+34, 45*LP+35, 46*LP+36,       // 31=36 = data; 41-46 = data clear
  25*LP+49, 26*LP+48,  1*LP+ 2,  3*LP+ 4,  5*LP+ 6,  7*LP+ 8, AR+13*LP+14, AR+16*LP+15, // 1-8 = ac keys 13-16 = deposit/examine (long and auto repeat)
  25*LP+49, 26*LP+48,  9*LP+10, 11*LP+12,    AR+18, 20*LP+19, 21*LP+22, 23*LP+24      // processor control (some auto repeat)
};
String items[]=
{ "2 Change data switches",
  "3 Examine/Deposit regs & memory",
  "1 Processor control and keyboard"
};
#else
// older 4 bank method
static int keyBank=0, defaultKeyBank=0;
int keys[] =                                                                            // 49=power, 48=bank, 25=clear screen
{ 25*LP+49, 26*LP+48, 41*LP+31, 42*LP+32, 43*LP+33, 44*LP+34, 45*LP+35, 46*LP+36,       // 31=36 = data; 41-46 = data clear
  25*LP+49, 26*LP+48,  1*LP+ 2,  3*LP+ 4,  5*LP+ 6,  7*LP+ 8, AR+13*LP+14, AR+16*LP+15, // 1-8 = ac keys 13-16 = deposit/examine (long and auto repeat)
  25*LP+49, 26*LP+48,        9,       10,       11,       12,    AR+17,    AR+18,       // processor control (some auto repeat)
  25*LP+49, 26*LP+48,       19,       20,       21,       22,       23,       24        // program load
};
String items[]=
{ "3 Change data switches",
  "4 Examine/Deposit regs & memory",
  "1 Processor control",
  "2 Program load and keyboard"
};
#endif

// function names of Nova keys and extensions for help strings
String keyInfo[]={
        "",     //0
        "Deposit",
        "AC0",
        "Deposit",
        "AC1",
        "Deposit",
        "AC2",
        "Deposit",
        "AC3",
        "Reset",  //9
        "Stop",
        "Start",
        "Continue",
        "Deposit",
        "Dp Next",
        "Examine",
        "Ex Next",
        "Mem Step",
        "Inst Step",
        "Prog Load", //19
        "Store", //20
        "[Esc]", //21
        "Keyboard", //22
        "Speed", //23
        "Commands", //24
        "Cls", //25
        "Help", //26
        "", //27
        "", //28
        "", //29
        "", //30
        "100000", //31
        "010000", //32
        "001000", //33
        "000100", //34
        "000010", //35
        "000001", //36
        "", //37
        "", //38
        "", //39
        "", //40
        "Zero", //41
        "Zero", //42
        "Zero", //43
        "Zero", //44
        "Zero", //45
        "Zero", //46
        "", //47
        "FUNC", //48
        "ON-OFF"  //49
};
        
String keyHelp(int button)
{ String s="";
  int k=keys[button+keyBank*numButtons];
  if ((k&255)<50) s=s+keyInfo[k&255];
  if (k&0xff00)
  { if (((k>>8)&255)<50) s=s+" / "+keyInfo[(k>>8)&255];
  }
  return s;
}

bool isKeyActive(int key)
{ for (int i=0; i<numButtons; i++)
  { int k = keys[keyBank*numButtons+i];
    if ((k&255)==key) return true;
    if (((k>>8)&255)==key) return true;
  }
  return false;
}

// get key with debounce, autorepeat and long press as configured in key array
// raw keys used for menu only
int getButtonPress(bool raw)
{ bool genKey=false;
  static int stat=0;
  static int previousButtons=0;
  static int lastKey=0;

  static unsigned int lastChange=0;
  static unsigned int autoRepeat=0;
  
#ifdef FEATHERWING_TFT_TOUCH
  return getTouchPress(raw);
#endif

  int numBanks = sizeof(keys)/(numButtons*sizeof(keys[0]));
  unsigned int repeatTime=100;

  static bool conf=false;
  if (!conf)
  { 
#ifdef BUTTONGND1
    pinMode(BUTTONGND1, OUTPUT); // switch 1, 4 low
    digitalWrite(BUTTONGND1, LOW);
#endif
#ifdef BUTTONGND2
    pinMode(BUTTONGND2, OUTPUT); // switch 1, 4 low
    digitalWrite(BUTTONGND2, LOW);
#endif
#ifdef BUTTONGND3
    pinMode(BUTTONGND3, OUTPUT); // switch 1, 4 low
    digitalWrite(BUTTONGND3, LOW);
#endif
#ifdef BUTTONGND4
    pinMode(BUTTONGND4, OUTPUT); // switch 1, 4 low
    digitalWrite(BUTTONGND4, LOW);
#endif
#ifdef BUTTONGND5
    pinMode(BUTTONGND5, OUTPUT); // switch 1, 4 low
    digitalWrite(BUTTONGND5, LOW);
#endif
    
#ifdef BUTTON1
    pinMode(BUTTON1, INPUT_PULLUP); // btn 1: on-off
    pinMode(BUTTON2, INPUT_PULLUP); // btn 2: function (map btn 1-6 to nova keys)
    pinMode(BUTTON3, INPUT_PULLUP); // btn 3: 1
    pinMode(BUTTON4, INPUT_PULLUP); // btn 4: 2
    pinMode(BUTTON5, INPUT_PULLUP);  // btn 5: 3
    pinMode(BUTTON6, INPUT_PULLUP);  // btn 6: 4
    pinMode(BUTTON7, INPUT_PULLUP);  // btn 7: 5
    pinMode(BUTTON8, INPUT_PULLUP);  // btn 8: 6
#endif
    conf=true;
    lastChange = millis();
    delay(10);
    return 0;
  }

#ifdef BUTTON1
  int currentButtons =
    ((!digitalRead(BUTTON1))) |
    ((!digitalRead(BUTTON2))<<1) |
    ((!digitalRead(BUTTON3))<<2) |
    ((!digitalRead(BUTTON4))<<3) |
    ((!digitalRead(BUTTON5))<<4) |
    ((!digitalRead(BUTTON6))<<5) |
    ((!digitalRead(BUTTON7))<<6) |
    ((!digitalRead(BUTTON8))<<7);
#else
  int currentButtons = 0;
#endif

  // nothing happened
  if (stat==0 && currentButtons==previousButtons) 
    return 0;

  // button pressed or released; start debounce
  if (stat<2 && currentButtons!=previousButtons) 
  { lastChange=millis();
    previousButtons = currentButtons;
    stat=2; // debounce
    return 0;
  }

  // decode single key from bitflags
  int ckey=-1;
  int cb = currentButtons;
  while(cb) 
  { ckey++;
    cb = cb>>1;
  }

  // lookup key in table
  int kinfo = keys[numButtons*keyBank+max(ckey,0)];
  if (raw) kinfo=ckey+1+AR;

  int autorep = kinfo & AR;
  int longpress = (kinfo>>8) & 255;
  if (!autorep) repeatTime=20000;

  // waiting for end debounce
  if (stat==2 && currentButtons==previousButtons && (millis()-lastChange)>20) 
  { lastChange=millis();
    previousButtons=currentButtons;
    if (currentButtons==0)
    { stat=0; // no button pressed
      genKey=true;
      kinfo=lastKey;
    }
    else
    { stat=1; // button just pressed
      kinfo = kinfo & 255;
      autoRepeat=0x7fffffff;
      if (longpress)
      { lastKey=kinfo;
        return 0;
      }
      else
      { genKey=true;
        lastKey=0;
      }
    }      
  }

  // failed debounce
  if (stat==2 && currentButtons!=previousButtons && millis()-lastChange>20) 
  { lastChange=millis();
    previousButtons=currentButtons;
    if (currentButtons==0)
      stat=0;
    else
      stat=1;
    return 0;
  }

  // waiting for long press
  if (stat==1 && millis()-lastChange>500 && millis()-autoRepeat>repeatTime && (autorep||longpress)) 
  { genKey=true;
    lastKey=0;
    autoRepeat=millis();

    if (longpress) kinfo = longpress;
    else           kinfo = kinfo & 255;
  }

  // output key, special options for bank selection
  if (genKey && kinfo)
  { if (kinfo==48) 
    { keyBank++;
      if (keyBank>=numBanks) keyBank=0;
      kinfo = 80+keyBank;
    }
    if (kinfo==25) 
      keyBank=0;
    return kinfo;
  }

  return 0;
}

int novaKey = 1;

#ifdef FEATHERWING_TFT_TOUCH
// get touch with debounce, autorepeat and long press as configured in key array
// raw touch used for menu only
int getTouchPress(bool raw)
{ bool genKey=false;
  static int stat=0;
  static int previousButtons=0;
  static int lastKey=0;

  static unsigned int lastChange=0;
  static unsigned int autoRepeat=0;
  static unsigned int lastTouch=0;

  int numBanks = sizeof(keys)/(numButtons*sizeof(keys[0]));
  unsigned int repeatTime=100;
  
  static bool conf=false;
  if (!conf)
  { conf=true;
    lastChange = millis();
    delay(10);
    return 0;
  }

  uint16_t x=0, y=0;
  uint8_t z=0;
  static int currentButtons = 0;
  if (millis()-lastTouch>200 || novaKey==2)
  { lastTouch=millis();
    if (touch.touched())
    { unsigned short *data1 = (unsigned short *)malloc(320*2);
      unsigned short *data2 = (unsigned short *)malloc(320*2);
      unsigned short *data3 = (unsigned short *)malloc(320*2);
      unsigned short *data4 = (unsigned short *)malloc(320*2);
  
      tft.readRect(0, 120, 320, 1, data1);  
      tft.readRect(80, 0, 1, 240, data2);  
      tft.readRect(160, 0, 1, 240, data3);  
      tft.readRect(240, 0, 1, 240, data4);  
  
      tft.drawLine(0, 120, 319, 120, CL(255,255,255));
      tft.drawLine(80, 0, 80, 239, CL(255,255,255));
      tft.drawLine(160, 0, 160, 239, CL(255,255,255));
      tft.drawLine(240, 0, 240, 239, CL(255,255,255));
  
      while (!touch.bufferEmpty()) touch.readData(&x,&y,&z);
      if (x && y && z>20) 
      { 
        x = (x-200)/1800; if (x>1) x=1; if (x<0) x=0;
        y = (y-200)/900; if (y>3) y=3; if (y<0) y=0;
        x=(y+x*4);
        switch(x)
        { case 0: currentButtons=1; break;
          case 1: currentButtons=2; break;
          case 2: currentButtons=16; break;
          case 3: currentButtons=32; break;
          case 4: currentButtons=4; break;
          case 5: currentButtons=8; break;
          case 6: currentButtons=64; break;
          case 7: currentButtons=128; break;
        }
      }
      delay(7);
      tft.writeRect(0, 120, 320, 1, data1);  
      tft.writeRect(80, 0, 1, 240, data2);  
      tft.writeRect(160, 0, 1, 240, data3);  
      tft.writeRect(240, 0, 1, 240, data4);  
  
      free(data1);
      free(data2);
      free(data3);
      free(data4);
    }
    else
    { while (!touch.bufferEmpty()) touch.readData(&x,&y,&z);
      currentButtons=0;
    }
  }
      
  // nothing happened
  if (stat==0 && currentButtons==previousButtons) 
    return 0;

  // button pressed or released; start debounce
  if (stat<2 && currentButtons!=previousButtons) 
  { lastChange=millis();
    previousButtons = currentButtons;
    stat=2; // debounce
    //return 0;
  }

  // decode single key from bitflags
  int ckey=-1;
  int cb = currentButtons;
  while(cb) 
  { ckey++;
    cb = cb>>1;
  }

  // lookup key in table
  int kinfo = keys[numButtons*keyBank+max(ckey,0)];
  if (raw) kinfo=ckey+1+AR;

  int autorep = kinfo & AR;
  int longpress = (kinfo>>8) & 255;
  if (!autorep) repeatTime=20000;

  // waiting for end debounce
  if (stat==2 && currentButtons==previousButtons)// && (millis()-lastChange)>1) 
  { lastChange=millis();
    previousButtons=currentButtons;
    if (currentButtons==0)
    { stat=0; // no button pressed
      genKey=true;
      kinfo=lastKey;
    }
    else
    { stat=1; // button just pressed
      kinfo = kinfo & 255;
      autoRepeat=0x7fffffff;
      if (longpress)
      { lastKey=kinfo;
        return 0;
      }
      else
      { genKey=true;
        lastKey=0;
      }
    }      
  }

  // failed debounce
  if (stat==2 && currentButtons!=previousButtons)// && millis()-lastChange>1) 
  { lastChange=millis();
    previousButtons=currentButtons;
    if (currentButtons==0)
      stat=0;
    else
      stat=1;
    return 0;
  }

  // waiting for long press
  if (stat==1 && millis()-lastChange>500 && millis()-autoRepeat>repeatTime && (autorep||longpress)) 
  { genKey=true;
    lastKey=0;
    autoRepeat=millis();

    if (longpress) kinfo = longpress;
    else           kinfo = kinfo & 255;
  }

  // output key, special options for bank selection
  if (genKey && kinfo)
  { if (kinfo==48) 
    { keyBank++;
      if (keyBank>=numBanks) keyBank=0;
      kinfo = 80+keyBank;
    }
    if (kinfo==25) 
      keyBank=0;
    return kinfo;
  }

  return 0;
}
#endif

// visual representation of the Nova
// keys and switches
int keySel=-1;
int novaSwitches = 0;
int novaButton = -1;

// lights
int novaAddress = 0;
int novaCarry = 0;
int novaData = 0;
int novaLights = 0;

#include "nova.image.h"

void updateImage()
{ int lampoff=CL(32, 16, 0);
  int lampon=CL(192,64, 0);
  static int oldsw=-1;
  static int oldsk=-1;
  static int oldks=-2;
  static int oldlights=-1;
  static int olddatasw=-1;
  static int oldaddress=-1;
  static int olddata=-1;
  int vshift=240-93;

  // for testing
  int sw=novaButton; 
  int sk=novaKey;

  bool pwroff = (sk&3)==1;

  // element that overlay bitmap stuff require full repaint
  if (sw!=oldsw || sk!=oldsk || keySel!=oldks)
  { if (keySel<-1) keySel=-1;

    // draw background (darken when switch=OFF)
    if (novaKey==1) 
    { unsigned short *data = (unsigned short *)malloc(gimp_image.width*gimp_image.height*2);
      for (unsigned int i=0; i<gimp_image.width*gimp_image.height; i++)
        data[i] = (((uint16_t*)(gimp_image.pixel_data))[i]&0b1110011110011100)>>2;
      tft.writeRect(0, vshift, gimp_image.width, gimp_image.height, data);
      free(data);
    }
    else
    { if (sw!=oldsw && sk==oldsk && keySel==oldks) // only draw switch area to avoid blinking
        tft.writeRect(0, vshift+49, gimp_image.width, gimp_image.height-49, (uint16_t*)(gimp_image.pixel_data)+49*gimp_image.width);
      else
        tft.writeRect(0, vshift, gimp_image.width, gimp_image.height, (uint16_t*)(gimp_image.pixel_data));
    }

    // draw toggle switches at rest
    for (int i=0; i<10; i++)
          if (isKeyActive(i*2+1) || isKeyActive(i*2+2) || pwroff)
      { tft.fillCircle(52+i*24, vshift+70, 3, pwroff?CL(32,32,32):CL(64,64,64));
      }
      else
        if ((sw>>1) == i) sw=49;

    // and one up or down
    if (sw>=0 && sw<20)
    { if (sw&1)
        tft.fillCircle(53+(sw-1)*12, vshift+74, 3, CL(64,64,64));
      else
        tft.fillCircle(53+(sw)*12, vshift+66, 3, CL(64,64,64));
    }

    // draw rotatory key
    if ((sk&3)==1) tft.fillRect(3, vshift+24, 15, 5, ILI9341_RED);
    if ((sk&3)==2) tft.fillRect(20, vshift+9, 5, 15, ILI9341_RED);
    if ((sk&3)==3) tft.fillRect(26, vshift+24, 15, 5, ILI9341_RED);
    if ((sk&3)==1) tft.drawRect(3, vshift+24, 15, 5, ILI9341_WHITE);
    if ((sk&3)==2) tft.drawRect(20, vshift+9, 5, 15, ILI9341_WHITE);
    if ((sk&3)==3) tft.drawRect(26, vshift+24, 15, 5, ILI9341_WHITE);

    // draw key selection hint
        if (!pwroff)
    { if (isKeyActive(31)) {tft.setCursor(52+225, vshift+40); tft.setTextColor(CL(255,0,0)); tft.print("Data"); }
      if (isKeyActive(1))  {tft.setCursor(30,     vshift+66); tft.setTextColor(CL(128,0,0)); tft.print("AC"); }
      if (isKeyActive(13)) {tft.setCursor(172,    vshift+66); tft.setTextColor(CL(128,0,0)); tft.print("DEP"); }
      if (isKeyActive(15)) {tft.setCursor(230,    vshift+66); tft.setTextColor(CL(128,0,0)); tft.print("EXA"); }
      if (isKeyActive(9))  {tft.setCursor(117,    vshift+66); tft.setTextColor(CL(128,0,0)); tft.print("STOP"); }
      if (isKeyActive(18)) {tft.setCursor(212,    vshift+66); tft.setTextColor(CL(128,0,0)); tft.print("STEP"); }
      if (isKeyActive(19)) {tft.setCursor(278,    vshift+66); tft.setTextColor(CL(128,0,0)); tft.print("PL"); }
        }

    oldsk=sk;
    oldsw=sw;
    oldks=keySel;
  }

  // draw address lights
  for (int i=0; i<15; i++) 
  { if (((novaAddress<<i)&0x4000) != (((oldaddress<<i)&0x4000)))
    { if (((novaAddress<<i)&0x4000)==0 || pwroff)
        tft.fillCircle(97+i*11.8, vshift+11, 2, lampoff);
      else
        tft.fillCircle(97+i*11.8, vshift+11, 2, lampon);
    }
  }
  oldaddress=novaAddress;

  // draw carry and data lights
  int dc = novaData;
  if (novaCarry) dc += 0x10000;
  for (int i=0; i<17; i++)
  { if (((dc<<i)&0x10000) != (((olddata<<i)&0x10000)))
    { if (((dc<<i)&0x10000)==0 || pwroff)
        tft.fillCircle(74+i*11.8, vshift+24, 2, lampoff);
      else
        tft.fillCircle(74+i*11.8, vshift+24, 2, lampon);
    }
  }
  olddata = dc;

  // ion and run lights
  if (oldlights!=novaLights)
  { for (int i=0; i<2; i++) 
    { if (((novaLights>>i)&1)==0 || pwroff)
        tft.fillCircle(274+i*12.5, vshift+12, 2, lampoff);
      else
        tft.fillCircle(274+i*12.5, vshift+12, 2, lampon);
    }

    // fetch, defer, exec lights
    for (int i=0; i<3; i++) 
    { if (((novaLights>>i)&4)==0 || pwroff)
        tft.fillCircle(274+i*12.5, vshift+25, 2, lampoff);
      else
        tft.fillCircle(274+i*12.5, vshift+25, 2, lampon);
    }
  }
  oldlights = novaLights;

  // draw data switches (simplified)
  if (olddatasw!=novaSwitches)
  { for (int i=0; i<16; i++) 
    {  if (((novaSwitches<<i)&0x8000)==0)
        tft.fillCircle(85+i*11.8, vshift+40, 3, (pwroff||!isKeyActive(31))?CL(16,16,16):CL(32, 32, 32));
      else
       tft.fillCircle(85+i*11.8, vshift+40, 3, (pwroff||!isKeyActive(31))?CL(64,64,64):CL(128,128,128));
    }
  }
  olddatasw = novaSwitches;
}

String makespc(int n)
{ String s;
  for (int i=0; i<n; i++) s.concat(" ");
  return s;
}

// Show help screen fitting to selected keyBank
int helpScreen=-1;
void showHelp() {
  int l;
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(CL(192,192,64));
  tft.setTextSize(1);
  tft.setCursor(0,0);
  tft.println("");
#ifdef FEATHERWING_TFT_TOUCH
  tft.println("                    Touch help");
#else
  tft.println("                    Button help");
#endif
  tft.setTextColor(CL(255,64,0));
  tft.println("FUNC: key functions             Press short / long");
#ifdef FEATHERWING_TFT_TOUCH
  int offs=3;
  tft.drawRect(97, 35, 109, 74, CL(32, 128, 32));
  tft.drawLine(97, 35+37, 97+108, 35+37, CL(32, 128, 32));
  tft.drawLine(97+27, 35, 97+27, 35+73, CL(32, 128, 32));
  tft.drawLine(97+54, 35, 97+54, 35+73, CL(32, 128, 32));
  tft.drawLine(97+81, 35, 97+81, 35+73, CL(32, 128, 32));
#else
  int offs=0;
  tft.drawRect(124, 18, 57, 100, CL(32, 128, 32));
#if !defined(PICOD2)
  tft.drawRect(124+15, 18+84, 57-30, 16, CL(32, 128, 32));
#endif
#endif
  tft.println("");
  tft.println("");
#if !defined(PICO) && !defined(PICOD2)
  l=keyHelp(1).length();
  tft.println(makespc(16-l)+keyHelp(1)+makespc(offs)+"   o");
#else
  tft.println("");
  tft.println("");
  tft.println("");
  tft.println("");
#endif
  l=keyHelp(0).length();
  tft.println(makespc(16-l)+keyHelp(0)+makespc(offs/2)+" o");
  tft.setTextColor(CL(192,192,64));
#if !defined(PICO) && !defined(PICOD2)
  tft.println(makespc(31-offs)+"o   "+makespc(offs)+keyHelp(4));
#endif

#if defined(PICOD2)
  tft.println(makespc(12-l)+keyHelp(5)+makespc(offs/2)+" o");
#else
  tft.println(makespc(31-offs/2)+"  o "+makespc(offs/2)+keyHelp(5));
#endif

#if !defined(PICO) && !defined(PICOD2)
  l=keyHelp(3).length();
  tft.println(makespc(16-l)+keyHelp(3)+makespc(offs)+"   o");
  l=keyHelp(2).length();
  tft.println(makespc(16-l)+keyHelp(2)+makespc(offs/2)+" o");
#endif
  tft.println(makespc(31-offs)+"o   "+makespc(offs)+keyHelp(6));
  tft.println(makespc(31-offs/2)+"  o "+makespc(offs/2)+keyHelp(7));
  tft.println("");
  tft.println("");
  l=items[keyBank].length();
  tft.setTextColor(CL(255,64,0));
  tft.println(makespc(23-(l>>1))+"FUNC "+items[keyBank]);
  tft.println("");
  tft.setTextColor(CL(192,192,64));
#if !defined(PICO) && !defined(PICOD2)
  tft.print("Teensy Nova1200 sim mvh; " __DATE__ "; Memory "+String(MEMSIZE)+"kW");
#else
  tft.print("RPPico Nova1200 sim mvh; " __DATE__ "; Memory "+String(MEMSIZE)+"kW");
#endif
}

// monitor display
unsigned char textBuffer[64*18]={0};
unsigned char textBackup[64*18]={0};
int textCol=0;
int textLine=0;
  
void restoreText(bool force)
{ for (int i=0; i<18; i++)
    for (int j=0; j<64; j++)
    { if (force || textBackup[i*64+j]!=textBuffer[i*64+j])
      { tft.setCursor(j*5-1,i*8);
        tft.setTextColor(CL(128,128,96), CL(0,0,0));
        if (textBuffer[i*64+j]==0) textBuffer[i*64+j]=' ';
        tft.write(textBuffer[i*64+j]);
        textBackup[i*64+j]=textBuffer[i*64+j];
      }
    }
}

void clearText(bool force)
{ textCol=0;
  textLine=0;
  Serialwrite(-1); // reset character and graphical modes
  for (int i=0; i<18*64; i++) textBuffer[i]=' ';
  restoreText(force);
}

void scroll()
{ for (int i=64; i<64*18; i++)
  { textBuffer[i-64]=textBuffer[i];
  }
  for (int i=64*17; i<64*18; i++)
  { textBuffer[i]=' ';
    textBackup[i]=0;
  }
  
  restoreText(false);
}

// serial output from Nova with simple graphics terminal
// see program header for documentation
// pass -1 to reset color and graphic modes

void Serialwrite(int a)
{ static int val;
  static int vals[10];
  static bool hasnum=false;
  static unsigned char command;
  static int x=0, y=0;
  static int textColor=CL(128,128,96);
  static int textBgColor=CL(0,0,0);
  static bool inString=false;
  static bool inControl=false;
  static int inCursor=0;
  
  // internal: reset all modes
  if (a==-1)
  { textColor=CL(128,128,96);
    textBgColor=CL(0,0,0);
    inString=false;
    inControl=false;
    inCursor=0;
    return;
  }

  Serial.write(a);
#ifdef WIFI
  if (hasWIFI) WIFI.write(a);
#endif

  if (inCursor && a>='0' && a<='9') // collect graphics terminal number
  { val=10*val+a-'0';
    hasnum=true;
        return;
  }
    
  if (inCursor) 
  { if (hasnum) // store graphics terminal number
    { vals[inCursor-1]=val;
      inCursor++;
      if (inCursor>10) inCursor=0; // protect against array overflow
      hasnum=false;
      val=0;
    }
    if (a==';') // execute graphics terminal command
    { int n=inCursor-1; inCursor=0;
      switch(command)
      { case 'G':
        case 'g': textCol=vals[0]%64; textLine=vals[1]%18; return;
        case 'C':
        case 'c': textColor=CL(vals[0],vals[1],vals[2]); return;
        case 'B':
        case 'b': textBgColor=CL(vals[0],vals[1],vals[2]); return;

        case 'm':
        case 'M': x=vals[0]; y=vals[1]; textCol=textLine=0; return; //avoid scrolling
        case 'l': 
        case 'L': tft.drawLine(x, y, vals[0], vals[1], textColor); 
                          x=vals[0]; y=vals[1]; textCol=textLine=0; return;

        case 'r': tft.drawRect(x, y, vals[0], vals[1], textColor); textCol=textLine=0; return;
        case 'R': tft.fillRect(x, y, vals[0], vals[1], textColor); textCol=textLine=0; return;
        case 'e': tft.drawCircle(x, y, vals[0], textColor); textCol=textLine=0; return;
        case 'E': tft.fillCircle(x, y, vals[0], textColor); textCol=textLine=0; return;

        case 't': 
        case 'T': 
                  int s=1; 
#ifndef USEADAFRUIT
                  s=tft.getTextSize(); 
#endif
                  tft.setTextSize(vals[0]);
                  tft.setTextColor(textColor, textBgColor);
                  tft.setCursor(x, y);
                  for (int i=1; i<n; i++) tft.write(vals[i]); // not for control characters 
                  tft.setTextSize(s); 
                  return;
          }
        }
        return;
  }

  if (a==10) 
  { textCol=0;
    textLine++;
    if (textLine>=18) {scroll(); textLine=17;}
    inString=false;
    inControl=false;
    inCursor=0;
  }
  else if (a==13) 
  { 
  }
  else if (a==12) 
  { clearText(false);
    inString=false;
    inControl=false;
    inCursor=0;
  }
  else
  { if (inControl)
        { inControl=false;
      if (a=='f') 
          { clearText(false);
        return;
          }
      if (a=='F') 
          { clearText(true);
        return;
          }
      if (a=='p' || a=='P') 
          { tft.drawPixel(x, y, textColor);
        return;
          }
      if (a=='g'|| a=='G' || a=='b' || a=='B' || a=='C' || a=='C') 
          { inCursor=1;  // text commands
        command = a;
        return;
          }
      if (a=='m'|| a=='M' || a=='r' || a=='R' || a=='l' || a=='L' || a=='e' || a=='E' || a=='t' || a=='T') 
          { inCursor=1;  // graphics commands
        command = a;
        return;
          }
    }

    if (a=='"') // do not execute control commands while enclosed in ""
          inString=!inString;

    if (a=='\\' && !inString) // start of command
        { inControl=true; 
      command=0;
          val=0;
          hasnum=false;
      return;
        }

    textCol++;
    if (textCol>=64)
    { textLine++;
      textCol=0;
      if (textLine>=18) {scroll(); textLine=17;}
    }
    tft.setCursor(textCol*5-1,textLine*8);
    tft.setTextColor(textColor, textBgColor);
    tft.write(a);
    textBuffer[textLine*64+textCol]=a;
    textBackup[textLine*64+textCol]=a;
  }
}

// Nova simulator from simhv-39

// number of instructions per update of front panel (modify with speed command or button)
static int SIMINTERVAL=973;

// memory size; note: cannot be changed without recreating sessions in ROM and EEPROM

unsigned short NovaMem[MEMSIZE];
int NovaRegs[4];
int NovaPC=0;
int NovaC=0;

bool SerialIO=false;               // if set serial is sent/recieved to Nova
bool novaRunning=false;

#define M NovaMem
#define AC NovaRegs
#define PC NovaPC
#define C NovaC
#include "nova.simh.h"

unsigned int examineAC(int address) {
  novaData = NovaRegs[address&3];
  return NovaRegs[address&3];
}

unsigned int examine(int address) {
  if (address>=MEMSIZE) return 0;
  novaData = NovaMem[address];
  return NovaMem[address];
}

void depositAC(int address, int value) {
  NovaRegs[address&3] = value & 0xffff;
  novaData = value & 0xffff;
}

void deposit(int address, int value) {
  if (address>=MEMSIZE) return;
  NovaMem[address] = value & 0xffff;
  novaData = value & 0xffff;
  novaAddress = address & 0x7fff;
}

unsigned int stopNova(void)
{ novaRunning=false;
  novaData = saved_PC;
  novaLights =
    (novaRunning?1:0)|
    (int_req & INT_ION?2:0)|
    (novaRunning?4:0)|
    (novaRunning?8:0)|
    (0);
  return saved_PC;
}

// start at address a
void startNova(unsigned int a)
{ novaRunning=true;
  saved_PC=NovaPC=a;
  novaLights =
    (novaRunning?1:0)|
    (int_req & INT_ION?2:0)|
    (novaRunning?4:0)|
    (novaRunning?8:0)|
    (0);
}

// continue
// note: examine and deposit affect continue address in real Nova
void continueNova(void)
{ startNova(saved_PC);
  return;
}

#define HALT       063077

// System calls (these all are functional HALT instructions, interpreted by Arduino program on real Nova)
// Unused in Teensy Nova simulator
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

// execute N instruction
void simulateNova(int N)
{ novaLights = (
      (novaRunning?1:0)|
      (int_req & INT_ION?2:0)|
      (novaRunning?4:0)|
      (novaRunning?8:0)|
      (0));
  novaCarry = (NovaC&CBIT?1:0);
  saved_PC=NovaPC;
  int stat = sim_instr(N);
  NovaPC=saved_PC;
  novaAddress = saved_PC;
  novaData = examine(saved_PC);
  novaLights = (
      (novaRunning?1:0)|
      (int_req & INT_ION?2:0)|
      (novaRunning?4:0)|
      (novaRunning?8:0)|
      (0));
  novaCarry = (NovaC&CBIT?1:0);

  if (stat==2) // HALT
  { int haltAddress = saved_PC-1;
    int haltInstruction = examine(haltAddress);
    int haltA0          = examineAC(0);

    //if (kbmode==3) haltInstruction=0; // allow key 9 to stop running

    if (haltInstruction==HALT)
    { Serial.println("HALT at address: "+toOct(haltAddress));
      Serial.print(">");
      stopNova();
      SerialIO=false;
      return; // Normal halt: nova ready for input
    }    
    else if (haltInstruction==INFO)
    { //lcdPrintDebug();
      examine(haltAddress);
      continueNova();
    }
    else if (haltInstruction==PUTC) // write character
    { byte b=haltA0&255;
      Serial.write(b);
      //putLCD(b);
      examine(haltAddress);
      continueNova();
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
          //putLCD(b);
        }
        else
          flag=false;
        b = s&255;
        if (b==0) break;
        if (!flag)
        { if (b=='%') { Serial.print(String((short)examineAC((s2>>8)&3))); flag=true; }
          else Serial.write(b);
          //putLCD(b);
        }
        else
          flag=false;
      }
      examine(haltAddress);
      continueNova();
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
    }
    else if (haltInstruction==READBLOCK) // stored BIGENDIAN in eeprom
    { unsigned int A2=examineAC(2);
      //gpio(1);
      //readBlockfromEEPROM(haltA0, A2);
      depositAC(0, haltA0+1);
      depositAC(2, A2+64);
      //gpio(0);
      examine(haltAddress);
      continueNova();
    }
    else if (haltInstruction==WRITEBLOCK) // read BIGENDIAN from eeprom
    { unsigned int A2=examineAC(2);
      //gpio(4);
      //writeBlocktoEEPROM(haltA0, A2);
      //gpio(0);
      depositAC(0, haltA0+1);
      depositAC(2, A2+64);
      examine(haltAddress);
      continueNova();
    }
    else if (haltInstruction==GPIO)     // GPIO through MCP20008
    { //gpio(haltA0);
      examine(haltAddress);
      continueNova();
    }
    else if (haltInstruction==DELAY)   // delay on arduino
    { examineAC(1); // show ac1 on lights
      delay(haltA0);
      examine(haltAddress);
      continueNova();
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
    }
    else if (haltInstruction==GETC)
    { //break; // handled in serial and keyboard code
    }
  }
}

// execute one instruction
void stepNova(void)
{ novaRunning = true;
  simulateNova(1);
  novaRunning = false;
  novaLights = (
      (novaRunning?1:0)|
      (int_req & INT_ION?2:0)|
      (novaRunning?4:0)|
      (novaRunning?8:0)|
      (0));
}

void memstepNova(void)
{ stepNova(); // not implemented, same as step
  return;
}

void resetNova(void)
{ novaRunning = false;
  NovaPC=0;
  novaCarry=0;
  saved_PC=0;
  reset_all(0);
  //int_req = 03000000;
  Serial.println("Reset");
  novaLights =
    (novaRunning?1:0)|
    (int_req & INT_ION?2:0)|
    (novaRunning?4:0)|
    (novaRunning?8:0)|
    (0);
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

// mini debugger display on serial interface
void serialDebug(int mode) {
  unsigned int pc = NovaPC;
  unsigned int carry = NovaC!=0;
  unsigned int in = NovaMem[NovaPC], a0, a1, a2=0, a3=0;
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

// mini debugger display on serial interface
#ifdef WIFI
void WIFIDebug(int mode) {
  unsigned int pc = NovaPC;
  unsigned int carry = NovaC!=0;
  unsigned int in = NovaMem[NovaPC], a0, a1, a2=0, a3=0;
  a0=examineAC(0);
  a1=examineAC(1);
  a2=examineAC(2);
  a3=examineAC(3);
  if (mode&1)
  { WIFI.print("ac0:");
    WIFI.print(toOct(a0));
    WIFI.print(" 1:");
    WIFI.print(toOct(a1));
    WIFI.print(" 2:");
    WIFI.print(toOct(a2));
    WIFI.print(" 3:");
    WIFI.print(toOct(a3));
    WIFI.print(" ");

    if (carry)
      WIFI.println("C");
    else
      WIFI.println(".");
  }

  if (mode&2)
  { in = examine(pc);
    WIFI.print(toOct(pc));    // print address
    WIFI.print(printDisas(in, OCT));    // disassemble instruction
  
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
      WIFI.print(" ");
      if (b>3) WIFI.print("@");
      if (b>0) WIFI.print(toOct(t));
  
      for (int i=0; i<indirect; i++)
      { if (b>3) do t=examine(t); while (t&0x8000);
        else t=examine(t);
      }
           
      if (indirect)
      { WIFI.print("->");
        WIFI.print(toOct(t));
      }
    }
  }
  examine(pc);
}
#endif

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

#if defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_RP_PICO)
#  define EEPROMWORDS 2020  // for Teensy 3.5 and 4.1; has 4096 bytes, 56 reserved for other data
#else
#  ifdef ARDUINO_TEENSY40
#    define EEPROMWORDS 512 // for Teensy 4.0, has 1080 bytes
#  else
#    define EEPROMWORDS 994 // for Teensy 3.0-3.2, has 2048 bytes
#  endif
#endif
int eepromwords = EEPROMWORDS;
// if SD card is found support 10000 words

#define ADD_DIFF(a) { if (ndiff<eepromwords) diffs[ndiff]=(a); ndiff++; };

// print memory or difference p[]-memory  as C-array to be embedded in the code 
// or store it in eeprom with given ID as parameter if non-0
void unload(const unsigned short *p, int eeprom, int base)
{ unsigned int startval=0xffffffff;
  int countsame=0;
  int block=0;
  int length=0;
  unsigned short *diffs = (unsigned short *)malloc(eepromwords*2);
  int ndiff=0;
  int id = micros()&0xffff;
  if (eeprom) id=eeprom;
  if (!eeprom) Serial.println(String(id) + "," + String(base) + ",");
  ADD_DIFF(id);
  ADD_DIFF(base);

  for (int i=0; i<=MEMSIZE; i++)
  { unsigned short v=examine(i);
    if (p) v = (p[i]-v)&0xffff;

    if (v==startval && i<MEMSIZE) 
      countsame++;
    else 
    { if (countsame > 3 || i==MEMSIZE)
      { if (length)
        { if (!eeprom) Serial.print(String(block) + "," + String(length) + ",");
          ADD_DIFF(block);
          ADD_DIFF(length);
          for (int j=0; j<length; j++) 
          { unsigned short v=examine(block+j);
            if (p) v = (p[block+j]-v)&0xffff;
            if (!eeprom) Serial.print(String(v)); 
            ADD_DIFF(v);
            if (!eeprom) Serial.print(","); 
          }
          if (!eeprom) Serial.println();
        }
        if (!eeprom) Serial.println(String(block+length+32768) + "," + String(countsame) + "," + String(startval) + ",");
        ADD_DIFF(block+length+32768);
        ADD_DIFF(countsame);
        ADD_DIFF(startval);
        block=i;
        length=0;
      }
      else
        length += countsame;
      startval=v;
      countsame=1;
    }

    if (length>=12 || (i==MEMSIZE && length>0))
    { if (!eeprom) Serial.print(String(block) + "," + String(length) + ",");
      ADD_DIFF(block);
      ADD_DIFF(length);
      for (int j=0; j<length; j++) 
      { unsigned short v=examine(block+j);
        if (p) v = (p[block+j]-v)&0xffff;
        if (!eeprom) Serial.print(String(v)); 
        ADD_DIFF(v);
        if (!eeprom) Serial.print(","); 
      }
      if (!eeprom) Serial.println();
      block=i;
      length=0;
      startval=v;
    }
    if (!eeprom) 
      while (!Serial.availableForWrite()) delay(10);
  }
  
  Serial.println("Length (words): " + String(ndiff));
  if (eeprom && ndiff>=eepromwords) 
  { Serial.println("Session does not fit in eeprom; not stored");
    tft.setCursor(0,0);
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.println("** Session does not fit in eeprom; not stored **");
    delay(5000);
  }
#ifdef SD_CS
  else if (eeprom && hasSD)
  { unsigned short s=ndiff;
    myFile = SD.open("tnses1.bin", FILE_WRITE);
    myFile.seek(0);
    myFile.write(s>>8);
    myFile.write(s&255);
    for (int i=0; i<ndiff; i++)
    { unsigned short s=diffs[i];
      myFile.write(s>>8);
      myFile.write(s&255);
    }
    myFile.close();

    Serial.println("Used SD(of "+String(eepromwords*2)+"): " + String(ndiff*2));
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0,0);
    tft.println("Used SD: " + String(ndiff*2));
    delay(1000);
    tft.setCursor(0,0);
    tft.println("                             ");
    tft.setTextSize(1);
    restoreText(true);
  }
#endif
  else if (eeprom)
  { for (int i=0; i<ndiff; i++)
    { unsigned short s=diffs[i];
      EEPROM.write(56+i*2, s>>8);
      EEPROM.write(56+i*2+1, s&255);
    }
    unsigned short s=ndiff;
    EEPROM.write(38, s>>8);
    EEPROM.write(39, s&255);
    Serial.println("Used eeprom(of "+String(eepromwords*2)+"): " + String(ndiff*2));
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0,0);
    tft.println("Used eeprom: " + String(ndiff*2));
    delay(1000);
    tft.setCursor(0,0);
    tft.println("                             ");
    tft.setTextSize(1);
    restoreText(true);
  }
  
  free(diffs);
}

// Load session data or difference
int sessionloader(const unsigned short *p, int len, String name, int base)
{  Serial.println("Loading session: "+name);
   int i=2; // skip ID and base
   
   if (base != p[1])
   { Serial.print("*** Error session difference with wrong base ***");
     tft.setCursor(0,0);
     tft.print("*** Error session difference with wrong base ***");
         delay (5000);
         return -1;
   }

   if (len<=0)
   { Serial.print("*** Error loading unknown session ***");
     tft.setCursor(0,0);
     tft.print("*** Error loading unknown session ***");
         delay (5000);
         return -1;
   }

   while (i<len)
   { unsigned short a = p[i++];

     // data block
     if ((a&0x8000)==0)
     { unsigned short adr = a;
       unsigned short wc = p[i++];
       
       //Serial.println(toOct(wc)+':'+toOct(adr));
       for (int j=0; j<wc; j++)
       { unsigned short data = p[i++];
         if (base) data += examine(adr+j);
         deposit(adr+j, data&0xffff);
       }
     }
     // constant block
     else if (a&32768)
     { unsigned short adr = a & 0x7fff;
       unsigned short wc = p[i++];
       //Serial.println(toOct(wc)+' '+toOct(adr));
       unsigned short data = p[i++];
       for (int j=0; j<wc; j++)
       { unsigned short d=data;
         if (base) d += examine(adr+j);
         deposit(adr+j, d&0xffff);
       }
     }
     // start address block (not implemented)
     else if (a == 0xffff)
     { //unsigned short adr = p[i++];
       //startaddress = adr;
       break;
     }
   }
   
   return p[0];
}

#include "basic.session.h"
#include "basic.code.h"
#include "basic.pong.h"

int sessionID=0, rootID=0; // current session except EEPROM changes, session root

// Session support functions; most are variables in this code
// Get length
int getSessionLength(int id)
{ if (id==basicsession[0]) return sizeof(basicsession)/2;
  if (id==basiccode[0]) return sizeof(basiccode)/2;
  if (id==basicpong[0]) return sizeof(basicpong)/2;
#ifdef SD_CS
  if (hasSD)
  { myFile = SD.open("tnses1.bin");
    if (myFile)
    { myFile.seek(0);
      int lenf = (myFile.read()<<8)+myFile.read();
      int idf = (myFile.read()<<8)+myFile.read();
      myFile.close();
      if (id==idf) return lenf;
    }
  }
#endif
  if (id==(EEPROM.read(56)<<8)+EEPROM.read(57)) return (EEPROM.read(38)<<8)+EEPROM.read(39);
  return 0;
}
  
// Get ID of base session
int getSessionBase(int id)
{ if (id==basicsession[0]) return basicsession[1];
  if (id==basiccode[0]) return basiccode[1];
  if (id==basicpong[0]) return basicpong[1];
#ifdef SD_CS
  if (hasSD)
  { myFile = SD.open("tnses1.bin");
    if (myFile)
    { myFile.seek(0);
      myFile.read();
      myFile.read();
      int idf = (myFile.read()<<8)+myFile.read();
      int basef = (myFile.read()<<8)+myFile.read();
      myFile.close();
      if (id==idf) return basef;
    }
  }
#endif
  if (id==(EEPROM.read(56)<<8)+EEPROM.read(57)) return (EEPROM.read(58)<<8)+EEPROM.read(59);
  return 0;
}

// Get pointer to session data
const unsigned short *getSessionAddress(int id)
{ static unsigned short *eepromcopy=NULL;
  if (eepromcopy==NULL) 
    eepromcopy = (unsigned short *)malloc(eepromwords*2);
  if (id==basicsession[0]) return basicsession;
  if (id==basiccode[0]) return basiccode;
  if (id==basicpong[0]) return basicpong;
#ifdef SD_CS
  if (hasSD)
  { myFile = SD.open("tnses1.bin");
    if (myFile)
    { myFile.seek(0);
      int lenf = (myFile.read()<<8)+myFile.read();
      int idf = (myFile.read()<<8)+myFile.read();
      int basef = (myFile.read()<<8)+myFile.read();
      if (id==idf)
      { eepromcopy[0]=idf;
        eepromcopy[1]=basef;
        for (int i=0; i<lenf; i++) 
          eepromcopy[i+2] = (myFile.read()<<8)+myFile.read();
        myFile.close();
        return eepromcopy;
      }
      myFile.close();
    }
  }
#endif
  if (id==(EEPROM.read(56)<<8)+EEPROM.read(57))
  { for (int i=0; i<EEPROMWORDS; i++) 
      eepromcopy[i] = (EEPROM.read(56+i*2)<<8)+EEPROM.read(57+i*2);
    return eepromcopy;
  }
  return NULL;
}

// Get ID by name (or alias)
int getSessionID(String name)
{ if (name==".BASIC") return basicsession[0];
  if (name==".BASICSESSION") return basicsession[0];
  if (name==".BASICSAMPLE") return basiccode[0];
  if (name==".PONG") return basicpong[0];
  if (name==".BASICPONG") return basicpong[0];
#ifdef SD_CS
  if (hasSD && name==".SD")
  { myFile = SD.open("tnses1.bin");
    if (myFile)
    { myFile.seek(0);
      myFile.read();
      myFile.read();
      int idf = (myFile.read()<<8)+myFile.read();
      myFile.close();
      return idf;
    }
  }
#endif
  if (name==".EEPROM") return (EEPROM.read(56)<<8)+EEPROM.read(57);
  if (name.toInt()) return name.toInt();
  return -1;
}
        
// Get name of session by ID
String getSessionName(int id)
{ if (id==basicsession[0] || id==-1) return ".BASICSESSION";
  if (id==basiccode[0]    || id==-2) return ".BASICSAMPLE";
  if (id==basicpong[0]    || id==-3) return ".BASICPONG";
#ifdef SD_CS
  if (hasSD)
  { myFile = SD.open("tnses1.bin");
    if (myFile)
    { myFile.seek(0);
      myFile.read();
      myFile.read();
      int idf = (myFile.read()<<8)+myFile.read();
      myFile.close();
      if (id==idf || id==-4) return ".SD";
    }
  }
#endif
  if (id==(EEPROM.read(56)<<8)+EEPROM.read(57) || id==-5) return ".EEPROM";
  return "";
}

// Restore session hierarchy from code memory and/or eeprom
int restoreSession(String name)
{ int ids[10];
  ids[0] = getSessionID(name); // last level

  if (ids[0]<=0)
  { Serial.println("** No session found **");     
    tft.println("** No session found **");
    return 0;
  }

  for (int i=0; i<9; i++) // look up base levels
  { ids[i+1] = getSessionBase(ids[i]);
  }
        
  memset(NovaMem, 0, MEMSIZE*2);

  int id = 0; // load from root to last level
  rootID = 0;
  for (int i=9; i>=0; i--)
  { if (ids[i])
    { id=sessionloader(getSessionAddress(ids[i]), 
                       getSessionLength(ids[i]), 
                       getSessionName(ids[i]), 
                       getSessionBase(ids[i]));
      if (getSessionName(id)!=".EEPROM" && getSessionName(id)!=".SD")
        sessionID = id;
      if (rootID==0)
        rootID = id;
    }
  }

  if (id==0)
  { Serial.println("** No session found **");     
    tft.println("** No session found **");
    delay(5000);
  }

  return id;
}

// store difference with current session to EEPROM
void unloadDiffToEEPROM()
{ if (sessionID==0) 
  { tft.setCursor(0,0);
    tft.println("** no session loaded **");
        delay(5000);
    return;
  }
  unsigned short *q=(unsigned short *)malloc(MEMSIZE*2);
  memcpy(q, NovaMem, MEMSIZE*2);
  int id=restoreSession(String(sessionID));
  unload(q, micros() & 0xffff, id); // stores memory-session to eeprom if fits
  memcpy(NovaMem, q, MEMSIZE*2);
  free(q);
}

// print difference with given session as code that can be embedded
void unloadDiffCode(String name)
{ if (name==".EEPROM") 
  { Serial.println("** cannot use EEPROM as base **");
    return;
  }
  if (name==".SD") 
  { Serial.println("** cannot use SD as base **");
    return;
  }
  unsigned short *q=(unsigned short *)malloc(MEMSIZE*2);
  memcpy(q, NovaMem, MEMSIZE*2);
  int id=restoreSession(name);
  if (id==0)
  { Serial.println("** cannot find session **");
    return;
  }
  unload(q, 0, id); // prints difference as code
  memcpy(NovaMem, q, MEMSIZE*2);
  free(q);
}

// Store session (difference of sessionID with code memory) to eeprom
void storeSession(void)
{ unloadDiffToEEPROM();
}

// Use whole screen for menu lines, returns choice, optionally injects as serial data
int menu(String info, String menuString, int ch, bool inject)
{ tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
  int offs=0;
#ifdef FEATHERWING_TFT_TOUCH
  offs=10;
#endif  
  tft.setCursor(230, 200+offs);
  tft.print("X X");
  tft.setCursor(230, 220+offs);
  tft.print("L R");
  tft.setCursor(270, 210);
  tft.print("U cancel");
  tft.setCursor(270, 230);
  tft.print("D select");

  tft.setTextSize(2);
  int pos;
  int choice=max(ch,0);
  tft.setCursor(20, 10);
  tft.print(info);
  while(true)
  { int N=0;
    int col=30;
    String text="";
    String s = menuString;
    while((pos=s.indexOf("\n"))>=0)
    { tft.setCursor(col, 30+20*(N%10));
      if (N==choice) 
      { tft.print(">");
        text = s.substring(0, pos);
      }
      else
        tft.print(" ");
      tft.print(s.substring(0, pos));
      s.remove(0, pos+1);
      N++;
      if (N%10==0) col=col+30;
    }
	
    int k=getButtonPress(true);
	if (ps2kb.available())
	{ k=ps2kb.read();
      switch(k)
	  { case PS2_PAGEUP: k=3; break;
	    case PS2_PAGEDOWN: k=4; break;
		case PS2_UPARROW: k=5; break;
		case PS2_DOWNARROW: k=7; break;
		case 13: k=8; break;
		case 27: k=6; break;
		default: k=0;
	  }
	}
	
    tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
    if (k==8) 
    { tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(1);
      if (helpScreen>=0) showHelp(); else restoreText(true);
      keySel=-99;
      if (inject && text.length()==1) injectSerial(text);
      if (inject && text.length()>1) injectSerial(text + "\r");
      return choice;
    }
    if (k==6) 
    { tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(1);
      if (helpScreen>=0) showHelp(); else restoreText(true);
      keySel=-99;
      return -1;
    }
    if (k==5) choice=(choice+10*N-1) % N;
    if (k==7) choice=(choice+10*N+1) % N;
    if (k==3) choice=(choice+10*N-10) % N;
    if (k==4) choice=(choice+10*N+10) % N;
    delay(10);
  }
  return 0;
}

// Use whole screen for keyboard entry injected as serial data
int keyboard(String info, String menuString, int ch)
{ String t="";
  static String previous="";

  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
  int offs=0;
#ifdef FEATHERWING_TFT_TOUCH
  offs=10;
#endif  
  tft.setCursor(230, 200+offs);
  tft.print("- +");
  tft.setCursor(230, 220+offs);
  tft.print("L R");
  tft.setCursor(270, 210);
  tft.print("U cancel");
  tft.setCursor(270, 230);
  tft.print("D enter");

  tft.setTextSize(2);
  int choice=max(ch,0);
  tft.setCursor(20, 10);
  tft.print(info);
  while(true)
  { int N=0;
    int col=30;
    String text="";
    String s = menuString;
    while(s.length()>0)
    { tft.setCursor(30*floor(N%10), col);
      if (N==choice) 
      { tft.print(">");
        text = s.substring(0, 1);
      }
      else
        tft.print(" ");
      byte b=s.charAt(0);
      if (b<' ')
      { tft.setTextSize(1);
        switch(b)
        { case 0x11: tft.print("Prv"); break;
          case 0x1: tft.print("C-A"); break;
          case 0x1b: tft.print("Esc"); break;
          case 0x0d: tft.print("CR"); break;
          default: tft.print("\\"+String(b,HEX));
        }
        tft.setTextSize(2);
      }
      else
        tft.print(s.charAt(0));
      s.remove(0, 1);
      N++;
     if (N%10==0) col=col+20;
    }
    tft.setCursor(0, 180);
    for (unsigned int i=0; i<40; i++)
    { if (i>=t.length()) tft.print(" ");
      else if(t[i]==' ') tft.print("_");
      else tft.print(t[i]);
    }


    int k=getButtonPress(true);
    tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
    if (k==2) 
      t = t + text;
    if (k==1) 
      t.remove(t.length()-1);
    if (k==8) 
    { if (text=="\x11") 
        t = previous;
      else
      { tft.fillScreen(ILI9341_BLACK);
        tft.setTextSize(1);
        if (helpScreen>=0) showHelp(); else restoreText(true);
        keySel=-99;
        if (t=="") 
        { injectSerial(text);
          return 0;
        }
        injectSerial(t+"\r");
        previous=t;
        return 0;
      }
    }
    if (k==6) 
    { tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(1);
      if (helpScreen>=0) showHelp(); else restoreText(true);
      keySel=-99;
      return -1;
    }
    if (k==5) choice=max(choice-10, 0);
    if (k==7) choice=min(choice+10, N-1);
    if (k==3) choice=max(choice-1, 0);
    if (k==4) choice=min(choice+1, N-1);
    delay(10);
  }
  return 0;
}

int tempHelp=0;
int steppinghelp=0; 

// process button choices
void processButton(int but)
{ static int t=0;
  static int ch1=0;
  static int ch2=0;
  static int ch3=0;

  // buttons 3-8 toggle help if power off
  if (but && novaKey<2)
  { if (novaKey<2 && but<48 && but!=26 && but!=49)
	{ if (helpScreen<0) about();
      but=26;
    }             
  }

  // animate key toggle
  if (but>0 && but<25) t=millis();
  if (but==0 && t && millis()-t>200) { novaButton=-1; t=0; updateImage(); }
   
  // real Nova keys
  if (but==1) { novaButton=0; depositAC(0, novaSwitches); novaData = novaSwitches; }
  if (but==3) { novaButton=2; depositAC(1, novaSwitches); novaData = novaSwitches; }
  if (but==5) { novaButton=4; depositAC(2, novaSwitches); novaData = novaSwitches; }
  if (but==7) { novaButton=6; depositAC(3, novaSwitches); novaData = novaSwitches; }
  
  if (but==2) { novaButton=1; novaData = examineAC(0); }
  if (but==4) { novaButton=3; novaData = examineAC(1); }
  if (but==6) { novaButton=5; novaData = examineAC(2); }
  if (but==8) { novaButton=7; novaData = examineAC(3); }

  if (but==9) { novaButton=8; resetNova(); SerialIO=false; }
  if (but==10) { novaButton=9; stopNova(); SerialIO=false; }

  if (but==11) { novaButton=10; startNova(novaSwitches); SerialIO=true; steppinghelp=0; }
  if (but==12) { novaButton=11; startNova(saved_PC); SerialIO=true; steppinghelp=0; }

  if (but==13) { novaButton=12; saved_PC=novaAddress=novaSwitches; deposit(novaAddress, novaSwitches); }
  if (but==14) { novaButton=13; novaAddress++; deposit(novaAddress, novaSwitches); }

  if (but==15) { novaButton=14; saved_PC=novaAddress=novaSwitches; novaData = examine(novaAddress); }
  if (but==16) { novaButton=15; novaAddress++; novaData = examine(novaAddress); }

  if (but==17) { novaButton=16; stepNova(); steppinghelp=1; }
  if (but==18) { novaButton=17; memstepNova(); steppinghelp=1; }

  // program load restores eeprom session and its dependencies and starts it
  if (but==19) 
  { novaButton=18; 
    int id;
    if (hasSD) id=restoreSession(".SD"); 
    else id=restoreSession(".EEPROM"); 
    if (id==0) 
    { id=restoreSession(".BASICSAMPLE"); 
    }
    if (id==0) 
    { return;
    }
    novaAddress=2; novaSwitches=2;  
    startNova(2);
    SerialIO=true;
    clearText(true);
    injectSerial("\x1b");
  }

  // long push program load stores session modifies to eeprom
  if (but==20) { novaButton=19; storeSession(); 
#if defined(PICO) || defined(PICOD2) || defined(ARDUINO_LOLIN32)
                 EEPROM.commit(); 
#endif
               }

  // menus for running software
  if (but==21) 
  { injectSerial("\x1b");
    Serialwrite(-1); 
  }
  if (but==22) 
  { ch2=keyboard("Keyboard", "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ  ,\\ =+-*/^<>: ;@[]()\"\'?\x11_!#$%&\x01\x1b\r", ch2); 
  }
  if (but==23) { if (SIMINTERVAL>9000) SIMINTERVAL=1; //set speed
                 else if (SIMINTERVAL<2) SIMINTERVAL=31; 
                 else if (SIMINTERVAL<100) SIMINTERVAL=971;
                 else SIMINTERVAL=39731;
                 keySel=-99; updateImage();
               }
  if (but==24) 
  { Serialwrite(-1); 
    if (novaRunning)
      ch1=menu("Run - Basic commands", "\x1bLIST\n\x1bRUN\n\x1b\n", ch1, true);
    else
      ch3=menu("Stop - Monitor commands", "tape .basic\nsession .basic\nsession .pong\nsession .basicsample\nautostart\nrestore\nstore\nclear 0 77777\nabout\n", ch3, true);
  }

  // increment data switches one button per octet
  if (but==31) { novaSwitches=((novaSwitches+0100000)&0100000)+(novaSwitches&~0100000); }
  if (but==32) { novaSwitches=((novaSwitches+0010000)&0070000)+(novaSwitches&~0070000); }
  if (but==33) { novaSwitches=((novaSwitches+0001000)&0007000)+(novaSwitches&~0007000); }
  if (but==34) { novaSwitches=((novaSwitches+0000100)&0000700)+(novaSwitches&~0000700); }
  if (but==35) { novaSwitches=((novaSwitches+0000010)&0000070)+(novaSwitches&~0000070); }
  if (but==36) { novaSwitches=((novaSwitches+0000001)&0000007)+(novaSwitches&~0000007); }

  // zero data switches one button per octet
  if (but==41) { novaSwitches=novaSwitches&~0100000; }
  if (but==42) { novaSwitches=novaSwitches&~0070000; }
  if (but==43) { novaSwitches=novaSwitches&~0007000; }
  if (but==44) { novaSwitches=novaSwitches&~0000700; }
  if (but==45) { novaSwitches=novaSwitches&~0000070; }
  if (but==46) { novaSwitches=novaSwitches&~0000007; }

  // show help if changing function pages
  if (but>=80 && but<90 ) keySel=-99; 
  if (but>=80 && but<90 && helpScreen>=0) showHelp(); 

  if (but==25) { tft.setCursor(0, 0); // cls
                 tft.fillScreen(ILI9341_BLACK);
                 keySel=-99; // forces repaint
                 helpScreen=-1;
                 clearText(true);
                 keyBank=defaultKeyBank;
               }

  if (but==26) 
               { keySel=-99; // help
                 if (helpScreen<0) 
                 { helpScreen=0; showHelp();  
                 }
                 else
                 { helpScreen=-1; restoreText(true);
                 }
               } 

  if (but==49) { novaKey = novaKey!=2?2:1; // power 
                 novaRunning = false;
                 keyBank=defaultKeyBank;
                 reset_all(0);
                 SIMINTERVAL=973; // default
                 
#if !defined(FEATHERWING_TFT_TOUCH) && !defined(ARDUINO_LOLIN32) && !defined(PICOD2)
                 // flash lights when EEPROM read/write
                 if (novaKey==2) 
                 { analogWrite(14, 100); 
                   digitalWrite(15, 0);
                 }
                 else
                 { analogWrite(14, 200); 
                   digitalWrite(15, 1);
                 }
#endif
#if false && defined(PICOD2)
                 // flash lights when EEPROM read/write
                 if (novaKey==2) 
                 { analogWrite(LED_RED, 100); 
                   digitalWrite(LED_GREEN, 0);
                 }
                 else
                 { analogWrite(LED_RED, 200); 
                   digitalWrite(LED_GREEN, 1);
                 }
#endif

#ifdef PS2_VCC
                 if (novaKey==2) 
                 { digitalWrite(PS2_VCC, 1); 
#ifdef PS2_VCC2
                   digitalWrite(PS2_VCC2, 1); // kb ON
#endif
                 }
                 else
                 { digitalWrite(PS2_VCC, 0); 
#ifdef PS2_VCC2
                   digitalWrite(PS2_VCC2, 1); // kb ON
#endif
                 }
#endif
      
                 // analogWrite(16-novaKey, 20); analogWrite(13+novaKey, 0);
                 if (novaKey==2) 
                 { EEPROM.get(0, novaSwitches);
                   if (hasSD) restoreSession(".SD");
                   else restoreSession(".EEPROM");
                 };
                 if (novaKey==1) 
                 { EEPROM.put(0, novaSwitches); 
                   storeSession();
                   novaSwitches=0; 
#if defined(PICO) || defined(PICOD2) || defined(ARDUINO_LOLIN32)
                    EEPROM.commit(); 
#endif
                 };
     
                 novaLights=0;
                 novaAddress=0;
                 novaCarry=0;
                 novaData=examine(0);

                 delay(300); 

#if !defined(FEATHERWING_TFT_TOUCH) && !defined(ARDUINO_LOLIN32) && !defined(PICOD2)
                 // Dim green light when ON
                 if (novaKey==2) 
                   analogWrite(14, 3); 
                 else 
                 { analogWrite(14, 0);
                   digitalWrite(15, 0);
                 }
#endif
#if false && defined(PICOD2)
                 // Dim green light when ON
                 if (novaKey==2) 
                   analogWrite(LED_GREEN, 3); 
                 else 
                 { analogWrite(LED_GREEN, 0);
                   digitalWrite(LED_RED, 0);
                 }
#endif

                 tft.setCursor(0, 0);
                 tft.fillScreen(ILI9341_BLACK);
                 keySel=-99; // forces repaint
                 clearText(true);
                 if (helpScreen>=0) 
                   showHelp();
                 else if (novaKey>1)
                 { tempHelp=millis();
                   showHelp();
                 }
               }
}

// ------------------------------------------------------------
// Serial interface description, interactive mode all numbers in octal

String helpstring=
String("Interactive debugger/supervisor for Nova1200 simulator:\r\n")+
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
//String("load addr block cnt   load RAM from eeprom blocks\r\n")+
//String("save addr block cnt   save RAM to eeprom blocks\r\n")+
String("tape filename         Load tape data from code memory\r\n")+
String("tape list             List all tapes in eeprom and code memory\r\n")+
String("session filename      load session from code memory \r\n")+
String("session list          List all sessions from code memory\r\n")+
String("store                 store session difference to eeprom\r\n")+
String("restore               restore session and difference from eeprom\r\n")+
String("unload                print session as numbers to be embedded in code\r\n")+
String("unloaddiff filename   print difference to session as numbers to be embedded in code\r\n")+
//String("test n                run test function\r\n")+
String("version               print version\r\n")+
String("about                 print version on TFT\r\n")+
//String("lcd line text         display line 0..4 on LCD\r\n")+
String("led N                 program GPIO and Arduino LED (400)\r\n")+
String("adc N                 read 2,3=A2,A3\r\n")+
//String("dac N                 write A14\r\n")+
String("reads                 read switches\r\n")+
String("readk                 read key\r\n")+
String("writed N              write data (without NOVA)\r\n")+
String("writea N              write address (without NOVA)\r\n")+
String("writel N              write lights (without NOVA)\r\n")+
//String("eeprom block          hex dump eeprom block to output\r\n")+
String("memory address        hex dump memory block to output\r\n")+
String("plot address range    plot 80 values (+/- range)\r\n")+
String("vis address count     visualize memory as gray scale, default 1600 chars\r\n")+
String("init                  init Arduino supervisor\r\n")+
String(":nnaaaa[hhhh]         Deposit nn memory locations (a and h hex)\r\n")+
//String(";naaaa[hh*16]         Write line n in eeprom block (a and h hex)\r\n")+
String("? $                   Write in hex ?lights ADSS $regs 1234pC\r\n")+
//String("~                     Switch to non-interactive mode\r\n")+
String("help                  Show this text\r\n")+
String("autostart             execute proposed next command\r\n")+
String("(enter)               automatically propose next command");

String line;
String nextcmd;
#define NHISTORY 40
String history[NHISTORY];
int nhist=0;
bool kmode=false;
int kaddress=0;

// support to inect 'serial' data from code or buttons
String injectSerialText="";
int injectSerialDelay=0;

byte Serialread()
{ if (Serial.available())
    return Serial.read();
  
#ifdef WIFI
  if (WIFI.available())
  { hasWIFI = true;
    return WIFI.read();
  }
#endif
  
  if (injectSerialText.length())
  { byte b = injectSerialText.charAt(0);
    injectSerialText.remove(0, 1);
        return b;
  }
  
  return '$';
}

void injectSerial(byte b)
{ injectSerialText.concat(b);
}

void injectSerial(String s)
{ injectSerialText.concat(s);
}

void about(void)
{ tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(0,0);
  tft.println();
  tft.println("Teensy Nova1200 by Marcel van Herk " __DATE__);
  tft.println();
  tft.setTextColor(CL(192,192,64));
  tft.println("Data General Nova - 1st 16 bit minicomputer in 1969");
  tft.println("This circuit simulates a 1970 DG Nova 1200 machine,");
  tft.println("equipped with 1 serial board and 4 8 KW core boards.");
  tft.println("It runs the original 1970 DG BASIC software. You");
  tft.println("can load and modify different 'sessions' to demo.");
  tft.println();
  tft.println("Note: the graphics terminal is an anachronism ;->>>");
  tft.println();
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Contains parts of simhv3-9");
  tft.println("Copyright (c) 1993-2008, Robert M. Supnik");
  tft.println();
  tft.println("Contains image from FrontPanel 2.0");
  tft.println("Copyright (c) 2007-2008, John Kichury");
  tft.println();
  tft.setTextColor(CL(192,192,64));
  tft.println("** Press button or key to return **");
  Serial.println("** Press button or key to continue **");
  keySel=-99;
  updateImage();
  while(!getButtonPress(true) && !Serial.available() && !ps2kb.available());
  if (Serial.available()) Serial.read();
#ifdef WIFI
  if (WIFI.available()) WIFI.read();
#endif
  if (ps2kb.available()) ps2kb.read();
  restoreText(true);
}

// process Serial data to pass to Nova or for monitor
void processSerial(int count)
{ int av = Serial.available();
  byte b = Serialread();
  char m[50];
  int pc;
  int a;

  // power ON if not
  if (novaKey==1)
  { processButton(49);
    return;
  }
  
  if (b==PS2_F1)
  { processButton(49); // on-off
    return;
  }
  else if (b==PS2_F2)
  { processButton(19); // restore
    return;
  }
  else if (b==PS2_F3)
  { processButton(20); // store
    return;
  }
  else if (b==PS2_F4)
  { processButton(23); // speed
    return;
  }
  else if (b==PS2_F5) 
  { about();           // about
    return;
  }
  else if (b==PS2_F6)
  { processButton(25); // cls
    return;
  }
  else if (b==PS2_F7)
  { processButton(10); // stop
    return;
  }
  else if ( b==PS2_F8)
  { processButton(17); // step
    return;
  }
  else if (b==PS2_F9)
  { processButton(12); // continue
    return;
  }
  else if (b==PS2_F10)
  { processButton(24); // menu
    return;
  }
  else if (b==PS2_F12)
  { processButton(26); // help
    return;
  }

  if (SerialIO) 
  { if (b==3)
    { SerialIO=false;
      stopNova();
      Serial.println("ctrl-C");
      Serial.print(">");
      return;
    }
    else
    { if (DEV_IS_DONE(INT_TTI)) 
     { if (av) injectSerial(b); // avoid it getting lost
        return;
     }
      
      //if (DEV_IS_BUSY(INT_TTI))
      { ttibuf= b;
        DEV_CLR_BUSY( INT_TTI ) ;
        DEV_SET_DONE( INT_TTI ) ;
        DEV_UPDATE_INTR ;
        return;
      }
    }
    return;
  }

  // ctrl-C
  if (b==3)
  { SerialIO=false;
    stopNova();
    Serial.println("ctrl-C");
    Serial.print(">");
    return;
  }

  // new interactive mode
  else 
  { if (b==27) // read escape key sequences (up and down)
    { int k = Serialread();
      if (k=='[')
      { k = Serialread();
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
      pc=NovaPC&0x7fff;
          
          if (line.startsWith("autostart"))
          { line = nextcmd;
        if(getSessionName(rootID)==".BASICSESSION")
                  injectSerial("\x1b");
          }

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
        NovaPC = readOct(line, pos2+1);
        Serial.println("pc = "+toOct(NovaPC&0x7fff));
        nextcmd = "step";
      }

      // show all registers
      else if (line.startsWith("reg"))
      { for (int a=0; a<4; a++)
          Serial.println("ac"+String(a)+" = "+toOct(examineAC(a)));
        Serial.println("pc = "+toOct(NovaPC&0x7fff));
        Serial.println("carry = "+String(NovaC!=0));
        // Serial.println("intrq = "+String(int_req, OCT));
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
          String t=line.substring(pos2);
	  t.toLowerCase();

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
          { String s = printDisas(b, OCT);
	          s.toLowerCase();
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
        SerialIO=true;
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
        if(SerialIO) 
        { Serial.println("interactive mode");
          SerialIO=false; 
        }  
        nextcmd = "";
      }

      else if (line.startsWith("poweroff"))
      { resetNova();
        stopNova(); 
        Serial.println("power off");
        SerialIO=false; 
        novaKey=1;
        nextcmd = "";
      }

      else if (line.startsWith("stop"))
      { stopNova(); 
        if(SerialIO) 
        { Serial.println("stopped");
          SerialIO=false; 
        }  
        nextcmd = "continue";
      }

      else if (line.startsWith("continue"))
      { continueNova();
        SerialIO=true; 
        nextcmd = "stop";
      }

      else if (line.startsWith("init")) {
        setup();
        delay(100);
        nextcmd = "";
      }

      else if (line.startsWith("version")) {
        Serial.println("Teensy Nova1200 by Marcel van Herk " __DATE__);
        Serial.println();
        Serial.println("Data General Nova - 1st 16 bit minicomputer in 1969");
        Serial.println("This circuit simulates a 1970 DG Nova 1200 machine,");
        Serial.println("equipped with 1 serial board and 4 8 KW core boards.");
        Serial.println("It runs the original 1970 DG BASIC software. You");
        Serial.println("can load and modify different 'sessions' to demo.");
        Serial.println();
        Serial.println("Note: the graphics terminal is an anachronism ;->>>");
        Serial.println();
        Serial.println("Contains parts of simhv3-9");
        Serial.println("Copyright (c) 1993-2008, Robert M. Supnik");
        Serial.println();
        Serial.println("Contains image from FrontPanel 2.0");
        Serial.println("Copyright (c) 2007-2008, John Kichury");
        nextcmd = "";
      }

      else if (line.startsWith("about")) {
        about();
        nextcmd = "";
      }

      // load memory from eeprom
      else if (line.startsWith("load "))
      { int pos1 = 5;
        int pos2 = line.indexOf(' ', 6);
        int pos3 = line.indexOf(' ', pos2+1);
        if (pos2>=0 && pos3>=0)
        { unsigned short a = readOct(line, pos1);
          //unsigned short b = readOct(line, pos2+1);
          //unsigned short c = readOct(line, pos3+1);
          //
          //Serial.println("loaded memory "+toOct(s)+ " with " + c + " blocks");
          nextcmd = "run "+toOct(a);
        }
        else
          nextcmd = "";
        examine(pc);
      }

      // save memory to eeprom
      else if (line.startsWith("save "))
      { //int pos1 = 5;
        //int pos2 = line.indexOf(' ', 6);
        //int pos3 = line.indexOf(' ', pos2+1);
        //if (pos2>=0 && pos3>=0)
        //{ //unsigned short a = readOct(line, pos1);
          //unsigned short b = readOct(line, pos2+1);
          //unsigned short c = readOct(line, pos3+1);
          //unsigned short s=a;
          //
          //Serial.println("saved memory "+toOct(s)+ " total of " + c + " blocks");
        //}
        nextcmd = "";
        examine(pc);
      }

      // restore memory from eeprom
      else if (line.startsWith("restore"))
      { if (hasSD) restoreSession(".SD");
        else restoreSession(".EEPROM");
      }

      // store memory to eeprom
      else if (line.startsWith("store"))
      { storeSession();
#if defined(PICO) || defined(PICOD2) || defined(ARDUINO_LOLIN32)
         EEPROM.commit(); 
#endif
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
        Serial.println("Data="+toOct(novaData)); 
        Serial.println("Addr="+toOct(novaAddress)); 
        Serial.println("Status="+toOct(novaLights));
        nextcmd = "lights";
      }

      // run test function (may not return)
      else if (line.startsWith("test "))
      { //int pos1 = 5;
        //unsigned short a = readOct(line, pos1);
        //tests(a);
      }

      // set line of LCD
      else if (line.startsWith("lcd "))
      { int pos1 = 4;
        int pos2 = line.indexOf(' ', pos1+1);
        if (pos2>=0)
        { unsigned short a = readOct(line, pos1);
          //display(a, line.substring(pos2+1));
          nextcmd = "lcd "+String((a+1)%5)+" "+line.substring(pos2+1);
        }
      }

      // program leds
      else if (line.startsWith("led "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
        //gpio(a & 255);

#if false && defined(PICOD2)
        digitalWrite(LED_RED, a&1);
        digitalWrite(LED_GREEN, (a>>1)&1);
        digitalWrite(LED_BLUE, (a>>2)&1);
#endif

#ifdef PICO
        pinMode(25, OUTPUT);
        digitalWrite(25, (a>>8)&1);
#elif !defined(PICOD2)
        digitalWrite(13, (a>>8)&1);
#endif
        //digitalWrite(12, (a>>9)&1);
        //digitalWrite(11, (a>>10)&1);
        //writeColor(a&15);
        nextcmd = "led "+toOct(a+1);
      }
          
      // program DAC
      else if (line.startsWith("dac "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
#if !defined(FEATHERWING_TFT_TOUCH) && !defined(ARDUINO_LOLIN32)
#ifdef A14
        analogWrite(A14, a);
#endif
#endif
        nextcmd = "dac "+toOct(a+1);
      }

      // read switches bank
      else if (line.startsWith("reads"))
      { Serial.println("Switches="+toOct(novaSwitches)); 
        nextcmd = "reads";
      }

      // read key
      else if (line.startsWith("readk"))
      { //Serial.println("Key="+toOct(readKeys())); 
        nextcmd = "readk";
      }

      // write data bus (for unconnected nova)
      else if (line.startsWith("writed "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        novaData = a;
        nextcmd = "writed "+toOct(a+1);
      }

      // write address bus (for unconnected nova)
      else if (line.startsWith("writea "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        novaAddress = a;
        nextcmd = "writea "+toOct(a+1);
      }

      // write lights (for unconnected nova)
      else if (line.startsWith("writel "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        novaLights = a;
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
        //
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
      { //line.toCharArray(m, 50);
        //unsigned short n = readHex(m+1,1);
        //unsigned short a=readHex(m+2,4);
        //
        nextcmd = "";
      }

      else if (line.startsWith("?"))
      { Serial.print(toHex(novaAddress)); 
        Serial.print(toHex(novaData)); 
        Serial.println(toHex2(novaLights));
        nextcmd = "?";
      }

      // dump all registers in hex
      else if (line.startsWith("$"))
      { for (int a=0; a<4; a++)
          Serial.print(toHex(examineAC(a)));
        Serial.println(toHex(novaAddress));
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
        novaSwitches=a;
      }

      // absolute tape loader using data in eeprom; tape data is stored after 16 bytes $$llFILENAME@; where ll is its 16 bits length
      else if (line.startsWith("tape "))
      { int pos1 = 5;
        String filename = line.substring(pos1, 99);
	filename.toUpperCase(); // +"@";

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
        else if(filename!="LIST")
          Serial.println("Tape "+filename+" not found");
      }

      else if (line.startsWith("unloaddiff "))
      { int pos1=11;
        String filename = line.substring(pos1, 99);
	      filename.toUpperCase();
        unloadDiffCode(filename);
      }

      else if (line.startsWith("unload"))
      { unload((unsigned short *) NULL, 0, 0);
      }

      else if (line.startsWith("cleareepromsession")) // undocumented
      { EEPROM.write(38, 0);
        EEPROM.write(39, 0);
        EEPROM.write(56, 0);
        EEPROM.write(57, 0);
        EEPROM.write(58, 0);
        EEPROM.write(59, 0);
        Serial.println("Erased EEPROM session; will not load anymore");
#if defined(PICO) || defined(PICOD2) || defined(ARDUINO_LOLIN32)
        EEPROM.commit();
#endif
      }

      else if (line.startsWith("session "))
      { int pos1 = 8;
        String filename = line.substring(pos1, 99);
	filename.toUpperCase();
                
        if (filename=="LIST")
        { for (int i=1; i<100; i++)
          { String name=getSessionName(-i);
            int len= getSessionLength(getSessionID(name));
            if (name!="" && len>0)
            Serial.println(name+" ; length "+String(len)+
                           "; id: "+String(getSessionID(name))+
                            "; based on: "+getSessionName(getSessionBase(getSessionID(name))));
          }
        }
        else
        { int id=getSessionID(filename);
          if (id>0) restoreSession(filename);
          else Serial.println("Session "+filename+" not found");
          nextcmd = "run 2";
        }
      }

      else if (line.startsWith("speed "))
      { int pos1 = 6;
        unsigned short a = readOct(line, pos1);
        SIMINTERVAL=a;
        nextcmd="";
      }       

      else if (line.length()>0)
        Serial.println("Unknown or mistyped command ignored");

      Serial.print(">");
      line = "";
    }
  }
}

// process wifi data to pass to Nova or for monitor
#ifdef WIFI
void processWifi(int count)
{ int av = WIFI.available();
  byte b = WIFI.read();
  char m[50];
  int pc;
  int a;

  // power ON if not
  if (novaKey==1)
  { processButton(49);
    return;
  }
  
  if (b==PS2_F1)
  { processButton(49); // on-off
    return;
  }
  else if (b==PS2_F2)
  { processButton(19); // restore
    return;
  }
  else if (b==PS2_F3)
  { processButton(20); // store
    return;
  }
  else if (b==PS2_F4)
  { processButton(23); // speed
    return;
  }
  else if (b==PS2_F5) 
  { about();           // about
    return;
  }
  else if (b==PS2_F6)
  { processButton(25); // cls
    return;
  }
  else if (b==PS2_F7)
  { processButton(10); // stop
    return;
  }
  else if ( b==PS2_F8)
  { processButton(17); // step
    return;
  }
  else if (b==PS2_F9)
  { processButton(12); // continue
    return;
  }
  else if (b==PS2_F10)
  { processButton(24); // menu
    return;
  }
  else if (b==PS2_F12)
  { processButton(26); // help
    return;
  }

  if (SerialIO) 
  { if (b==3)
    { SerialIO=false;
      stopNova();
      WIFI.println("ctrl-C");
      WIFI.print(">");
      return;
    }
    else
    { if (DEV_IS_DONE(INT_TTI)) 
     { if (av) injectSerial(b); // avoid it getting lost
        return;
     }
      
      //if (DEV_IS_BUSY(INT_TTI))
      { ttibuf= b;
        DEV_CLR_BUSY( INT_TTI ) ;
        DEV_SET_DONE( INT_TTI ) ;
        DEV_UPDATE_INTR ;
        return;
      }
    }
    return;
  }

  // ctrl-C
  if (b==3)
  { SerialIO=false;
    stopNova();
    WIFI.println("ctrl-C");
    WIFI.print(">");
    return;
  }

  // new interactive mode
  else 
  { if (b==27) // read escape key sequences (up and down)
    { int k = WIFI.read();
      if (k=='[')
      { k = WIFI.read();
        if (k=='A')
        { nhist--;
          if (nhist<0) nhist=0;
          line = history[nhist];
          WIFI.write("\33[2K\r");
          WIFI.print(">");
          WIFI.print(line);
        }
        else if (k=='B')
        { nhist++;
          if (nhist>=NHISTORY) nhist=NHISTORY-1;
          line = history[nhist];
          WIFI.write("\33[2K\r");
          WIFI.print(">");
          WIFI.print(line);
        }
      }
      else WIFI.write(k);
    }
    else if (b==127 || b==8) // backspace
    { if (line.length()>0) 
      { line.remove(line.length()-1);
        WIFI.write(8);
        WIFI.write(32);
        WIFI.write(8);
      }
    }
    else if (b != '\r' && b != '\n') 
    { line.concat(char(b)); 
      WIFI.write(b);
    }

    // enter
    if (b == '\r') 
    { // auto suggest next command
      if (line=="" && nextcmd!="")
      { line=nextcmd;
        WIFI.write("\33[2K\r");
        WIFI.print(">");
        WIFI.print(line);
        nextcmd="";
        return;
      }
  
      WIFI.println();

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
      pc=NovaPC&0x7fff;
          
      if (line.startsWith("autostart"))
      { line = nextcmd;
        if(getSessionName(rootID)==".BASICSESSION")
        injectSerial("\x1b");
      }

      // show help text
      if (line.startsWith("help"))
      { WIFI.println(helpstring);
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
        { WIFI.print(toOct(a));
          unsigned short v=examine(a++);
          WIFI.print(" ");
          WIFI.print(toOct(v));
          WIFI.println(printDisas(v, OCT));
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
        { WIFI.print(toOct(a));
          unsigned short v=examine(a++);
          WIFI.print(" ");
          WIFI.print(toOct(v));
          WIFI.print(" ");
          WIFI.print(char(max(v>>8,32)));
          WIFI.print(char(max(v&255,32)));
          WIFI.println("");
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
        WIFI.println("ac"+String(a)+" = "+toOct(examineAC(a)));
        if (pos2>=0) nextcmd = "ac"+String((a+1)%4)+" "+toOct(readOct(line, pos2+1));
        else nextcmd = "ac"+String((a+1)%4);
      }

      // read or set pc
      else if (line.startsWith("pc"))
      { int pos2 = line.indexOf(' ', 2);
        if (pos2>=0) examine(readOct(line, pos2+1));
        NovaPC = readOct(line, pos2+1);
        WIFI.println("pc = "+toOct(NovaPC&0x7fff));
        nextcmd = "step";
      }

      // show all registers
      else if (line.startsWith("reg"))
      { for (int a=0; a<4; a++)
          WIFI.println("ac"+String(a)+" = "+toOct(examineAC(a)));
        WIFI.println("pc = "+toOct(NovaPC&0x7fff));
        WIFI.println("carry = "+String(NovaC!=0));
        // WIFI.println("intrq = "+String(int_req, OCT));
      }

      // read or set memory location
      else if (line.startsWith("mem "))
      { int pos1 = 4;
        int pos2 = line.indexOf(' ', 5);
        unsigned short a = readOct(line, pos1);
        if (pos2>=0) deposit(a, readOct(line, pos2+1));
        WIFI.println("mem "+toOct(a)+" = "+toOct(examine(a)));
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
          String t=line.substring(pos2);
          t.toLowerCase();

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
          { String s = printDisas(b, OCT);
            s.toLowerCase();
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
        WIFI.print(toOct(a));
        unsigned short v=examine(a++);
        WIFI.print(" ");
        WIFI.print(toOct(v));
        WIFI.println(printDisas(v, OCT));
        examine(pc);
      }

      // run without serial IO
      else if (line.startsWith("go "))
      { int pos1 = 3;
        unsigned short a = readOct(line, pos1);
        WIFI.println("running");
        SerialIO=true;
        startNova(a);
        nextcmd = "stop";
      }

      // run with serial IO
      else if (line.startsWith("run "))
      { int pos1 = 4;
        SerialIO=true;
        unsigned short a = readOct(line, pos1);
        WIFI.println("running, use ctrl-c to return to command mode");
        startNova(a);
        nextcmd = "stop";
      }

      else if (line.startsWith("reset"))
      { resetNova();
        stopNova(); 
        if(SerialIO) 
        { WIFI.println("interactive mode");
          SerialIO=false; 
        }  
        nextcmd = "";
      }

      else if (line.startsWith("poweroff"))
      { resetNova();
        stopNova(); 
        WIFI.println("power off");
        SerialIO=false; 
        novaKey=1;
        nextcmd = "";
      }

      else if (line.startsWith("stop"))
      { stopNova(); 
        if(SerialIO) 
        { WIFI.println("stopped");
          SerialIO=false; 
        }  
        nextcmd = "continue";
      }

      else if (line.startsWith("continue"))
      { continueNova();
        SerialIO=true; 
        nextcmd = "stop";
      }

      else if (line.startsWith("init")) {
        setup();
        delay(100);
        nextcmd = "";
      }

      else if (line.startsWith("version")) {
        WIFI.println("Teensy Nova1200 by Marcel van Herk " __DATE__);
        WIFI.println();
        WIFI.println("Data General Nova - 1st 16 bit minicomputer in 1969");
        WIFI.println("This circuit simulates a 1970 DG Nova 1200 machine,");
        WIFI.println("equipped with 1 serial board and 4 8 KW core boards.");
        WIFI.println("It runs the original 1970 DG BASIC software. You");
        WIFI.println("can load and modify different 'sessions' to demo.");
        WIFI.println();
        WIFI.println("Note: the graphics terminal is an anachronism ;->>>");
        WIFI.println();
        WIFI.println("Contains parts of simhv3-9");
        WIFI.println("Copyright (c) 1993-2008, Robert M. Supnik");
        WIFI.println();
        WIFI.println("Contains image from FrontPanel 2.0");
        WIFI.println("Copyright (c) 2007-2008, John Kichury");
        nextcmd = "";
      }

      else if (line.startsWith("about")) {
        about();
        nextcmd = "";
      }

      // load memory from eeprom
      else if (line.startsWith("load "))
      { int pos1 = 5;
        int pos2 = line.indexOf(' ', 6);
        int pos3 = line.indexOf(' ', pos2+1);
        if (pos2>=0 && pos3>=0)
        { unsigned short a = readOct(line, pos1);
          //unsigned short b = readOct(line, pos2+1);
          //unsigned short c = readOct(line, pos3+1);
          //
          //Serial.println("loaded memory "+toOct(s)+ " with " + c + " blocks");
          nextcmd = "run "+toOct(a);
        }
        else
          nextcmd = "";
        examine(pc);
      }

      // save memory to eeprom
      else if (line.startsWith("save "))
      { //int pos1 = 5;
        //int pos2 = line.indexOf(' ', 6);
        //int pos3 = line.indexOf(' ', pos2+1);
        //if (pos2>=0 && pos3>=0)
        //{ //unsigned short a = readOct(line, pos1);
          //unsigned short b = readOct(line, pos2+1);
          //unsigned short c = readOct(line, pos3+1);
          //unsigned short s=a;
          //
          //Serial.println("saved memory "+toOct(s)+ " total of " + c + " blocks");
        //}
        nextcmd = "";
        examine(pc);
      }

      // restore memory from eeprom
      else if (line.startsWith("restore"))
      { if (hasSD) restoreSession(".SD");
        else restoreSession(".EEPROM");
      }

      // store memory to eeprom
      else if (line.startsWith("store")) 
      { storeSession();
#if defined(PICO) || defined(PICOD2) || defined(ARDUINO_LOLIN32)
        EEPROM.commit(); 
#endif
     }

      // debug step
      else if (line.startsWith("step"))
      { int pos2 = line.indexOf(' ', 4);
        unsigned short a = pos2>0 ? readOct(line, pos2+1) : 1;
        for (int i=0; i<a; i++) {
          WIFIDebug(1);
          WIFIDebug(2); 
          WIFI.println();
          stepNova();
        }
        nextcmd = "step "+toOct(a);
      }

      // print lights status
      else if (line.startsWith("lights")) {
        WIFI.println("Data="+toOct(novaData)); 
        WIFI.println("Addr="+toOct(novaAddress)); 
        WIFI.println("Status="+toOct(novaLights));
        nextcmd = "lights";
      }

      // run test function (may not return)
      else if (line.startsWith("test "))
      { //int pos1 = 5;
        //unsigned short a = readOct(line, pos1);
        //tests(a);
      }

      // set line of LCD
      else if (line.startsWith("lcd "))
      { int pos1 = 4;
        int pos2 = line.indexOf(' ', pos1+1);
        if (pos2>=0)
        { unsigned short a = readOct(line, pos1);
          //display(a, line.substring(pos2+1));
          nextcmd = "lcd "+String((a+1)%5)+" "+line.substring(pos2+1);
        }
      }

      // program leds
      else if (line.startsWith("led "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
        //gpio(a & 255);

#if false && defined(PICOD2)
        digitalWrite(LED_RED, a&1);
        digitalWrite(LED_GREEN, (a>>1)&1);
        digitalWrite(LED_BLUE, (a>>2)&1);
#endif

#ifdef PICO
        pinMode(25, OUTPUT);
        digitalWrite(25, (a>>8)&1);
#elif !defined(PICOD2)
        digitalWrite(13, (a>>8)&1);
#endif
        //digitalWrite(12, (a>>9)&1);
        //digitalWrite(11, (a>>10)&1);
        //writeColor(a&15);
        nextcmd = "led "+toOct(a+1);
      }
          
      // program DAC
      else if (line.startsWith("dac "))
      { int pos1 = 4;
        unsigned short a = readOct(line, pos1);
#if !defined(FEATHERWING_TFT_TOUCH) && !defined(ARDUINO_LOLIN32)
#ifdef A14
        analogWrite(A14, a);
#endif
#endif
        nextcmd = "dac "+toOct(a+1);
      }

      // read switches bank
      else if (line.startsWith("reads"))
      { WIFI.println("Switches="+toOct(novaSwitches)); 
        nextcmd = "reads";
      }

      // read key
      else if (line.startsWith("readk"))
      { //Serial.println("Key="+toOct(readKeys())); 
        nextcmd = "readk";
      }

      // write data bus (for unconnected nova)
      else if (line.startsWith("writed "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        novaData = a;
        nextcmd = "writed "+toOct(a+1);
      }

      // write address bus (for unconnected nova)
      else if (line.startsWith("writea "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        novaAddress = a;
        nextcmd = "writea "+toOct(a+1);
      }

      // write lights (for unconnected nova)
      else if (line.startsWith("writel "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        novaLights = a;
        nextcmd = "writel "+toOct(a+1);
      }

      // read ADC
      else if (line.startsWith("adc"))
      { 
        int pos1 = 4;
        int pins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
        int pin=readOct(line, pos1);
        pinMode(pins[pin&7], INPUT);
        WIFI.println("adc"+String(pin)+"="+toOct(analogRead(pins[pin&7])));
        nextcmd = "adc";
      }

      // list eeprom block in hex
      else if (line.startsWith("eeprom "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        //
        nextcmd = "eeprom "+toOct(a+1);
      }

      // list memory block in hex
      else if (line.startsWith("memory "))
      { int pos1 = 7;
        unsigned short a = readOct(line, pos1);
        for (short i=0; i<64; i++)
        { String s;
          s = toHex(examine(a+i));
          if ((i&7)==7) WIFI.println(s); else WIFI.print(s);
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
          WIFI.println("cleared memory "+toOct(s)+ " " + toOct(b) + " words to " + toOct(c));
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
      { //line.toCharArray(m, 50);
        //unsigned short n = readHex(m+1,1);
        //unsigned short a=readHex(m+2,4);
        //
        nextcmd = "";
      }

      else if (line.startsWith("?"))
      { WIFI.print(toHex(novaAddress)); 
        WIFI.print(toHex(novaData)); 
        WIFI.println(toHex2(novaLights));
        nextcmd = "?";
      }

      // dump all registers in hex
      else if (line.startsWith("$"))
      { for (int a=0; a<4; a++)
          WIFI.print(toHex(examineAC(a)));
        WIFI.println(toHex(novaAddress));
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
          if (v>=32768) WIFI.print("-");
          else if (w>=(int)strlen(visual)) WIFI.print("+");
          else WIFI.write(visual[w]);
          if (i%80==79) WIFI.println("");
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
             if (v==i) WIFI.print("*");
             else if (i==0) WIFI.print("-");
             else WIFI.print(" ");
           }
          WIFI.println("");
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
        novaSwitches=a;
      }

      // absolute tape loader using data in eeprom; tape data is stored after 16 bytes $$llFILENAME@; where ll is its 16 bits length
      else if (line.startsWith("tape "))
      { int pos1 = 5;
        String filename = line.substring(pos1, 99);
        filename.toUpperCase(); // +"@";

        // absolute tape loader using program data
        if (filename==".BASIC")
        { nextcmd=tapeloader(basictape, sizeof(basictape), ".basic");
          WIFI.print(">");
          line = "";
          return;
        }
        if (filename=="LIST")
        { WIFI.println(".BASIC in code memory; length "+toOct(sizeof(basictape)));
        }
        else if(filename!="LIST")
          WIFI.println("Tape "+filename+" not found");
      }

      /*else if (line.startsWith("unloaddiff "))
      { int pos1=11;
        String filename = line.substring(pos1, 99);
        filename.toUpperCase();
        unloadDiffCode(filename);
      }

      else if (line.startsWith("unload"))
      { unload((unsigned short *) NULL, 0, 0);
      }
      */
      
      else if (line.startsWith("cleareepromsession")) // undocumented
      { EEPROM.write(38, 0);
        EEPROM.write(39, 0);
        EEPROM.write(56, 0);
        EEPROM.write(57, 0);
        EEPROM.write(58, 0);
        EEPROM.write(59, 0);
        WIFI.println("Erased EEPROM session; will not load anymore");
      }

      else if (line.startsWith("session "))
      { int pos1 = 8;
        String filename = line.substring(pos1, 99);
        filename.toUpperCase();
                
        if (filename=="LIST")
        { for (int i=1; i<100; i++)
          { String name=getSessionName(-i);
            int len= getSessionLength(getSessionID(name));
            if (name!="" && len>0)
            WIFI.println(name+" ; length "+String(len)+
                           "; id: "+String(getSessionID(name))+
                            "; based on: "+getSessionName(getSessionBase(getSessionID(name))));
          }
        }
        else
        { int id=getSessionID(filename);
          if (id>0) restoreSession(filename);
          else WIFI.println("Session "+filename+" not found");
          nextcmd = "run 2";
        }
      }

      else if (line.startsWith("speed "))
      { int pos1 = 6;
        unsigned short a = readOct(line, pos1);
        SIMINTERVAL=a;
        nextcmd="";
      }       

      else if (line.length()>0)
        WIFI.println("Unknown or mistyped command ignored");

      WIFI.print(">");
      line = "";
    }
  }
}
#endif

void setup() {

#ifdef TFT_RESET
pinMode(TFT_RESET, OUTPUT);
digitalWrite(TFT_RESET, HIGH);
#endif

#if false && defined(PICOD2)
   pinMode(LED_RED, OUTPUT);
   pinMode(LED_GREEN, OUTPUT);
   pinMode(LED_BLUE, OUTPUT);
#endif

// on the long teensy boards, the TFT must overlap pins 33-39 (2 pin stick over), use pin 32 for LED, 36 for reset
// all other pins are wired to the orginals, so they are set to input for safety

#if (defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41)) && !defined(LAZY_BREADBOARDTFT)
                        // tft 9 wired to MISO (pin 12)
  pinMode(32, OUTPUT);
  digitalWrite(32, 1);  // tft 8 through 470 ohm resistor (LED)
  pinMode(33, INPUT);   // tft 7 wired to SCK (pin 13)
  pinMode(34, INPUT);   // tft 6 wired to MOSI (pin 12)
  pinMode(35, INPUT);   // tft 5 wired to DC (pin 9)
  pinMode(36, OUTPUT);  // 
  digitalWrite(36, 1);  // tft 4 (RESET)
  pinMode(37, INPUT);   // tft 3 wired to CS (pin 10)
  pinMode(38, INPUT);   // tft 2 wired to GND
  pinMode(39, INPUT);   // tft 1 wired to VCC
  delay(100);
#endif

#ifdef TFT_GND
  pinMode(TFT_GND, OUTPUT);
  digitalWrite(TFT_GND, 0);
#endif

#ifdef TFT_VCC
  pinMode(TFT_VCC, OUTPUT);
  digitalWrite(TFT_VCC, 1);
#endif

#ifdef TFT_RESET
  pinMode(TFT_RESET, OUTPUT);
  digitalWrite(TFT_RESET, 1);
#endif

#ifdef TFT_DC
  pinMode(TFT_DC, INPUT);
#endif

#ifdef TFT_MOSI
  pinMode(TFT_MOSI, INPUT);
  pinMode(TFT_SCK, INPUT);
  pinMode(TFT_MISO, INPUT);
#endif

#ifdef TFT_LED
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, 1);
#endif

#ifdef PS2_VCC
  pinMode(PS2_VCC, OUTPUT);
  digitalWrite(PS2_VCC, 1); // kb ON
  if (PS2_VCC>24)
  { pinMode(PS2_VCC-1, OUTPUT);
    digitalWrite(PS2_VCC-1, 1); // kb ON
  }
#endif

#ifdef FEATHERWING_TFT_TOUCH
  pinMode(ONOFFSLIDER, INPUT_PULLUP); // switch
#endif

#if defined(ARDUINO_LOLIN32)
  EEPROM.begin(2048); // Note: somehow initialising EEPROM 1st time takes forever; check GUI eeprom size setting
#endif

#if defined(PICO) || defined(PICOD2)
  EEPROM.begin(4096);
#endif

#ifdef FEATHERWING_TFT_TOUCH
  if (!touch.begin())
    Serial.println("No touch screen found");

  // power button ps2 keyboard interface board
  pinMode(PWRBUTTON, INPUT_PULLUP); // switch
#endif

#ifdef SD_CS
  if (!card.init(SPI_HALF_SPEED, SD_CS)) 
  { Serial.println("No SD card found");
  }
  else
  { if (!volume.init(card)) 
    { Serial.println("No filesystem on SD card found");
    }
    else
    { //root.openRoot(volume);
      //root.ls(LS_R | LS_DATE | LS_SIZE);
      hasSD = true;
      Serial.println("Using file tnses1.bin on SD card for session info");
      SD.begin(SD_CS);
      myFile = SD.open("tnses1.bin");
      if (myFile) Serial.println("File size on SD card: "+String(myFile.size()));
      myFile.close();
      eepromwords = 10000; // SD card session size limited by RAM for buffers only
    }
  }
#endif

  delay(100);
#ifdef PICO
  tft.begin(25000000);
  tft.lockTransaction(true);
#elif defined(PICOD2)
  tft.init(240, 320);
#else
  tft.begin();
#endif

#ifndef USEADAFRUIT
  tft.setClock(75000000);
  tft.setRotation(1);
#else
  tft.setRotation(3);
#endif
  tft.fillScreen(ILI9341_BLACK);

  Serial.begin(115200);
#ifdef WIFI
  WIFI.begin(115200);
#endif
  
  Serial.println("Teensy Nova 1210 simulator!"); 

#if !defined(FEATHERWING_TFT_TOUCH) && !defined(ARDUINO_LOLIN32) && !defined(PICOD2)
  pinMode(14, OUTPUT); // red/green led, pin controlled analog
  pinMode(15, OUTPUT); // red/green led, pin controlled digital to support TEENSY35
#endif
    
  //Draw Nova frontpanel
  updateImage();
  tft.setCursor(0,0);
  
#if defined(PS2_DATA)
  ps2kb.begin(PS2_DATA, PS2_CLK);
#endif
}

int runtime=0;
int instructions=0;
int prevSIMINTERVAL=0;

void loop(void) {
  int count, a;
  int t=micros();

  // update MIPS when changing speed
  if (!novaRunning || prevSIMINTERVAL!=SIMINTERVAL || runtime>10000000)
  { runtime=0;
    instructions=0;
    prevSIMINTERVAL=SIMINTERVAL;
  }

  // Injected or true serial data ?
  if ((count=injectSerialText.length()) &&  millis()-injectSerialDelay>20)
  { //pinMode(25, OUTPUT);
    //digitalWrite(25, 1);
    processSerial(count);
    //digitalWrite(25, 0);
    injectSerialDelay=millis();
    //if (SerialIO) return;
  }
  else if ((count=Serial.available()))
  { processSerial(count);
    //if (SerialIO) return;
  }
#ifdef WIFI
  else if ((count=WIFI.available()))
  { hasWIFI = true;
    processWifi(count);
    if (SerialIO) return;
  }
#endif

  // Button presses?
  a=getButtonPress(false);
  processButton(a); // no button (0) deals with key up repaint
  if (a>=80 && helpScreen<0)
  { tempHelp=millis();
    showHelp();
  }
  
#if defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY41) || defined(FEATHERWING_TFT_TOUCH)
  if(ps2kb.available())
  { char c = ps2kb.read();
    String s=""; s.concat(c);
    injectSerial(s);
  }
#endif

  // Erase temporary help screen
  if (tempHelp && millis()-tempHelp>(novaRunning?1000:5000))
  { restoreText(true);
    tempHelp=0;
  }

  // run simulator
  if(novaRunning)
  { simulateNova(SIMINTERVAL);
    instructions += SIMINTERVAL;
    if (!novaRunning) updateImage();
    //Serial.println("running: "+toOct(novaRunning));
  }

#ifdef PWRBUTTON
  static bool prev15=true;
  bool val15 = digitalRead(PWRBUTTON);
  if (val15!=prev15) 
  { delay(50);
    val15 = digitalRead(PWRBUTTON);
    if (val15!=prev15) 
    { prev15=val15;
      if(val15==false) 
	  { processButton(49);
        a=49;
	  }
	}
  }
#endif

#ifdef FEATHERWING_TFT_TOUCH
  digitalWrite(TFT_LED, digitalRead(ONOFFSLIDER));
#endif

  // something to do -> update screen
  if ((a || novaRunning || count))
    updateImage();

  // for MIPS calculation
  if(novaRunning)
    runtime += micros()-t;

  // update MIPS display (note screen refresh and serial output is counted in time)
  if (runtime && instructions>100 && novaKey>1)
  { tft.setTextColor(CL(255,255,0), CL(0,0,0));
    tft.setCursor(276, 178);
    if (novaRunning) { tft.print(((float)instructions/runtime)); tft.print(" "); }
    else tft.print("     ");
  }
  
  // show disassembled code as well
  if ((helpScreen>=0 || steppinghelp>0 || SIMINTERVAL==1) && novaKey>1)
  { tft.setTextColor(CL(128,255,0), CL(40,75,125));
    tft.setCursor(100, 229);
    int s=novaData;
    String d=printDisas(examine(novaAddress), OCT);
    tft.print(toOct(novaAddress)+" "+d+makespc(20-d.length()));
    novaData=s;
  }
}
