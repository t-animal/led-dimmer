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