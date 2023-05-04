
/*
    Name:       TestRingBuffer.ino
    Created:	01/05/2023 16:01:48
    Author:     Robert E Bridges (C) 4 May 2023
*/

#include "Arduino.h"
#include <RingBuffer.h>
#include <EEPROM.h>

#define RingBufferArrayEEPROMAddr 0

RingBuffer::infoType data;

uint32_t currentBufferSize;

#define arrayBufferSize 20
char buffer[arrayBufferSize];

RingBuffer cBuf(arrayBufferSize);

void setup()
{
    Serial.begin(9600);
    while (!Serial && millis() < 5000);
    Serial.println(F("Putting some data uint32_to Ring Buffer, but NOT filling it."));
    Serial.println(F("Adding to buffer with A to K inclusive"));

    for (char i = 'A'; i < 'L'; i++) {
        buffer[cBuf.AddDataToBuffer()] = i;
    }

    Serial.println(F("Reading from Ring buffer - from Head to Tail (LIFO)"));
    Serial.print( buffer[ cBuf.ReadFromHead() ] );

    for (uint32_t i = 1; i < cBuf.currentBufferLength(); i++) { // wouldn't normally use "i < cBuf.BufferLength();" just used to demonstrate use
        Serial.print( buffer[ cBuf.ReadNext() ]);        // better to use next example. The limit is not repeatedly evaluated.
    }
    Serial.println();

    Serial.println(F("Reading from Ring buffer in opposite direction - from Tail to Head (FIFO)"));
    Serial.print( buffer[ cBuf.ReadFromTail() ] );

    currentBufferSize = cBuf.currentBufferLength();
    for (uint32_t i = 1; i < currentBufferSize; i++) {
        Serial.print( buffer[ cBuf.ReadNext() ] );
    }
    Serial.println();

    Serial.println(F("Adding more data to just fill the buffer."));
    Serial.println(F("Adding extra to buffer with L to U inclusive"));

    for (char i = 'L'; i < 'U'; i++) {
        buffer[cBuf.AddDataToBuffer()] = i;
    }

    Serial.print( buffer[ cBuf.ReadFromHead() ] );

    currentBufferSize = cBuf.currentBufferLength();
    for (uint32_t i = 1; i < currentBufferSize; i++) {
        Serial.print( buffer[ cBuf.ReadNext() ] );       
    }

    Serial.println();
    Serial.print( buffer[ cBuf.ReadFromTail() ] );

    for (uint32_t i = 1; i < cBuf.currentBufferLength(); i++) {
        Serial.print( buffer[ cBuf.ReadNext() ] );
    }
    Serial.println();

    Serial.println(F("Adding extra to go uint32_to roll over."));
    Serial.println(F("Adding extra to buffer with U to Z inclusive" ));

    for (char i = 'U'; i < '['; i++) {
        buffer[cBuf.AddDataToBuffer()] = i;
    }
    Serial.print( buffer[ cBuf.ReadFromHead() ] );
    currentBufferSize = cBuf.currentBufferLength();
    for (uint32_t i = 1; i < currentBufferSize; i++) {
        Serial.print( buffer[ cBuf.ReadNext() ] );
    }
    Serial.println();
    Serial.print( buffer[ cBuf.ReadFromTail() ] );
    for (uint32_t i = 1; i < currentBufferSize; i++) {
        Serial.println( buffer[cBuf.ReadNext() ] );
    }
    Serial.println();
    
    Serial.println("Save away Ring Buffer to EEPROM");
    data=cBuf.SaveRBInfo();
    EEPROM.put(RingBufferArrayEEPROMAddr, buffer);
    EEPROM.put(RingBufferArrayEEPROMAddr + sizeof(buffer), data);

    Serial.println("Get Back Ring Buffer from EEPROM");
    EEPROM.get(RingBufferArrayEEPROMAddr, buffer);
    EEPROM.get(RingBufferArrayEEPROMAddr + sizeof(buffer), data);
    cBuf.RestoreRBInfo(data);
}

void loop()
{


}
