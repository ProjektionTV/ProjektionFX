#include <https_web_server.h>

#include <ESPmDNS.h>

#include <sstream>
#include <cstdint>

uint64_t latestLatency = 0;
void HttpsWebServer::setupDNS(){
      if (!MDNS.begin("projektionfx")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
}

void HttpsWebServer::generateSSLCert(){
  Serial.println("Creating certificate... (takes ~30s)");
  
  cert = new SSLCert();

  int createCertResult = createSelfSignedCert(
    *cert,
    SSLKeySize::KEYSIZE_2048,
    "CN=projektionfx.local,O=acme,C=DE");
  
  if (createCertResult != 0) {
    Serial.printf("Error generating certificate");
    return; 
  }

  Serial.println("Certificate created with success");
}

void HttpsWebServer::registerDelayRoute(){
  ResourceNode * nodeStreamDelay = new ResourceNode("/delay", "GET", [](HTTPRequest * req, HTTPResponse * res){
    ResourceParameters * params = req->getParams();
    std::string ParamLatencyName = "latency";
    std::string requestLatency;
    if (!params->getQueryParameter(ParamLatencyName, requestLatency)){
      Serial.println("latency param not found");
      return;
    }

    Serial.print("Current stream latency: ");
    Serial.print(requestLatency.c_str());
    Serial.println("ms");
    std::istringstream iss(requestLatency);
    iss >> latestLatency;
  });
  secureServer->registerNode(nodeStreamDelay);
}

void HttpsWebServer::registerDefaultRoute(){
  ResourceNode * nodeRoot = new ResourceNode("/", "GET", [](HTTPRequest * req, HTTPResponse * res)
  {
    res->setHeader("Content-Type", "text/html");
    String s = R"(
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
    res->println(s);
  });
  secureServer->registerNode(nodeRoot);
}

boolean  HttpsWebServer::start(){
  secureServer = new HTTPSServer(cert);
  registerDefaultRoute();
  registerDelayRoute();
  secureServer->start();

  if (secureServer->isRunning()) {
    return false;
  }
  Serial.println("Server ready.");
  return true;
}

void HttpsWebServer::loop(){
  secureServer->loop();
  this->streamLatency = latestLatency;
}
