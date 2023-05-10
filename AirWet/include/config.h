/*
 ***************************************************************************************************
 * File  : config.h
 * Date  :
 * Autor :
 * Notes :
 ***************************************************************************************************
 */

#ifndef __TEST_SERVER_H
#define __TEST_SERVER_H


#include <Arduino.h>
/*
 *************************************************************************************************** 
 *                               Definitions
 ***************************************************************************************************
 */
#define CONFIG_SD_CARD        0
#define CONFIG_SPIFFS         1

#define CONFIG_FS             CONFIG_SD_CARD
//#define CONFIG_FS             CONFIG_SPIFFS
/*
 *************************************************************************************************** 
 *                               Constants
 ***************************************************************************************************
 */
const char* ssid = "NetGear";          // your network SSID (NetGear)
const char* pass = "9961302425";       // your network password
//String _ssid = "NetGear";         // Для хранения SSID
//String _password = "9961302425";  // Для хранения пароля сети
String _ssidAP = "";             // SSID AP точки доступа
String _passwordAP = "";         // пароль точки доступа


/*
 *************************************************************************************************** 
 *                               Typedefs
 ***************************************************************************************************
 */
typedef struct {
  uint32_t id;
  uint16_t p_in_min;
  uint16_t p_in_max;
  uint16_t p_out_min;
  uint16_t p_out_max;
  uint16_t p_precarge;
  uint16_t p_drain;
  uint32_t p_delay;
  uint16_t p_timeout;
  uint32_t t_drain;
  uint32_t t_open;
  uint32_t t_close;
  uint16_t pump_rpm;
  float    Kp;
  float    Ki;
  float    Kd;
  uint16_t discharge_timeout;
  uint16_t precharge_timeout;
  uint16_t hum_min;
  uint16_t hum_max;
  char     mac;
  char     ip;
  char     mask;
  char     gateway;
  char     dns;
} gigro_board_config_t;

typedef struct {
  uint8_t  id;
  bool     active;
  int8_t   timezone;
  gigro_board_config_t board_config;
} gygro_system_t;
/*
 *************************************************************************************************** 
 *                               Variables
 ***************************************************************************************************
 */
 
#endif
