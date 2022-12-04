#ifndef __WEBSERVER__
#define __WEBSERVER__

#ifdef ARDUINO_ARCH_ESP8266
#include <HttpsWebServerESP8266.h>
#elif ARDUINO_ARCH_ESP32
#include <HttpsWebServerESP32.h>
#endif

class HttpsWebServer{
  private:
#ifdef ARDUINO_ARCH_ESP8266
  HttpsWebServerESP8266 https_internal;
#elif ARDUINO_ARCH_ESP32
  HttpsWebServerESP32 https_internal;
#endif

  public:
    void setupDNS();
    void setSSLCert();
    void registerDelayRoute();
    void registerDefaultRoute();
    boolean start();
    void loop();
};

#endif
