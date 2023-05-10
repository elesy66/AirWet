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
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "AsyncJson.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <stdio.h>
#include "config.h"
#include "sd_card.h"
#include "mb.h"


String esprev;
String espcpufreq;
String espflash;   

// Создаем экземпляр класса «AsyncWebServer»
// под названием «server» и задаем ему номер порта «80»:
AsyncWebServer server(80);

const char* mqtt_server = "5.181.253.216";
uint16_t    mqtt_port = 1883;
const char  mqtt_client[] = "ESP32_MQTT";

uint8_t  Fw_topic = 0;
uint32_t lastMsg = 0;
char     msg[50];

WiFiClient  espClient;
PubSubClient client(espClient);


EspSoftwareSerial::UART swSer;

void SPIFFS_Init ( void );
void WiFiAP_Client_Init ( uint8_t mode );
void printWiFiStatus ( void );
void Server_Init ( void );
void MQTT_Init ( void );

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
void WiFiAP_Client_Init ( uint8_t mode )
{
  if ( mode == 1 ) {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi Init ... start");
    //WiFi.mode(WIFI_AP_STA);
    WiFi.mode(WIFI_STA);
    Serial.print("Connect to Nets : ");
    Serial.println( ssid );
    WiFi.begin ( ssid, pass );
    //WiFi.begin(_ssid.c_str(), _password.c_str());
    //WiFi.softAP(_ssid.c_str(), _password.c_str());
    //WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  
    //IPAddress myIP = WiFi.softAPIP();
    //Serial.println(" ");
    //Serial.print("AP IP address: ");
    //Serial.println(myIP);
  

    //Serial.print("MAC address: ");
    //Serial.println(WiFi.macAddress());

    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 60) {
      delay(1000);
      i++;
      //WiFi.begin ( ssid, pass );
      Serial.print(".");
      switch (WiFi.status())
      {
      case 0:
        Serial.println("WL_IDLE_STATUS");
        break;
      case 1:
        Serial.println("WL_NP_SSID_AVAIL");
        break;
      case 2:
        Serial.println("WL_SCAN_COMPLETED");
        break;
      case 3:
        Serial.println("WL_CONNECTED");
        break;
      case 4:
        Serial.println("WL_CONNECTED_FAILED");
        break;
      case 5:
        Serial.println("WL_CONNECTED_LOST");
        break;
      case 6:
        Serial.println("WL_DISCONNECTED");
        break;
      }
    }
    Serial.println(" ");
    // Print local IP address and start web server
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else if ( mode == 0 ) {

  } else {

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
void printWiFiStatus ( void )
{
  Serial.println(" ");

  Serial.print("WiFi Status : ");
  switch (WiFi.status())
    {
      case 0:
        Serial.println("WL_IDLE_STATUS");
        break;
      case 1:
        Serial.println("WL_NP_SSID_AVAIL");
        break;
      case 2:
        Serial.println("WL_SCAN_COMPLETED");
        break;
      case 3:
        Serial.println("WL_CONNECTED");
        break;
      case 4:
        Serial.println("WL_CONNECTED_FAILED");
        break;
      case 5:
        Serial.println("WL_CONNECTED_LOST");
        break;
      case 6:
        Serial.println("WL_DISCONNECTED");
        break;
    }
    
  Serial.print("SSID      :");
  Serial.println(WiFi.SSID());

  Serial.print("IP Addres :");
  Serial.println(WiFi.localIP());

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI) :");
  Serial.print(rssi);
  Serial.println(" dBm");
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
void Server_Init ( void )
{
  // URL для корневой страницы веб-сервера:
  // Send a GET request to <IP>/get?message=<message>
  server.on("/",HTTP_GET,[](AsyncWebServerRequest * request){
        //обработчик запросов без BODY
        String action = request->arg("action");

        Serial.println(" GET request -> no Body");
        
        if ( action == "state" ) {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/state.html");
#else
          
#endif    request->send(SD, "/data/state.html");      
          Serial.println(" GET request -> no Body -> action = state");
        } else if ( action == "init") {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/init.html");
#else
          request->send(SD, "/data/init.html");
#endif          
          Serial.println(" GET request -> no Body -> action = init");
        } else if ( action == "features") {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/features.html");
#else
          request->send(SD, "/data/features.html");
#endif          
          Serial.println(" GET request -> no Body -> action = features");
        } else if ( action == "network") {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/network.html");
#else
          request->send(SD, "/data/network.html");
#endif          
          Serial.println(" GET request -> no Body -> action = network");
        } else if ( action == "control") {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/control.html");
#else
          request->send(SD, "/data/control.html");
#endif          
          Serial.println(" GET request -> no Body -> action = control");
        } else if ( action == "access") {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/access.html");
#else
          request->send(SD, "/data/access.html");
#endif          
          Serial.println(" GET request -> no Body -> action = access");
        } else if ( action == "firmware") {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/firmware.html");
#else
          request->send(SD, "/data/firmware.html");
#endif
          Serial.println(" GET request -> no Body -> action = firmware");
        } else if ( action == "handleState") {
          // отдаем json
          StaticJsonDocument<192> doc;
          String response;
          doc["status"] = "102";
          doc["id"] = "243";
          doc["p_in"] = "10";
          doc["p_out"] = "20";
          doc["valve_in"] = "true";
          doc["valve_drain"] = "false";
          doc["pump_rpm"] = "1000";
          JsonArray zones = doc.createNestedArray("zones");
          JsonObject zones_0 = zones.createNestedObject();
          zones_0["num"] = "777";
          zones_0["hum"]["activity"] = "true";
          JsonObject zones_1 = zones.createNestedObject();
          zones_1["num"] = "888";
          zones_1["hum"]["activity"] = "false";

          serializeJson(doc, response);
          request->send(200, "application/json", response);
          serializeJson(doc, Serial);
          Serial.println(" ");
        } else if ( action == "hahdleFeatures" ) {
          // выполнить действие для обработки понунения параметров в МК
          Serial.println(" GET request -> no Body -> handleFeatures");

          // отдаем запродеддде парамет
        } else if ( action == "hahdleNetwork" ) {
          // выполнить действие для обработки пошучения параметров в МК
          Serial.println(" GET request -> no Body -> handleNetwork");

          // отдаем запродеддде парамет
        } else {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/index.html");
#else
          request->send(SD, "/data/index.html");
#endif
        }
    }, NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
        //обработчик запросов с BODY - оно тут в data лежит
        String action = request->arg("action");

        Serial.println(" GET request -> with Body");
  });
  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/",HTTP_POST,[](AsyncWebServerRequest * request){
        //обработчик запросов без BODY
        String action = request->arg("action");

        Serial.println(" POST request -> no Body");
        //
    }, NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
        //обработчик запросов с BODY - оно тут в data лежит
        String action = request->arg("action");
    
        Serial.println(" POST request -> with Body");
    
        if ( action == "handleLogin" ) {
          // выполнить действие для обработки handleState
          Serial.println(" POST -> handleLogin");

          for (size_t i = 0; i < len; i++) {
            Serial.write(data[i]);
          } 
          Serial.println();
          Serial.println("  json read and parser");
          //DynamicJsonBuffer jsonBuffer;
          StaticJsonDocument<96> jsonBuffer;
          DeserializationError error = deserializeJson(jsonBuffer, data);
          if (error) {
            Serial.print("   deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
          } else {
            Serial.print("   deserializeJson() complete : ");
            Serial.println(error.c_str());
          }
          
          String d_login = jsonBuffer["login"];       // "admin"
          String d_password = jsonBuffer["password"]; // "admin"
          Serial.print("   d_login   = ");
          Serial.println( d_login);
          Serial.print("   d_password = ");
          Serial.println( d_password);
          if ( ( d_login == "admin" ) || ( d_password == "admin") ) {
            String response;
            //StaticJsonDocument<128> doc_resp;
            StaticJsonDocument<96> doc_resp;
            doc_resp["data"]["auth_token"] = "Qw123^251Uhd";   // "Qw123^251Uhd"
            doc_resp["data"]["initialization"] = "true";       // "true"
            //doc_resp["data"]["initialization"] = "false";       // "false"
            serializeJson(doc_resp, response);
            serializeJson(doc_resp, Serial);
            Serial.println(" ");
            request->send(200, "application/json", response);
            Serial.println("    login and password ... Ok");
          } else {
            //request->send(401);
            request->send(403);
            Serial.println("    login and password ... Error");
          }
        }
        //
    });

  // Send a PUT request to <IP>/put with a form field message set to <message>
  server.on("/",HTTP_PUT,[](AsyncWebServerRequest * request){
        //обработчик запросов без BODY
        String action = request->arg("action");
    
        Serial.println(" PUT request -> no Body");
        if ( action == "init" ) {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/init.html");
#else
          request->send(SD, "/data/init.html");
#endif          
          Serial.println(" PUT request -> action = init");
        } else if ( action == "access") {
#if CONFIG_FS == CONFIG_SPIFFS
          request->send(SPIFFS, "/access.html");
#else
          request->send(SD, "/data/access.html");
#endif          
          Serial.println(" PUT request -> action = access");
        } else {
          Serial.println(" PUT request -> action = other");
        }
        //
  }, NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
        //обработчик запросов с BODY - оно тут в data лежит
        String action = request->arg("action");
    
        Serial.println(" PUT request -> with Body");

        if ( action == "handleInit" ) {
          // выполнить действие для обработки инициализации
          Serial.println(" PUT -> handleInin");

          for (size_t i = 0; i < len; i++) {
            Serial.write(data[i]);
          } 
        } else if ( action == "handleFeatures" ) {
          // выполнить действие для обработки записи параметров в МК
          Serial.println(" PUT -> handleFeatures");

          for (size_t i = 0; i < len; i++) {
            Serial.write(data[i]);
          }
        } else if ( action == "handleNetwork" ) {
          // выполнить действие для обработки записи параметров в МК
          Serial.println(" PUT -> handleNetwork");

          for (size_t i = 0; i < len; i++) {
            Serial.write(data[i]);
          }
        } else {
          // выполнить другое действие
          Serial.println(" PUT -> other action");
        }
  });
  // URL для файла «style.css»:
  server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
