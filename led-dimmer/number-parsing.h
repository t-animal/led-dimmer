#pragma once

#include "Arduino.h"

const int INCORRECT_DIGIT_COUNT=-1;
const int MALFORMATTED_STRING=-2;
const int OUT_OF_RANGE=-3;

int parsePositiveNumber(String string, unsigned int minDigits=1, unsigned int maxDigits=3, int min=0, int max=999);

int parseHours(String string); 
int parseMinutes(String string);
int parsePercentage(String string);
