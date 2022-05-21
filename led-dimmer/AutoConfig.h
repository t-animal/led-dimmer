#pragma once

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <NTPClient.h>

#include "number-parsing.h"
#include "days-of-week.h"

extern EEPROMClass EEPROM;
extern NTPClient timeClient;
extern unsigned int currentPercentage;

class AutoConfig {
public:
  bool isEnabled=false;
  uint8_t startHours=0;
  uint8_t startMinutes=0;

  uint8_t endHours=0;
  uint8_t endMinutes=0;

  uint8_t percentage=0;

  DaysOfWeek daysOfWeek;

  static bool fromRequest(ESP8266WebServer& server, AutoConfig& config);

  static bool fromEeprom(int index, AutoConfig& config);
  bool storeInEeprom(int index);

  void setPercentageIfApplicable();
  String toJson();

private: 
  String timeString(int hours, int minutes) {
    return (hours<10?"0":"") + String(hours) + ":" + (minutes<10?"0":"") + String(minutes);
  }
};