#if CONFIG_FS == CONFIG_SPIFFS
    request->send(SPIFFS, "/css/styles.css", "text/css");
#else
    request->send(SD, "/data/css/styles.css", "text/css");
#endif
  });
  //js
  server.on("/js/scripts.js", HTTP_GET, [](AsyncWebServerRequest *request){
#if CONFIG_FS == CONFIG_SPIFFS
    request->send(SPIFFS, "/js/scripts.js", "text/js");
#else
    request->send(SD, "/data/js/scripts.js", "text/js");
#endif    
  });

  server.begin();
  Serial.println("Server start.");
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
void MQTT_Init ( void )
{
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
  
  if ( !client.connected() ){
    Serial.println("Connecting to MQTT.... ");
    if ( client.connect("ESP32_MQTT") ) {
      Serial.println("connected.... "); 
    }
  } else {
    Serial.println("MQTT Connect .... Error");
  }
  
  //client.setCallback(callback);
  client.setCallback ([]( char *topic, uint8_t *payload, unsigned int len ) {
    Serial.print("MQTT topic receive -> ");
    Serial.print(topic);
    Serial.print(" -> : with value :");
    Serial.println("");
    for( uint16_t i = 0; i < len; i++ ) {
      Serial.print((char)payload[i]);
    }
    Serial.println("");
    Serial.print(" ->  ( ");
    Serial.print(len);
    Serial.println(" bytes ) receyved");
  });
  
  client.subscribe("from_system/243/config/get");
  client.subscribe("from_system/243/zone:399/get");
  client.subscribe("from_system/243/zone:400/get");
  client.subscribe("from_system/243/board:156/get");
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
  Serial.begin(115200);
  delay( 3000 );

  Serial.println();
  esprev     = ESP.getChipRevision();
  espcpufreq = ESP.getCpuFreqMHz();
  espflash   = ESP.getFlashChipSize();
  Serial.print("ESP revision      : ");
  Serial.println(esprev);
  Serial.print("ESP CPU Frequency :");
  Serial.println(espcpufreq);
  Serial.print("ESP Flash size    : ");
  Serial.println(espflash);
  Serial.println("");

#if CONFIG_FS == CONFIG_SPIFFS
  SPIFFS_Init();
#else
  sd_init();
#endif
  
  WiFiAP_Client_Init ( 1 );
  printWiFiStatus ();
  
  Server_Init();
  Serial.println("");
  
  MQTT_Init ();

  mb_init();
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
  //
  client.loop();
  //
  long now = millis();
  if ( now - lastMsg > 5000 ) {
    lastMsg = now;
    
    switch ( Fw_topic )
    {
      case 0:
        client.publish("to_system/243/a_init/get","token : токен");
        Fw_topic = 1;
        break;
      case 1:
        client.publish("to_system/243/zone:399/get","token : ");
        Fw_topic = 2;
        break;
      case 2:
        client.publish("to_system/243/zone:400/get","token : ");
        Fw_topic = 3;
        break;
      case 3:
        client.publish("to_system/243/board:156/get","token : ");
        Fw_topic = 4;
        break;
      case 4:
     
        Fw_topic = 0;
        break;
    }
  }
  //
  //bool isRxPending = m_isrOverflow.exchange(false);
  auto avail = swSer.available ();
  //
}