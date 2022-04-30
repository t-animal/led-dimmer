#include "string"

#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "number-parsing.h"
#include "route-handlers.h"
#include "AutoConfig.h"

#define PWM_PIN D4

unsigned int currentPercentage = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

ESP8266WebServer server(80);

AutoConfig config0;
AutoConfig config1;
AutoConfig config2;
AutoConfig config3;
AutoConfig config4;

void setUpAndConnectWifi() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //  wm.resetSettings();

  bool res;
  wm.setHostname("LedDimmer");
  res = wm.autoConnect("LedDimmer");

  if(!res) {
    Serial.println("Connection to WiFi failed");
    ESP.restart();
  } 
  else {
    Serial.println("Successfully connected to Wifi)");
  }
}

void setupAndStartServer() {
  server.on("/", httpServerHandleRoot);
  server.on("/current", httpServerHandleDimValue);
  server.on("/time", httpServerHandleTime);
  server.on("/time/update", httpServerHandleTimeUpdate);
  server.on("/config/0", [](){httpServerHandleConfig(0, config0);});
  server.on("/config/1", [](){httpServerHandleConfig(1, config1);});
  server.on("/config/2", [](){httpServerHandleConfig(2, config2);});
  server.on("/config/3", [](){httpServerHandleConfig(3, config3);});
  server.on("/config/4", [](){httpServerHandleConfig(4, config4);});
  server.onNotFound(httpServerHandleNotFound);
  
  const char *headers[] = {contentType,};
  server.collectHeaders(headers, 1);
  server.begin();
}

void setup() {
  Serial.begin(115200);

  setUpAndConnectWifi();

  pinMode(PWM_PIN, OUTPUT);

  setupAndStartServer();
  
  timeClient.begin();
  timeClient.setUpdateInterval(60*60*1000);
  timeClient.forceUpdate();

  EEPROM.begin(5 * sizeof(AutoConfig));
  AutoConfig::fromEeprom(0, config0);
  AutoConfig::fromEeprom(1, config1);
  AutoConfig::fromEeprom(2, config2);
  AutoConfig::fromEeprom(3, config3);
  AutoConfig::fromEeprom(4, config4);
}

void loop() {
  server.handleClient();
  timeClient.update();
  config0.setPercentageIfApplicable();
  analogWrite(PWM_PIN, (100 - currentPercentage) * 1023 / 100); 
}
