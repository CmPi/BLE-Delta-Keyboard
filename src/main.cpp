/*
* main.cpp
*
* Created on Mon Mar 08 2021
*
* Copyright (c) 2021 CmPi
*
*/

#include "main.h"

BleKeyboard bleKeyboard("Spanish Delta Keyboard", "CmPi", 100);

CapacitiveTouchButton aButtons[TOUCHS_NUM];

bool bLeftModiferArmed = false;
bool bRightModiferArmed = false;

uint32_t tsLastKey = 0;

//static const char *TAG = "main";

void RedOn() {
  pinMode(GPIO_NUM_25,OUTPUT);
  digitalWrite(GPIO_NUM_25,HIGH);
}

void RedOff() {
  pinMode(GPIO_NUM_25,OUTPUT);
  digitalWrite(GPIO_NUM_25,LOW);
}

void GreenOn()
{

}


void setup()
{
  RedOn();

  // a TAG is required by ESP_LOGx macros - let's use the defined one at least once to avoid warning
  ESP_LOGI(LOG_TAG, "%s", LOG_TAG);

  #ifdef DEBUG_SERIAL_SUPPORT
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial)
    ;
  #endif

  ds_Setup();

  bleKeyboard.begin();

  int iTouchIndex;
  for (iTouchIndex = 0; iTouchIndex < TOUCHS_NUM; iTouchIndex++)
  {
    aButtons[iTouchIndex].bIsTouched = false;
    aButtons[iTouchIndex].iInitValue = touchRead(aButtonsPin[iTouchIndex]);
    aButtons[iTouchIndex].iLastValue = 0;
    aButtons[iTouchIndex].iTouchedCnt = 0;
    aButtons[iTouchIndex].iNotTouchedCnt = 0;
    aButtons[iTouchIndex].iTouchingCnt = 0;
    ESP_LOGI(LOG_TAG, "Capacitive touch %d (GPIO %d) = %d", iTouchIndex, aButtonsPin[iTouchIndex], aButtons[iTouchIndex].iInitValue);
  }

  ESP_LOGI(LOG_TAG, "end of setup");
  RedOff();
}

void sendAltSequence(Sequence pTouche)
{
  RedOn();
  uint8_t i;
  KeyReport touch;
  for (i = 0; i < 6; i++)
    touch.keys[i] = 0;
  for (i = 0; i < 6; i++)
  {
    if (pTouche[i] == 0x00)
    {
      break;
    }
    else
    {
      touch.keys[0] = pTouche[i];
      touch.modifiers = 0x04;
      bleKeyboard.sendReport(&touch);
    }
  }
  bleKeyboard.releaseAll();
  tsLastKey = millis();
  RedOff();
}

void loop()
{

  /*
  delay(50);
  delay(50);
  digitalWrite(GPIO_NUM_25,LOW);
  */

  int iTouchIndex;

  if (bleKeyboard.isConnected())
  {
  }

  for (iTouchIndex = 0; iTouchIndex < TOUCHS_NUM; iTouchIndex++) // Circle from touch 0 to touch 9
  {
    aButtons[iTouchIndex].iLastValue = touchRead(aButtonsPin[iTouchIndex]);
    if (aButtons[iTouchIndex].iLastValue < (aButtons[iTouchIndex].iInitValue - 20))
    {
      aButtons[iTouchIndex].iTouchedCnt++;
      if (aButtons[iTouchIndex].iTouchedCnt > 5)
      {
        ESP_LOGI(LOG_TAG, "Touch %d activated", iTouchIndex);
        aButtons[iTouchIndex].iTouchedCnt = 0;

        if (!aButtons[iTouchIndex].bIsTouched)
        {
          aButtons[iTouchIndex].bIsTouched = true;

          if (NUM_MODIFIERS>0 && iTouchIndex == DKEY_LEFT_TOUCH) {
            if (bLeftModiferArmed){
              bLeftModiferArmed = false;
              RedOff();
            } else {
              bLeftModiferArmed = true;
              RedOn();
            }
            if (bRightModiferArmed)
            {
              ESP_LOGI(LOG_TAG, "Sequence RIGHT + LEFT", iTouchIndex);
              bleKeyboard.print("Delta Keyboard");
              bLeftModiferArmed = false;
              bRightModiferArmed = false;
            }

          }
          else if (NUM_MODIFIERS>1 && iTouchIndex == DKEY_RIGHT_TOUCH) {
            bRightModiferArmed = true;
            if (bLeftModiferArmed)
            {
              ESP_LOGI(LOG_TAG, "Sequence LEFT + RIGHT", iTouchIndex);
              bleKeyboard.print("Delta keyboard");
              bLeftModiferArmed = false;
              bRightModiferArmed = false;
            } else {

            }


          }
          else
          {
            ESP_LOGI(LOG_TAG, "Normal Touch %d ", iTouchIndex);  

            ESP_LOGI(LOG_TAG, "Sequence %d ", aTouchToSequence[iTouchIndex]);  

            if (!bLeftModiferArmed && !bRightModiferArmed)
              sendAltSequence(aSequence[aTouchToSequence[iTouchIndex] - 1]);
            else if (bLeftModiferArmed && !bRightModiferArmed)
              sendAltSequence(aSequence[aTouchToSequence[iTouchIndex] - 1 + KEYS_NUM]);
            else if (!bLeftModiferArmed && bRightModiferArmed)
              sendAltSequence(aSequence[aTouchToSequence[iTouchIndex] - 1 + 2*KEYS_NUM]);

            bLeftModiferArmed = false;
            bRightModiferArmed = false;
            RedOff();
          }
        }
      }
    }
    else
    {
      if (aButtons[iTouchIndex].iTouchedCnt > 0)
      {
        aButtons[iTouchIndex].iNotTouchedCnt++;
        if (aButtons[iTouchIndex].iNotTouchedCnt > 2)
        {
          aButtons[iTouchIndex].iTouchedCnt = 0;
          aButtons[iTouchIndex].iNotTouchedCnt = 0;
        }
      }
      else
      {
        aButtons[iTouchIndex].bIsTouched = false;
      }
    }
  }

  if (millis() > (tsLastKey + 120000))
  {
    RedOff();

    ds_Sleep(T6); // 1st modifier
  }
}
