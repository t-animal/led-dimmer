#pragma once

#include "Arduino.h"
#include <NTPClient.h>

template<size_t bufferSize>
class Logger {
public:
    void write(String message);
    String read();

    Logger() {
        memset(ringBuffer, 0, bufferSize);
    }

private:
    const size_t ringBufferSize = bufferSize;
    const char *endOfRingBuffer = ringBuffer + bufferSize;
    char ringBuffer[bufferSize];

    bool wrappedAtLeastOnce = false;
    char* currentWriteStartPointer = ringBuffer; // points to end ('\0') of previous string
    char* currentReadStartPointer = ringBuffer;
};


template<size_t bufferSize> 
void Logger<bufferSize>::write(String message) {
    unsigned int bytesNeeded = message.length() + 1;
    if(bytesNeeded > ringBufferSize) {
        return;
    }

    int bufferRemaining = endOfRingBuffer - currentWriteStartPointer; // off-by-one? // growth direction?
    if(bufferRemaining > bytesNeeded) {
        Serial.println("Logging " + message);
        Serial.println(String(bufferRemaining) + " bytes remaining,  " + bytesNeeded + " needed");
        message.toCharArray(currentWriteStartPointer, bytesNeeded);
        currentWriteStartPointer += bytesNeeded - 1;
    }

    if(bufferRemaining == bytesNeeded) {
        Serial.println("Logging " + message);
        Serial.println(String(bufferRemaining) + " bytes remaining,  " + bytesNeeded + " needed");
        Serial.println("Hitting end of buffer exactly");
        message.toCharArray(currentWriteStartPointer, bytesNeeded);
        currentWriteStartPointer = ringBuffer;
        wrappedAtLeastOnce=true;
    }

    if(bufferRemaining < bytesNeeded) {
        //  Hallo Welt
        // 012345789 12345789 12345789 12345789 12345789 12345789
        //'------------------------------------------------------'
        Serial.println("Logging " + message);
        Serial.println(String(bufferRemaining) + " bytes remaining,  " + bytesNeeded + " needed");
        Serial.println("Have to split message");
        message.toCharArray(currentWriteStartPointer, bufferRemaining);
        Serial.println("Splitting up in " + String(currentWriteStartPointer));

        unsigned int messageRemaining = message.length() - bufferRemaining + 2;
        message.toCharArray(ringBuffer, messageRemaining, bufferRemaining - 1);
        Serial.println("Splitting up in " + String(ringBuffer));

        currentWriteStartPointer = ringBuffer + messageRemaining - 1;
        wrappedAtLeastOnce=true;
    }

    if(wrappedAtLeastOnce) {
        currentReadStartPointer = currentWriteStartPointer + 1;
    }

    Serial.println("Start pointer is now at " + String((int) currentWriteStartPointer) + "(" + String((int) currentWriteStartPointer - (int) ringBuffer) + ")");
    Serial.println("Read pointer is now at " + String((int) currentReadStartPointer) + "(" + String((int) currentReadStartPointer - (int) ringBuffer) + ")");
}

template<size_t bufferSize> 
String Logger<bufferSize>::read() {
    if(currentReadStartPointer == ringBuffer && *ringBuffer == 0) {
        // nothing written yet
        return String();
    }

    if(currentReadStartPointer == ringBuffer) {
        return String(ringBuffer);
    }

    String start = String(currentReadStartPointer);
    Serial.println("Reading");
    Serial.print(start);
    start.concat(String(ringBuffer));
    Serial.println("Concat with");
    Serial.print(String(ringBuffer));
    Serial.println("Buffer content:");

    for(int i=0; i<1024; i++) {
        Serial.print(ringBuffer[i] < ' ' ? "["+ String((int)ringBuffer[i]) + "]": ringBuffer[i]);
    }
    return start;
}