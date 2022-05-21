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

bool DaysOfWeek::isInvalid() {
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

String DaysOfWeek::toString() {
  if(isInvalid()) {
    return "\"\"";
  }

  String result = "\"";
  bool isFirst = true;
  
  if(isEnabled(DayOfWeek::Monday)) {
    result += "Mon";
    isFirst = false;
  }
  if(isEnabled(DayOfWeek::Tuesday)) {
    if(!isFirst){
      result += ',';
    }
    result += "Tue";
    isFirst = false;
  }
  if(isEnabled(DayOfWeek::Wednesday)) {
    if(!isFirst){
      result += ',';
    }
    result += "Wed";
    isFirst = false;
  }
  if(isEnabled(DayOfWeek::Thursday)) {
    if(!isFirst){
      result += ',';
    }
    result += "Thu";
    isFirst = false;
  }
  if(isEnabled(DayOfWeek::Friday)) {
    if(!isFirst){
      result += ',';
    }
    result += "Fri";
    isFirst = false;
  }
  if(isEnabled(DayOfWeek::Saturday)) {
    if(!isFirst){
      result += ',';
    }
    result += "Sat";
    isFirst = false;
  }
  if(isEnabled(DayOfWeek::Sunday)) {
    if(!isFirst){
      result += ',';
    }
    result += "Sun";
    isFirst = false;
  }

  result += "\"";

  return result;
}