#ifndef __WEBSERVER__
#define __WEBSERVER__

#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using httpsserver::SSLCert;
using httpsserver::SSLKeySize;
using httpsserver::HTTPSServer;
using httpsserver::ResourceNode;
using httpsserver::ResourceParameters;
using httpsserver::HTTPRequest;
using httpsserver::HTTPResponse;

class HttpsWebServer{
  private:
    SSLCert * cert;
    HTTPSServer * secureServer;
  public:
    void generateSSLCert();
    void registerDelayRoute();
    void registerDefaultRoute();
    boolean start();
    void loop();
};

#endif
