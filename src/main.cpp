/*
 * main.cpp
 *
 * Created on Mon Mar 08 2021
 *
 * Copyright (c) 2021 CmPi
 *
 */

#include "main.h"

BleKeyboard bleKeyboard("Spanish Delta Keyboard","CmPi",100);

CapacitiveTouchButton aButtons[TOUCHS_NUM];

bool bModifierArmed = false;

uint32_t tsLastKey = 0;

void setup() {

  #ifdef DEBUG_SERIAL_SUPPORT
  Serial.begin(SERIAL_BAUDRATE);
    while (!Serial)
  ;
  #endif

  ds_Setup();

 

  bleKeyboard.begin();

  int iTouchIndex;
  for (iTouchIndex=0; iTouchIndex<TOUCHS_NUM; iTouchIndex++)
  {
    aButtons[iTouchIndex].bIsTouched = false;
    aButtons[iTouchIndex].iInitValue = touchRead(aButtonsPin[iTouchIndex]);
    aButtons[iTouchIndex].iLastValue = 0;
    aButtons[iTouchIndex].iTouchedCnt = 0;
    aButtons[iTouchIndex].iNotTouchedCnt = 0;
    aButtons[iTouchIndex].iTouchingCnt = 0;

    #ifdef DEBUG_SERIAL_SUPPORT
    Serial.print("Line ");
    Serial.print(iTouchIndex);
    Serial.print(" (GPIO");
    Serial.print(aButtonsPin[iTouchIndex]);
    Serial.print(") = ");
    Serial.println(aButtons[iTouchIndex].iInitValue);
    #endif

  }

  #ifdef DEBUG_SERIAL_SUPPORT
  Serial.println("end of setup");
  #endif
  
}

void sendAltSequence(Sequence pTouche)
{
  uint8_t i;
  KeyReport touch;
  for (i=0; i<6; i++)
   touch.keys[i] = 0;
  for (i=0; i<6; i++) {
		if (pTouche[i] == 0x00) {
				break;
		} else {
      touch.keys[0] = pTouche[i]; 
      touch.modifiers = 0x04;    
      bleKeyboard.sendReport(&touch);
    }
	}
  bleKeyboard.releaseAll();
  tsLastKey = millis();
}



void loop() {

  int iTouchIndex;

  if(bleKeyboard.isConnected()) {
  }

  for (iTouchIndex=0; iTouchIndex<TOUCHS_NUM; iTouchIndex++)
  {
    aButtons[iTouchIndex].iLastValue = touchRead(aButtonsPin[iTouchIndex]);
    if (aButtons[iTouchIndex].iLastValue<(aButtons[iTouchIndex].iInitValue-20))
    {
      aButtons[iTouchIndex].iTouchedCnt++;
      if (aButtons[iTouchIndex].iTouchedCnt>5)
      {
      Serial.print("Touch ");
      Serial.print( iTouchIndex );
      Serial.println(" !");
      aButtons[iTouchIndex].iTouchedCnt = 0;

      if (!aButtons[iTouchIndex].bIsTouched)
      {
        aButtons[iTouchIndex].bIsTouched = true;

        if (iTouchIndex==0)
          bModifierArmed = true;
        else
        {
          if (!bModifierArmed)
            sendAltSequence(aSequence[iTouchIndex-1]);
          else  
            sendAltSequence(aSequence[iTouchIndex-1+TOUCHS_NUM-2]);
          bModifierArmed = false;
        }
      }
      }
    } else {
      if (aButtons[iTouchIndex].iTouchedCnt>0)
      {
        aButtons[iTouchIndex].iNotTouchedCnt++;
        if (aButtons[iTouchIndex].iNotTouchedCnt>2)
        {
          aButtons[iTouchIndex].iTouchedCnt = 0;
          aButtons[iTouchIndex].iNotTouchedCnt = 0;
        }
      } else {
        aButtons[iTouchIndex].bIsTouched = false;
      }      
    }

  }

  if (millis()>(tsLastKey+10000)) {
    ds_Sleep(T0);
  }

}
