#pragma once

#include "Arduino.h"

const int INCORRECT_DIGIT_COUNT=-1;
const int MALFORMATTED_STRING=-2;
const int OUT_OF_RANGE=-3;

// Values according to timeclient source code
enum DayOfWeek {
    Sunday = 0,
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6,
    Unknown = 7
};

int parsePositiveNumber(String string, unsigned int minDigits=1, unsigned int maxDigits=3, int min=0, int max=999);

int parseHours(String string); 
int parseMinutes(String string);
int parsePercentage(String string);

/*
 * Parses three letter abbreviations.
 * E.g. "Mon" => Monday, "Wed" => Wednesday, "Fri" => Friday
 * Everything else parses to DayOfWeek.Unkown
 */
DayOfWeek parseDayOfWeek(String string);
DayOfWeek sanitizeDayOfWeek(int number);