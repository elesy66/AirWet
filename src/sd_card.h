/*
******************************************************************************************
** File  : sd_fs.h
** Date  : 26.04.2023
** Autor : I.Zaicev
** Notes :
******************************************************************************************
*/
#ifndef _SD_CARD_H
#define _SD_CARD_H

#include <SPI.h>
#include <SD.h>
#include <FS.h>

/*
*******************************************************************************
**                        Definitions
*******************************************************************************
*/
#define SD_SCK      18
#define SD_MISO     19
#define SD_MOSI     5
#define SD_CS       33

/*
*******************************************************************************
**                        Constants
*******************************************************************************
*/

/*
*******************************************************************************
**                        Typedefs
*******************************************************************************
*/

/*
*******************************************************************************
**                        Variables
*******************************************************************************
*/
SPIClass spi = SPIClass(VSPI);
/*
*******************************************************************************
**                        Function Prototipes
*******************************************************************************
*/
void sd_init ( void );

#endif