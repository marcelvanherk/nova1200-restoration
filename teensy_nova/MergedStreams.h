// mvh 20240208 made s optional and added optional t; added constructors and access function
// e.g. MS = MergedStreams(Serial); // passing primary stream is required
// MS.setSecondary(Serial1); // enable second
// MS.setSecondary(NULL);	 // disable second
// MS.setTertiary(Telnet);	 // enable third
// MS.setTertiary(NULL);     // disable third


#ifndef MergedStreams_h
#define MergedStreams_h

#include "Arduino.h"

class MergedStreams : public Stream {
  public:
    MergedStreams(Stream &_p);
    MergedStreams(Stream &_p, Stream &_s);
    MergedStreams(Stream &_p, Stream &_s, Stream &_t);

    void setPrimary(Stream &_p);
    void setSecondary(Stream &_s);
    void setTertiary(Stream &_t);

    int available();
    int peek();
    void flush();

    int read();

    size_t write(uint8_t c);
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

  private:
    Stream* p;
    Stream* s = NULL;
    Stream* t = NULL;
};
#endif // MergedStreams_h