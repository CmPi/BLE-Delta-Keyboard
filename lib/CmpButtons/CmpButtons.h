/*
 * CmPButtons.h
 *
 * Created on Tue Mar 09 2021
 *
 * Copyright (c) 2021 CmPi
 *
 */

#pragma once

#ifndef CmPButtons_h

#define CmPButtons_h

#include "Arduino.h"

#define DEBOUNCE_MS      50
#define LONGCLICK_MS    250
#define DOUBLECLICK_MS  400

#define SINGLE_CLICK      1
#define DOUBLE_CLICK      2
#define LONG_CLICK        4

class CmpButton
{
    private:
        uint8_t pin;
        uint16_t debounce_time_ms;

    public:
        CmpButton(uint8_t attachTo, uint8_t buttonType = INPUT_PULLUP, unsigned int debounceTimeout = DEBOUNCE_MS);
        void setDebounceTime(unsigned int ms);
};

#endif
