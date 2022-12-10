#ifdef ARDUINO_ARCH_ESP8266

#include <HttpsWebServerESP8266.h>
#include <ESP8266mDNS.h>
#include <HttpsCert.h>
#include <StreamInfos.h>

#include <sstream>
#include <cstdint>

HttpsWebServerESP8266::HttpsWebServerESP8266() : secureServer(443){
}

void HttpsWebServerESP8266::setupDNS(){
  if (!MDNS.begin("projektionfx")) {
    Serial.println("Error setting up mDNS responder!");
    while(1) {
        delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  Serial.println("Access ProjektionFX at: https://projektionfx.local");
}

void HttpsWebServerESP8266::setSSLCert(){
  secureServer.getServer().setRSACert(
    new BearSSL::X509List(projektionfxLocalCert),
    new BearSSL::PrivateKey(projektionfxLocalKey)
  );
}

void HttpsWebServerESP8266::registerDelayRoute(){
  secureServer.on("/delay", HTTP_GET,  [&](){
    String ParamLatencyName = "latency";
    if(!secureServer.hasArg(ParamLatencyName)){
      Serial.println("latency param not found");
      secureServer.send(500, "text/plain", "latency param not found");
      return;
    }

    String requestLatency = secureServer.arg("latency");

    Serial.print("Current stream latency: ");
    Serial.print(requestLatency.c_str());
    Serial.println("ms");
    std::istringstream iss(requestLatency.c_str());
    iss >> streamInfos.latency;
    secureServer.send(200, "text/plain", "ok");
  });
}

void HttpsWebServerESP8266::registerDefaultRoute(){
  secureServer.on("/", [&](){
    String embeddedPlayerHtml = R"(
      <!DOCTYPE html>
      <html>
          <head>
              <meta name='viewport' content='width=device-width, initial-scale=1'>
              <script src='https://player.twitch.tv/js/embed/v1.js'></script>
          </head>
          <body>
              <div id='twitchplayer'></div>
              <br>
              <div id='delay'></div>
              <script type='text/javascript'>
                  //countdown function is evoked when page is loaded
                  var lastLatency = 0;
                  function latencyCheck()
                  {
                      var latency = parseInt(player.getPlaybackStats().hlsLatencyBroadcaster * 1000);
                      document.getElementById('delay').innerHTML = latency;
                      if (lastLatency != latency)
                      {
                          fetch('/delay?latency='+latency);
                          lastLatency = latency;
                      }
                  }
                  var options = {
                      width: 1280,
                      height: 720,
                      channel: 'projektiontv',
                  };
                  var player = new Twitch.Player('twitchplayer', options);
                  player.setVolume(0.5);
                  setInterval(latencyCheck, 1000);
              </script>
          </body>
      </html>
      )";
    secureServer.keepAlive(false);
    secureServer.send(200, "text/html", embeddedPlayerHtml);
  });
}

boolean  HttpsWebServerESP8266::start(){
  secureServer.getServer().setBufferSizes(1024+325, 837); // reduce buf_in heap usage from 16k to 1k - see bearssl.h - BR_SSL_BUFSIZE_INPUT
  registerDefaultRoute();
  registerDelayRoute();
  secureServer.begin();

  Serial.println("Server ready.");
  return true;
}

void HttpsWebServerESP8266::loop(){
  secureServer.handleClient();
  MDNS.update();
}

#endif
