/*
 * CmpButtons.cpp
 *
 * Created on Tue Mar 09 2021
 *
 * Copyright (c) 2021 CmPi
 *
 */

#include "Arduino.h"
#include "CmpButtons.h"

CmpButton::CmpButton(byte attachTo, byte buttonMode /*= INPUT_PULLUP*/, unsigned int debounceTimeout /*= DEBOUNCE_MS*/)
{
    pin = attachTo;
    setDebounceTime(debounceTimeout);
    pinMode(attachTo, buttonMode);
}

void CmpButton::setDebounceTime(unsigned int ms)
{
    debounce_time_ms = ms;
}
