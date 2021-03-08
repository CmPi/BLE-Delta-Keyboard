/*
 * main.cpp
 *
 * Created on Mon Mar 08 2021
 *
 * The MIT License (MIT)
 * Copyright (c) 2021 CmPi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>

#include "BleKeyboard.h"

#include "main.h"

BleKeyboard bleKeyboard("Spanish Delta Keyboard","CmPi",100);

CapacitiveTouchButton aButtons[TOUCHS_NUM];

void setup() {

  Serial.begin(115200);
    while (!Serial)
  ;

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
  }
  
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
}

void loop() {

  int iTouchIndex;
  bool bTouch1 = false;

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
          bTouch1 = true;
        else
          if (bTouch1)
            sendAltSequence(aSequence[iTouchIndex-1]);
          else  
            sendAltSequence(aSequence[iTouchIndex-1+TOUCHS_NUM-2]);
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

}