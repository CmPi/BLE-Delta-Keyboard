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

#define CONFIG_LOG_DEFAULT_LEVEL ESP_LOGV
