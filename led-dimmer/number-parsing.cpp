#include "number-parsing.h"

int parsePositiveNumber(String string, unsigned int minDigits, unsigned int maxDigits, int min, int max) {
    if(string.length() > maxDigits || string.length() < minDigits) {
      return INCORRECT_DIGIT_COUNT;
    }

    for (size_t i = 0; i < string.length(); i++) {
      if(!isDigit(string[i])) {
        return MALFORMATTED_STRING;
      }
    }

    int value = string.toInt();
    
    if(value < min || value > max) {
      return OUT_OF_RANGE;
    }

    return value;
}

int parseHours(String string) { return parsePositiveNumber(string, 2, 2, 0, 23); }
int parseMinutes(String string) { return parsePositiveNumber(string, 2, 2, 0, 59); }
int parsePercentage(String string) { return parsePositiveNumber(string, 1, 3, 0, 100); }

DayOfWeek parseDayOfWeek(String string) {
  if(string.equals("Mon")) {
    return DayOfWeek::Monday;
  }

  if(string.equals("Tue")) {
    return DayOfWeek::Tuesday;
  }

  if(string.equals("Wed")) {
    return DayOfWeek::Wednesday;
  }

  if(string.equals("Thu")) {
    return DayOfWeek::Thursday;
  }

  if(string.equals("Fri")) {
    return DayOfWeek::Friday;
  }

  if(string.equals("Sat")) {
    return DayOfWeek::Saturday;
  }

  if(string.equals("Sun")) {
    return DayOfWeek::Sunday;
  }

  return DayOfWeek::Unknown;
}

DayOfWeek sanitizeDayOfWeek(int number) {
  if(number < 0 || number > 7){
    return DayOfWeek::Unknown;
  }

  return (DayOfWeek) number;
}