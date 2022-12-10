#include <https_web_server.h>

void HttpsWebServer::setupDNS(){
  https_internal.setupDNS();
}

void HttpsWebServer::setSSLCert(){
  https_internal.setSSLCert();
}

void HttpsWebServer::registerDelayRoute(){
  https_internal.registerDelayRoute();
}

void HttpsWebServer::registerDefaultRoute(){
  https_internal.registerDefaultRoute();
}

boolean  HttpsWebServer::start(){
  return https_internal.start();
}

void HttpsWebServer::loop(){
  https_internal.loop();
}
