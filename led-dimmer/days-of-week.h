#pragma once

#include "number-parsing.h"

#define INVALID 0x80;

class DaysOfWeek {
public:
  /**
    * Resets to all days inactive, then parses three letter
    * abbreviations separated by comma.
    * E.g. "Mon,Wed,Fri,Sun" parses to Monday, Wednesday,
    * Friday and Sunday. Any invalid input stops parsing at
    * the invalid input, the isValid method will return false,
    * and no days will be activated.
    */
  void parseFromStringList(String abbreviatedList);
  bool isValid();

  void enableDay(DayOfWeek day);
  void disableDay(DayOfWeek day);
  bool isEnabled(DayOfWeek day);
private:
    uint8_t bitField = 0;
};