/*
 ***************************************************************************************************
 * File  : mb.cpp
 * Date  :
 * Autor :
 * Notes :
 * https://github.com/plerup/espsoftwareserial/blob/main/examples/bitpattern/bitpattern.ino
 * https://github.com/eModbus/eModbus
 ***************************************************************************************************
 */
#include "mb.h"

/*
 *************************************************************************************************** 
 *                               Variables
 ***************************************************************************************************
 */
//EspSoftwareSerial::UART swSer;

/*$PAGE$*/
/*
*******************************************************************************
** Description :
** Parameter   : none
** Return      : none
** Notes       :
*******************************************************************************
*/
std::atomic<bool> rxPending(false);
void IRAM_ATTR receiveHandler ()
{
    rxPending.store(false);
    //esp_schedule();
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
void mb_init ( void )
{
    Serial.println ("SW Serial Init ...");
    swSer.begin(BAUDRATE, EspSoftwareSerial::SWSERIAL_8N1, RX, TX);
    
    swSer.enableIntTx ( false );
    swSer.onReceive ( receiveHandler );
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

/*$PAGE$*/
/*
*******************************************************************************
** Description :
** Parameter   : none
** Return      : none
** Notes       :
*******************************************************************************
*/
