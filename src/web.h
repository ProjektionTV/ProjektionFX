#if !defined(_PFX_WEB_H__)
#define _PFX_WEB_H__

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include <html_ui.h>

AsyncWebServer server(80);

void setupWebServer()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                  

                   AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_index, PAGE_index_L);

    response->addHeader(F("Content-Encoding"),"gzip");
    request->send(response); });

    server.on("/reset-wifi", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        wifiManager.resetSettings();
        ESP.restart(); });

    server.begin();
}

#endif // _PFX_WEB_H__
