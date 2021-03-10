/*
 * deepsleep.cpp
 *
 * Added on Tue Mar 09 2021
 *
 */

#include "deepsleep.h"

static const char *TAG = "deepsleep";

touch_pad_t touchPin;

RTC_DATA_ATTR int ds_iBootCount = 0;

/*
Method to print the touchpad by which ESP32
has been awaken from sleep
*/
/// Debug - (Serial) Print

void ds_PrintWakeupTouchpad()
{
  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch (touchPin)
  {
  case 0:
    Serial.println("Touch detected on GPIO 4");
    break;
  case 1:
    Serial.println("Touch detected on GPIO 0");
    break;
  case 2:
    Serial.println("Touch detected on GPIO 2");
    break;
  case 3:
    Serial.println("Touch detected on GPIO 15");
    break;
  case 4:
    Serial.println("Touch detected on GPIO 13");
    break;
  case 5:
    Serial.println("Touch detected on GPIO 12");
    break;
  case 6:
    Serial.println("Touch detected on GPIO 14");
    break;
  case 7:
    Serial.println("Touch detected on GPIO 27");
    break;
  case 8:
    Serial.println("Touch detected on GPIO 33");
    break;
  case 9:
    Serial.println("Touch detected on GPIO 32");
    break;
  default:
    Serial.println("Wakeup not by touchpad");
    break;
  }
}

void ds_PrintWakeupReason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    ds_PrintWakeupTouchpad();
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void ds_Setup()
{
  //Increment boot number and print it every reboot
  ++ds_iBootCount;
  Serial.println("Boot number: " + String(ds_iBootCount));

  //Print the wakeup reason for ESP32
  ds_PrintWakeupReason();
}

/**
* @brief Default coallback function
*/

void ds_DefaultCallback()
{
}

void ds_Sleep(uint8_t Pin)
{
  ESP_LOGW(TAG, "Going to sleep now");
  delay(500);
  touchAttachInterrupt(T3, ds_DefaultCallback, DS_TRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  esp_deep_sleep_start();
}