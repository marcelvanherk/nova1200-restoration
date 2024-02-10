// From ESPTelnetStream
// mvh 20240210: added all stream output functions; protect against closed state

/////////////////////////////////////////////////////////////////

#pragma once
#ifndef TelnetStream_h
#define TelnetStream_h

/////////////////////////////////////////////////////////////////

#include "ESPTelnetBase.h"

/////////////////////////////////////////////////////////////////

class TelnetStream : public ESPTelnetBase, public Stream {
 public:
  using ESPTelnetBase::ESPTelnetBase;

  int available();
  int read();
  int peek();
  void flush();

  size_t write(uint8_t);
    size_t write(const char *str);
    size_t write(const char *buffer, size_t size);
    size_t write(const uint8_t *buffer, size_t size);

    size_t print(const __FlashStringHelper *ifsh);
    size_t print(const String &s);
    size_t print(const char str[]);
    size_t print(char c);
    size_t print(unsigned char b, int base);
    size_t print(int n, int base);
    size_t print(unsigned int n, int base);
    size_t print(long n, int base);
    size_t print(unsigned long n, int base);
    size_t print(double n, int digits);
    size_t println(const __FlashStringHelper *ifsh);
    size_t print(const Printable& x);
    size_t println(void);
    size_t println(const String &s);
    size_t println(const char c[]);
    size_t println(char c);
    size_t println(unsigned char b, int base);
    size_t println(int num, int base);
    size_t println(unsigned int num, int base);
    size_t println(long num, int base);
    size_t println(unsigned long num, int base);
    size_t println(double num, int digits);
    size_t println(const Printable& x);
    
 protected:
  void handleInput();
};

/////////////////////////////////////////////////////////////////

// << operator
//  template<class T> inline ESPTelnetStream &operator <<(ESPTelnetStream &obj, T arg) { obj.print(arg); return obj; }

/////////////////////////////////////////////////////////////////

#endif
/////////////////////////////////////////////////////////////////
