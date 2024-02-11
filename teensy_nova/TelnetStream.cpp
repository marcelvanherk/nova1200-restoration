// From LennartHennigs ESPTelnet & ESPTelnetStream
// mvh 20240210: added all stream output functions; protect against closed state
// mvh 20240211: amalgated into one file; fix for PICOW

/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#ifndef CORE_TEENSY

#include "TelnetStream.h"

ESPTelnetBase::ESPTelnetBase() {
  connected = false;
}

/////////////////////////////////////////////////////////////////

bool ESPTelnetBase::begin(uint16_t port /* = 23 */, bool checkConnection /* = true */) {
  ip = "";
  if (checkConnection) {
    // connected to WiFi or is ESP in AP mode?
    if (WiFi.status() != WL_CONNECTED && !_isIPSet(WiFi.softAPIP())) return false;
  }
  server_port = port;
  server = TCPServer(port);
  server.begin();
  server.setNoDelay(true);
  return true;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::loop() {
  processClientConnection();
  performKeepAliveCheck();
  handleClientInput();
  yield();
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::processClientConnection() {
  if (!server.hasClient()) return;
  TCPClient newClient = server.accept();
  if (!connected) {
    connectClient(newClient);
  } else {
    handleExistingConnection(newClient);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::handleExistingConnection(TCPClient &newClient) {
  String attemptedIp = newClient.remoteIP().toString();

  if (!isConnected()) {
    disconnectClient();
    return;
  }

  if (attemptedIp == ip) {
    handleReconnection(newClient, attemptedIp);
  } else {
    notifyConnectionAttempt(attemptedIp);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::handleReconnection(TCPClient &newClient, const String &attemptedIp) {
  disconnectClient(false);
  connectClient(newClient, false);
  if (on_reconnect) on_reconnect(attemptedIp);
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::notifyConnectionAttempt(const String &attemptedIp) {
  if (on_connection_attempt) on_connection_attempt(attemptedIp);
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::performKeepAliveCheck() {
  if (doKeepAliveCheckNow() && connected && !isConnected()) {
    disconnectClient();
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::handleClientInput() {
  if (on_input && client && client.available()) {
    handleInput();
  }
}


/////////////////////////////////////////////////////////////////

bool ESPTelnetBase::doKeepAliveCheckNow() {
  unsigned long currentTime = millis();
  unsigned long timeElapsed = currentTime - last_status_check;

  if (timeElapsed >= static_cast<unsigned long>(keep_alive_interval)) {
    last_status_check = currentTime;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::setKeepAliveInterval(int ms) {
  keep_alive_interval = ms;
}

/////////////////////////////////////////////////////////////////

int ESPTelnetBase::getKeepAliveInterval() {
  return keep_alive_interval;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::connectClient(WiFiClient c, bool triggerEvent) {
  client = c;
  ip = client.remoteIP().toString();
  client.setNoDelay(true);
  if (triggerEvent && on_connect != NULL) on_connect(ip);
  emptyClientStream();
  connected = true;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::disconnectClient(bool triggerEvent) {
  emptyClientStream();
  client.stop();
  if (triggerEvent && on_disconnect != NULL) on_disconnect(ip);
  ip = "";
  connected = false;
}

/////////////////////////////////////////////////////////////////
// helper method, as ESP32's IPAddress has no isSet() method

bool ESPTelnetBase::_isIPSet(IPAddress ip) {
#if defined(ARDUINO_ARCH_ESP8266)
  return ip.isSet();
#else
  // this works for ESP32, hopefully for others as well
  return ip.toString() != "0.0.0.0";
#endif
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::stop() {
  server.stop();
}

/////////////////////////////////////////////////////////////////

bool ESPTelnetBase::isConnected() {
  bool res;  
#if defined(ARDUINO_ARCH_ESP8266)
  res = client.status() == ESTABLISHED;
#else // defined(ARDUINO_ARCH_ESP32) // for PICOW
  res = client.connected();
#endif
  return res;
}

/////////////////////////////////////////////////////////////////

String ESPTelnetBase::getIP() const {
  return ip;
}

/////////////////////////////////////////////////////////////////

String ESPTelnetBase::getLastAttemptIP() const {
  return attemptIp;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::emptyClientStream() {
  client.flush();
  delay(50);
  while (client.available()) {
    client.read();
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onConnect(CallbackFunction f) {
  on_connect = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onConnectionAttempt(CallbackFunction f) {
  on_connection_attempt = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onReconnect(CallbackFunction f) {
  on_reconnect = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onDisconnect(CallbackFunction f) {
  on_disconnect = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onInputReceived(CallbackFunction f) {
  on_input = f;
}

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
  return write(str, strlen(str));
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

#endif
