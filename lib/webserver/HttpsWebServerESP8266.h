#ifndef __WEBSERVER_ESP8266__
#define __WEBSERVER_ESP8266__

#ifdef ARDUINO_ARCH_ESP8266

#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>

using BearSSL::ESP8266WebServerSecure;

class HttpsWebServerESP8266{
  private:
    ESP8266WebServerSecure secureServer;

  public:
    HttpsWebServerESP8266();
    void setupDNS();
    void setSSLCert();
    void registerDelayRoute();
    void registerDefaultRoute();
    boolean start();
    void loop();
};

#endif
#endif
