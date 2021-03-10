/*
 * config.h
 *
 * Created on Mon Mar 08 2021
 *
 * Copyright (c) 2021 CmPi
 *
 */

// Serial debug log

#ifndef DEBUG_SERIAL_SUPPORT
#define DEBUG_SERIAL_SUPPORT    1               // Enable serial debug log
#endif

#ifndef SERIAL_BAUDRATE
#define SERIAL_BAUDRATE         115200          // Default baudrate
#endif

#ifdef LOG_LOCAL_LEVEL 
#undef LOG_LOCAL_LEVEL 
#endif
#define LOG_LOCAL_LEVEL  ESP_LOG_VERBOSE
