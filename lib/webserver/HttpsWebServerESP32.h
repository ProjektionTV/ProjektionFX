#ifndef __WEBSERVER_ESP32__
#define __WEBSERVER_ESP32__

#ifdef ARDUINO_ARCH_ESP32

#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using httpsserver::SSLCert;
using httpsserver::HTTPSServer;

class HttpsWebServerESP32{
  private:
    SSLCert * cert;
    HTTPSServer * secureServer;

  public:
    void setupDNS();
    void setSSLCert();
    void registerDelayRoute();
    void registerDefaultRoute();
    boolean start();
    void loop();
};

#endif
#endif
