#include "days-of-week.h"

void DaysOfWeek::parseFromStringList(String abbreviatedList) {
  this->bitField = 0;
  String restOfString = abbreviatedList;

  while(restOfString.length() >= 3) {
    DayOfWeek currentDay = parseDayOfWeek(restOfString.substring(0,3));
    if(currentDay == DayOfWeek::Unknown){
      this->bitField = INVALID;
    }
    enableDay(currentDay);

    if(restOfString.length() < 4 || restOfString[3] != ',') {
      this->bitField = INVALID;
    }
    restOfString = restOfString.substring(4);
  }
}

bool DaysOfWeek::isValid() {
  return this->bitField == INVALID;
}

void DaysOfWeek::enableDay(DayOfWeek day) {
  this->bitField |= 1 << day;
}

void DaysOfWeek::disableDay(DayOfWeek day) {
  this->bitField &= ~(1 << day);
}

bool DaysOfWeek::isEnabled(DayOfWeek day) {
  if(day == DayOfWeek::Unknown) {
    return false;
  }

  return this->bitField & (1 << day);
}