/*
 * deepsleep.cpp
 *
 * Added on Tue Mar 09 2021
 *
 */

#include "deepsleep.h"

uint32_t ds_tsReboot = 0;
uint32_t ds_tsWakeup = 0;

// touchPin;

RTC_DATA_ATTR int ds_iBootCount = 0;
RTC_DATA_ATTR uint32_t ds_sAwakeTime = 0;

/*
Method to print the touchpad by which ESP32
has been awaken from sleep
*/
/// Debug - (Serial) Print

void ds_PrintWakeupTouchpad()
{
  touch_pad_t touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch (touchPin)
  {
  case 0:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 4" );
    break;
  case 1:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 0");
    break;
  case 2:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 2");
    break;
  case 3:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 15");
    break;
  case 4:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 13");
    break;
  case 5:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 12");
    break;
  case 6:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 14");
    break;
  case 7:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 27");
    break;
  case 8:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 33");
    break;
  case 9:
    ESP_LOGI( LOG_TAG, "Touch detected on GPIO 32");
    break;
  default:
    ESP_LOGI( LOG_TAG, "Wakeup not by touchpad");
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
    ESP_LOGI( TAG, "Wakeup caused by touchpad" );
    ds_PrintWakeupTouchpad();
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    ESP_LOGI( LOG_TAG, "Wakeup was not caused by deep sleep: %d\n", wakeup_reason );
    break;
  }
}

void ds_Setup()
{
  ds_tsWakeup = millis();

  //Increment boot number and print it every reboot
  ++ds_iBootCount;
  if (ds_iBootCount<2) 
  {
    ds_sAwakeTime = 0;
    ds_tsReboot = ds_tsWakeup;
  }

 // Serial.println("Boot number: " + String(ds_iBootCount));
  ESP_LOGI( LOG_TAG, "Boot number: %d", ds_iBootCount );
  ESP_LOGI( LOG_TAG, "ds_tsReboot = %d", ds_tsReboot );
  ESP_LOGI( LOG_TAG, "ds_tsWakeup = %d", ds_tsWakeup );
  ESP_LOGI( LOG_TAG, "ds_sAwakeTime = %d", ds_sAwakeTime );

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
  ESP_LOGI(TAG, "Going to sleep now");
  delay(500);
  touchAttachInterrupt(Pin, ds_DefaultCallback, DS_TRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  ds_sAwakeTime =+ (ds_tsWakeup-millis())/1000;
  esp_deep_sleep_start();
}