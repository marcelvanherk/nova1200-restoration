#include "MergedStreams.h"

// mvh 20240208 made s optional and added optional t; added constructors and access function
// e.g. MS = MergedStreams(Serial); // passing primary stream is required
// MS.setSecondary(Serial1); // enable second
// MS.setSecondary(NULL);	 // disable second
// MS.setTertiary(Telnet);	 // enable third
// MS.setTertiary(NULL);     // disable third

MergedStreams::MergedStreams(Stream &_p)
{
  this->p = &_p;
  this->s = NULL;
  this->t = NULL;
}

MergedStreams::MergedStreams(Stream &_p, Stream &_s)
{
  this->p = &_p;
  this->s = &_s;
  this->t = NULL;
}

MergedStreams::MergedStreams(Stream &_p, Stream &_s, Stream &_t)
{
  this->p = &_p;
  this->s = &_s;
  this->t = &_t;
}

void MergedStreams::setPrimary(Stream &_p)
{
  this->p = &_p;
}

void MergedStreams::setSecondary(Stream &_s)
{
  this->s = &_s;
}

void MergedStreams::setTertiary(Stream &_t)
{
  this->t = &_t;
}

int MergedStreams::available()
{ int r;
  r = p->available();
  if (s) r = r || s->available();
  if (t) r = r || t->available();
  return r;
}

int MergedStreams::peek()
{ int r;
  r = p->peek();
  if (s) r = r || s->peek();
  if (t) r = r || t->peek();
  return r;
}

void MergedStreams::flush()
{
  p->flush();
  if (s) s->flush();
  if (t) t->flush();
}

int MergedStreams::read()
{ if (t)
  { if(p->available()){
      return p->read();
    } else if (s && s->available()){
       return s->read();
      }  else { 
	      return t->read();
    }
  }
  else if (s)
  { if(p->available()){
      return p->read();
    }else{
      return s->read();
    }
  }
  else
	return p->read();
}

size_t MergedStreams::write(uint8_t c)
{
  size_t rtn = p->write(c);
  if(s) s->write(c);
  if(t) t->write(c);
  return rtn;
}

size_t MergedStreams::write(const char *str)
{
  if (str == NULL) return 0;
  size_t rtn = p->write((const uint8_t *)str, strlen(str));
  if (s) s->write((const uint8_t *)str, strlen(str));
  if (t) t->write((const uint8_t *)str, strlen(str));
  return rtn;
}

size_t MergedStreams::write(const char *buffer, size_t size)
{
  size_t rtn = p->write((const uint8_t *)buffer, size);
  if (s) s->write((const uint8_t *)buffer, size);
  if (t) t->write((const uint8_t *)buffer, size);
  return rtn;
}

size_t MergedStreams::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--) {
    n += p->write(*buffer);
    if (s) s->write(*buffer);
    if (t) t->write(*buffer);
    *buffer++;
  }
  return n;
}

size_t MergedStreams::print(const __FlashStringHelper *ifsh)
{
  size_t rtn = p->print(ifsh);
  if (s) s->print(ifsh);
  if (t) t->print(ifsh);
  return rtn;
}

size_t MergedStreams::print(const String &string)
{
  size_t rtn = p->print(string);
  if (s) s->print(string);
  if (t) t->print(string);
  return rtn;
}

size_t MergedStreams::print(const char str[])
{
  size_t rtn = p->print(str);
  if (s) s->print(str);
  if (t) t->print(str);
  return rtn;
}

size_t MergedStreams::print(char c)
{
  size_t rtn = p->print(c);
  if (s) s->print(c);
  if (t) t->print(c);
  return rtn;
}

size_t MergedStreams::print(unsigned char b, int base)
{
  size_t rtn = p->print(b, base);
  if (s) s->print(b, base);
  if (t) t->print(b, base);
  return rtn;
}

size_t MergedStreams::print(int n, int base)
{
  size_t rtn = p->print(n, base);
  if (s) s->print(n, base);
  if (t) t->print(n, base);
  return rtn;
}

size_t MergedStreams::print(unsigned int n, int base)
{
  size_t rtn = p->print(n, base);
  if (s) s->print(n, base);
  if (t) t->print(n, base);
  return rtn;
}

size_t MergedStreams::print(long n, int base)
{
  size_t rtn = p->print(n, base);
  if (s) s->print(n, base);
  if (t) t->print(n, base);
  return rtn;
}

size_t MergedStreams::print(unsigned long n, int base)
{
  size_t rtn = p->print(n, base);
  if (s) s->print(n, base);
  if (t) t->print(n, base);
  return rtn;
}

size_t MergedStreams::print(double n, int digits)
{
  size_t rtn = p->print(n, digits);
  if (s) s->print(n, digits);
  if (t) t->print(n, digits);
  return rtn;
}

size_t MergedStreams::println(const __FlashStringHelper *ifsh)
{
  size_t rtn = p->println(ifsh);
  if (s) s->println(ifsh);
  if (t) t->println(ifsh);
  return rtn;
}

size_t MergedStreams::print(const Printable& x)
{
  size_t rtn = p->print(x);
  if (s) s->print(x);
  if (t) t->print(x);
  return rtn;
}

size_t MergedStreams::println(void)
{
  size_t rtn = p->println();
  if (s) s->println();
  if (t) t->println();
  return rtn;
}

size_t MergedStreams::println(const String &string)
{
  size_t rtn = p->println(string);
  if (s) s->println(string);
  if (t) t->println(string);
  return rtn;
}

size_t MergedStreams::println(const char c[])
{
  size_t rtn = p->println(c);
  if (s) s->println(c);
  if (t) t->println(c);
  return rtn;
}

size_t MergedStreams::println(char c)
{
  size_t rtn = p->println(c);
  if (s) s->println(c);
  if (t) t->println(c);
  return rtn;
}

size_t MergedStreams::println(unsigned char b, int base)
{
  size_t rtn = p->println(b, base);
  if (s) s->println(b, base);
  if (t) t->println(b, base);
  return rtn;
}

size_t MergedStreams::println(int num, int base)
{
  size_t rtn = p->println(num, base);
  if (s) s->println(num, base);
  if (t) t->println(num, base);
  return rtn;
}

size_t MergedStreams::println(unsigned int num, int base)
{
  size_t rtn = p->println(num, base);
  if (s) s->println(num, base);
  if (t) t->println(num, base);
  return rtn;
}

size_t MergedStreams::println(long num, int base)
{
  size_t rtn = p->println(num, base);
  if (s) s->println(num, base);
  if (t) t->println(num, base);
  return rtn;
}

size_t MergedStreams::println(unsigned long num, int base)
{
  size_t rtn = p->println(num, base);
  if (s) s->println(num, base);
  if (t) t->println(num, base);
  return rtn;
}

size_t MergedStreams::println(double num, int digits)
{
  size_t rtn = p->println(num, digits);
  if (s) s->println(num, digits);
  if (t) t->println(num, digits);
  return rtn;
}

size_t MergedStreams::println(const Printable& x)
{
  size_t rtn = p->println(x);
  if (s) s->println(x);
  if (t) t->println(x);
  return rtn;
}
