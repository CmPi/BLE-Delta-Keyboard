/*
 * main.h
 *
 * Created on Mon Mar 08 2021
 *
 * Copyright (c) 2021 CmPi
 */

#pragma once

#include <Arduino.h>

#include "config.h"

#include "BleKeyboard.h"
#include "deepsleep.h"

typedef struct
{
	bool bIsTouched;
	uint16_t iInitValue;
	uint16_t iLastValue;
  	uint8_t iTouchedCnt;
  	uint8_t iNotTouchedCnt;
  	uint8_t iTouchingCnt;
} CapacitiveTouchButton;

typedef uint8_t Sequence[6];

#include "delta_es.h"