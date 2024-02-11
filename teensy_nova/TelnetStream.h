// From LennartHennigs ESPTelnet & ESPTelnetStream
// mvh 20240210: added all stream output functions; protect against closed state
// mvh 20240211: amalgated into one file; set base and digits default to 10

/////////////////////////////////////////////////////////////////

#pragma once
#ifndef TelnetStream_h
#define TelnetStream_h

#include <Arduino.h>
#ifndef CORE_TEENSY

/////////////////////////////////////////////////////////////////

#define ASCII_BELL 7
#define ASCII_BACKSPACE 8
#define ASCII_LF 10
#define ASCII_CR 13
#define KEEP_ALIVE_INTERVAL_MS 1000

/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <WiFi.h>

/////////////////////////////////////////////////////////////////

using TCPClient = WiFiClient;
using TCPServer = WiFiServer;

/////////////////////////////////////////////////////////////////
class ESPTelnetBase {
  typedef void (*CallbackFunction)(String str);

 public:
  ESPTelnetBase();

  bool begin(uint16_t port = 23, bool checkConnection = true);
  void stop();
  void loop();

  bool isConnected();
  void disconnectClient(bool triggerEvent = true);

  void setKeepAliveInterval(int ms);
  int getKeepAliveInterval();

  String getIP() const;
  String getLastAttemptIP() const;

  void onConnect(CallbackFunction f);
  void onConnectionAttempt(CallbackFunction f);
  void onReconnect(CallbackFunction f);
  void onDisconnect(CallbackFunction f);
  void onInputReceived(CallbackFunction f);

 protected:
  TCPServer server = TCPServer(23);  // must be initalized here
  TCPClient client;
  bool connected = false;  // needed because I cannot do "client = NULL"
  String ip = "";
  String attemptIp;
  String input = "";

  uint16_t server_port = 23;
  int keep_alive_interval = KEEP_ALIVE_INTERVAL_MS;
  long last_status_check;

  CallbackFunction on_connect = NULL;
  CallbackFunction on_reconnect = NULL;
  CallbackFunction on_disconnect = NULL;
  CallbackFunction on_connection_attempt = NULL;
  CallbackFunction on_input = NULL;

  void emptyClientStream();
  bool _isIPSet(IPAddress ip);
  virtual void handleInput() = 0;

 private:
  void connectClient(TCPClient c, bool triggerEvent = true);
  void processClientConnection();
  void performKeepAliveCheck();
  void handleClientInput();
  void notifyConnectionAttempt(const String &attemptedIp);
  void handleReconnection(TCPClient &newClient, const String &attemptedIp); 
  void handleExistingConnection(TCPClient &newClient);
  
  bool doKeepAliveCheckNow();

};

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
  size_t print(unsigned char b, int base=10);
  size_t print(int n, int base=10);
  size_t print(unsigned int n, int base=10);
  size_t print(long n, int base=10);
  size_t print(unsigned long n, int base=10);
  size_t print(double n, int digits=10);
  size_t println(const __FlashStringHelper *ifsh);
  size_t print(const Printable& x);
  size_t println(void);
  size_t println(const String &s);
  size_t println(const char c[]);
  size_t println(char c);
  size_t println(unsigned char b, int base=10);
  size_t println(int num, int base=10);
  size_t println(unsigned int num, int base=10);
  size_t println(long num, int base=10);
  size_t println(unsigned long num, int base=10);
  size_t println(double num, int digits=10);
  size_t println(const Printable& x);
  
 protected:
  void handleInput();
};

/////////////////////////////////////////////////////////////////

// << operator
//  template<class T> inline ESPTelnetStream &operator <<(ESPTelnetStream &obj, T arg) { obj.print(arg); return obj; }

/////////////////////////////////////////////////////////////////

#endif
#endif
/////////////////////////////////////////////////////////////////
