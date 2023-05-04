/*
******************************************************************************************
** File  : main.cpp
** Date  : 26.04.2023
** Autor : I.Zaicev
** Notes :
******************************************************************************************
*/
#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>

#include "sd_card.h"

/*$PAGE$*/
/*
*******************************************************************************
** Description :
** Parameter   : none
** Return      : none
** Notes       :
*******************************************************************************
*/
void SPIFFS_Init ( void )
{
  Serial.println("SPIFFS Init ... ");
  if(!SPIFFS.begin(true)){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  } else {
    Serial.println("SPIFFS Init ... Ok");
  }
}
/*$PAGE$*/
/*
*******************************************************************************
** Description :
** Parameter   : none
** Return      : none
** Notes       :
*******************************************************************************
*/
void setup() {
  // put your setup code here, to run once:
}
/*$PAGE$*/
/*
*******************************************************************************
** Description :
** Parameter   : none
** Return      : none
** Notes       :
*******************************************************************************
*/
void loop() {
  // put your main code here, to run repeatedly:
  if ( Serial.readString() == "ls" ) {
    Serial.print ("***");
  }
}