#pragma once
#include "Arduino.h"

uint8_t LATCHPIN = 0;
uint8_t CLOCKPIN = 5;
uint8_t DATAPIN = 35;

byte buttons = 0;
byte dip = 0;

uint8_t lastState = 0;

byte shiftIn(uint8_t data, uint8_t clock) 
{
    int i;
    int temp = 0;
    byte bitArray = 0;

    pinMode(clock, OUTPUT);
    pinMode(data, INPUT);

    for (i = 7; i >= 0; i--) 
    {

        digitalWrite(clock, 0);
        delayMicroseconds(2);

        temp = digitalRead(data);
        if (temp) 
        {
            bitArray = bitArray | (1 << i);
        }

        digitalWrite(clock, 1);
        delayMicroseconds(0.2);
    }
    return bitArray;
}

/// @brief Read the buttons and DIP switches from the CD4021B chip
void ReadButtons() 
{
    buttons = 0x0;
    dip = 0x0;
    digitalWrite(LATCHPIN, 1);
    delayMicroseconds(40);
    digitalWrite(LATCHPIN, 0);

    buttons = shiftIn(DATAPIN, CLOCKPIN);
    dip = shiftIn(DATAPIN, CLOCKPIN);

}

void ConfigureButtonPins()
{
    pinMode(CLOCKPIN, OUTPUT);
    pinMode(DATAPIN, INPUT);
    pinMode(LATCHPIN, OUTPUT);
}