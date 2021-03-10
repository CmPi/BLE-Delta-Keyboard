

#pragma once

#ifndef DEEPSLEEP_H

#define DEEPSLEEP_H

#include <Arduino.h>

// sensibility of the touchpin for awaking

#ifndef DS_TRESHOLD
#define DS_TRESHOLD 40
#endif

/**
 * @brief debug - (serial) print the wakeup reason
 * @return nothing
 */

void ds_PrintWakeupReason();

/**
 * @brief  
 * 
 */

void ds_Setup();

/**
 * @brief Get the touch pad which caused wakeup
 *
 * If wakeup was caused by another source, this function will return TOUCH_PAD_MAX;
 *
 * @return touch pad which caused wakeup
 */

void ds_Sleep( uint8_t Pin );

#endif