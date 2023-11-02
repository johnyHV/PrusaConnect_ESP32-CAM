/*
   This code is adapted for the ESP32-CAM board from Ai Thinker

   It's neccesary install support for ESP32 board to the arduino IDE. In the board manager we need add next link
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   Then we can install "ESP32 by Espressif Systems" board in the board manager.
   On the internet is too many manuals how to install support ESP32 board on the arduino IDE

   Tools -> Board -> ESP32 Arduino -> AI Thinker ESP32

   Project: ESP32 PrusaConnect Camera
   Author: Miroslav Pivovarsky
   e-mail: miroslav.pivovarsky@gmail.com
   Version 1.1

*/

/* includes */
#include <WiFi.h>
#include "Arduino.h"
#include <esp_task_wdt.h>
#include <ESPmDNS.h>

#include "server.h"
#include "cfg.h"
#include "var.h"
#include "mcu_cfg.h"

/* CFG variable */
/* Replace with your network credentials */
const char* ssid     =      "SSID";
const char* password =      "PASSWORD";

void setup() {
  /* Serial port for debugging purposes */
  Serial.begin(SERIAL_PORT_SPEER);
  Serial.println("Start MCU!");
  Serial.print("SW Version: ");
  Serial.println(SW_VERSION);

  /* read cfg from EEPROM */
  WifiMacAddr = WiFi.macAddress();
  Cfg_Init();

  /* Connect to Wi-Fi */
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  /* Print ESP32 Local IP Address */
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  /* Init MDNS record */
  Serial.println(F("----------------------------------------------------------------"));
  Serial.print("Starting mDNS record: http://");
  Serial.print(MDNS_RECORD_HOST);
  Serial.println(".local");
  if (!MDNS.begin(MDNS_RECORD_HOST)) {
    Serial.println("Error starting mDNS");
  } else {
    Serial.println("Starting mDNS OK");
  }

  /* init camera interface */
  Camera_InitCamera();
  Camera_CapturePhoto();

  /* GPIO cfg */
  pinMode(FLASH_GPIO_NUM, OUTPUT);
  digitalWrite(FLASH_GPIO_NUM, FLASH_STATUS);

  /* init WEB server */
  Server_InitWebServer();

  /* init wdg */
  esp_task_wdt_init(WDT_TIMEOUT, true); /* enable panic so ESP32 restarts */
  esp_task_wdt_add(NULL);               /* add current thread to WDT watch */
  esp_task_wdt_reset();                 /* reset wdg */

  Serial.println("MCU configuration done!");
}

void loop() {
  Serial.println("----------------------------------------------------------------");
  /* check wifi reconnecting */
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
  } else if (WiFi.status() == WL_CONNECTED) {
    char cstr[150];
    sprintf(cstr, "Wifi connected. SSID: %s, RSSI: %d dBm, IP: %s \n", WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.localIP().toString().c_str());
    Serial.printf(cstr);
  }

  /* take photo */
  Camera_CapturePhoto();

  /* send photo to backend */
  Server_SendPhotoToPrusaBackend();

  /* reset wdg */
  for (uint32_t i = 0; i < RefreshInterval; i++) {
    esp_task_wdt_reset();
    delay(1000);
  }
}

/* EOF */
