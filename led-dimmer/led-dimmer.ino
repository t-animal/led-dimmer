#include "string"

#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include <ctime>

#include "number-parsing.h"
#include "route-handlers.h"
#include "AutoConfig.h"
#include "logging.h"

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

Logger<1024> logger;

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
  server.on("/log", httpServerHandleLog);
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

  logDate();
  logger.write("Started up\n");
}

void loop() {
  unsigned int oldPercentage = currentPercentage;
  server.handleClient();
  timeClient.update();
  if(oldPercentage != currentPercentage) {
    logPercentageChange("Manually updated percentage", oldPercentage);
  }
  oldPercentage = currentPercentage;

  config0.setPercentageIfApplicable();
  config1.setPercentageIfApplicable();
  config2.setPercentageIfApplicable();
  config3.setPercentageIfApplicable();
  config4.setPercentageIfApplicable();
  analogWrite(PWM_PIN, (100 - currentPercentage) * 1023 / 100); 
  
  if(oldPercentage != currentPercentage) {
    logPercentageChange("Auto-updated percentage", oldPercentage);
  }
}


void logPercentageChange(String intro, unsigned int oldPercentage) {
  logDate();
  logger.write(intro + " from "+ String(oldPercentage)+ "% to "+ String(currentPercentage)+ "%\n");
}

void logDate() {
  char date[21];
  time_t time = timeClient.getEpochTime();
  std::strftime(date, 21, "%F %T ", std::gmtime(&time));
  logger.write(date);
}