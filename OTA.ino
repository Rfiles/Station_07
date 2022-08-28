#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);


#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <TelnetStream.h>



void setupota (void) {
  if (WiFi.status() != WL_CONNECTED) {
    printf( "OTA: Aborted. No Wifi.\n" );
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Rfiles ESP32");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();
  Serial.println("OTA: Server started.");
    
}

void AutoUpdate () {
  if (WiFi.status() != WL_CONNECTED) {
    printf( "AU: Aborted. No Wifi.\n" );
    return;
  }
  if ( checkfirmware() ) {
    printf("AU: Found New Firmware");
    updatefirmware();
  }
}

bool checkfirmware () {
  HTTPClient http;
  http.begin(firmwareversion);

  int httpCode = http.GET();
  if(httpCode > 0) {
    uint8_t filenum;
    printf("FWC: Server Get Response [%d]\n", httpCode);
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      filenum = payload.toInt();
      printf("FWC: Latest[%s][%d] Installed[%d]\n", payload, filenum, STATION_VERSION);
      if ( filenum <= STATION_VERSION ) return false;
      return true;
    }
  } else {
    printf("FWC: Check Failed. Error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return false;
  }

  return false;
 
}

void updatefirmware () {
    WiFiClient fwclient;

    httpUpdate.onStart(update_started);
    httpUpdate.onEnd(update_finished);
    httpUpdate.onProgress(update_progress);
    httpUpdate.onError(update_error);

    t_httpUpdate_return ret = httpUpdate.update(fwclient, firmwarepath);

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("FW: HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("FW: No Updates Available.");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("FW: HTTP Update DONE");
        break;
    }


  
}

void update_started() {
  Serial.println("FU:  HTTP update process started");
}

void update_finished() {
  Serial.println("FU:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("FU:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("FU:  HTTP update fatal error code %d\n", err);
}

bool setuptelnet () {
  TelnetStream.begin();
  
  return true;
}

