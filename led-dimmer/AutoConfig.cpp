#include "AutoConfig.h"

bool AutoConfig::fromRequest(ESP8266WebServer& server, AutoConfig& config) {
  int isEnabled = server.hasArg("enabled");
  int startHours = parseHours(server.arg("startTime").substring(0, 2));
  int startMinutes = parseMinutes(server.arg("startTime").substring(3, 5));
  int endHours = parseHours(server.arg("endTime").substring(0, 2));
  int endMinutes = parseMinutes(server.arg("endTime").substring(3, 5));
  int percentage = parsePercentage(server.arg("percentage"));
  DaysOfWeek daysOfWeek;
  daysOfWeek.parseFromStringList(server.arg("daysOfWeek"));


  if(startHours < 0 || startMinutes < 0 || endHours < 0 || endMinutes < 0 || percentage < 0 
     || daysOfWeek.isInvalid()) {
     return false;
  }

  config.isEnabled = isEnabled;
  config.startHours = startHours;
  config.startMinutes = startMinutes;
  config.endHours = endHours;
  config.endMinutes = endMinutes;
  config.percentage = percentage;
  config.daysOfWeek = daysOfWeek;
  return true;
}

bool AutoConfig::fromEeprom(int index, AutoConfig& config) {
  EEPROM.get(index * sizeof(AutoConfig), config);
  return true;
}

bool AutoConfig:: storeInEeprom(int index) {
  EEPROM.put(index * sizeof(AutoConfig), *this);
  EEPROM.commit();
  return true;
}

void AutoConfig::setPercentageIfApplicable() {
  if(!isEnabled) {
    return;
  }

  DayOfWeek today = sanitizeDayOfWeek(timeClient.getDay());
  if(!daysOfWeek.isEnabled(today)){
    return;
  }

  if(timeClient.getHours() == startHours && timeClient.getMinutes() == startMinutes) {
    currentPercentage = percentage;
  }

  if(timeClient.getHours() == endHours && timeClient.getMinutes() == endMinutes) {
    currentPercentage = 0;
  }
}

String AutoConfig::toJson() {
  String json = "{\"enabled\": " + String(isEnabled ?"true": "false") + ",";
  json += "\"startTime\": \"" + timeString(startHours, startMinutes) + "\",";
  json += "\"endTime\": \""   + timeString(endHours, endMinutes) + "\",";
  json += "\"percentage\": " + String(percentage)  + ",";
  json += "\"daysOfWeek\": " + daysOfWeek.toString() + "}";
  return json;
}
