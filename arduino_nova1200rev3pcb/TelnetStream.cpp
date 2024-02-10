// From ESPTelnetStream
// mvh 20240210: added all stream output functions; protect against closed state

/////////////////////////////////////////////////////////////////

#include "TelnetStream.h"

/////////////////////////////////////////////////////////////////

void TelnetStream::handleInput() {
  if (client && isConnected()) {
    char c = client.read();
    on_input(String(c));
  }
}

/////////////////////////////////////////////////////////////////

int TelnetStream::available() {
  if (client && isConnected()) {
    return client.available();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

int TelnetStream::read() {
  if (client && isConnected()) {
    return client.read();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

int TelnetStream::peek() {
  if (client && isConnected()) {
    return client.peek();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

void TelnetStream::flush() {
  if (client && isConnected()) {
    client.flush();
  }
}

/////////////////////////////////////////////////////////////////

size_t TelnetStream::write(uint8_t data) {
  if (client && isConnected()) {
    return client.write(data);
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

size_t TelnetStream::write(const char *buffer, size_t size) {
  size_t n = 0;
  if (client && isConnected()) {
    while (size--) {
      n += client.write(*buffer);
      *buffer++;
    }
  }
  return n;  	
}

size_t TelnetStream::write(const uint8_t *buffer, size_t size) {
  size_t n = 0;
  if (client && isConnected()) {
    while (size--) {
      n += client.write(*buffer);
      *buffer++;
    }
  }
  return n;  	
}

size_t TelnetStream::write(const char *str) {
  write(str, strlen(str));
}

size_t TelnetStream::print(const __FlashStringHelper *ifsh) {
  return 0;
}
    
size_t TelnetStream::print(const String &s) {
  return write(s.c_str());
}
    
size_t TelnetStream::print(const char str[]) {
  return write(str, strlen(str));
}

size_t TelnetStream::print(char c) {
  return write(c);
}

size_t TelnetStream::print(unsigned char b, int base) {
  return print(String(b, base));
}

size_t TelnetStream::print(int n, int base) {
  return print(String(n, base));
}

size_t TelnetStream::print(unsigned int n, int base) {
  return print(String(n, base));
}

size_t TelnetStream::print(long n, int base) {
  return print(String(n, base));
}

size_t TelnetStream::print(unsigned long n, int base) {
  return print(String(n, base));
}

size_t TelnetStream::print(double n, int digits) {
  return print(String(n, digits));
}

size_t TelnetStream::println(const __FlashStringHelper *ifsh) {
  return 0;
}

size_t TelnetStream::print(const Printable& x) {
  return 0; // print(String(x));
}

size_t TelnetStream::println(void) {
  write('\n');
  return 1;
}

size_t TelnetStream::println(const String &s) {
  size_t r = print(s);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(const char c[]) {
  size_t r = print(c);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(char c) {
  size_t r = print(c);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(unsigned char b, int base) {
  size_t r = print(b, base);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(int num, int base) {
  size_t r = print(num, base);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(unsigned int num, int base) {
  size_t r = print(num, base);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(long num, int base) {
  size_t r = print(num, base);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(unsigned long num, int base) {
  size_t r = print(num, base);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(double num, int digits) {
  size_t r = print(num, digits);
  write('\n');
  return r+1;
}

size_t TelnetStream::println(const Printable& x) {
  size_t r = print(x);
  write('\n');
  return r+1;
}
