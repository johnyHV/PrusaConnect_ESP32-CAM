#ifndef _SERVER_H_
#define _SERVER_H_

#include <AsyncEventSource.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebSrv.h>
#include <esp_task_wdt.h>
#include <StringArray.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <EEPROM.h>

//#include "Certificate.h"
#include "WebPage.h"
#include "mcu_cfg.h"
#include "var.h"
#include "camera.h"
#include "cfg.h"

extern AsyncWebServer server;   /* global variable for web server */
#define MSG_SAVE_OK       "<meta http-equiv=\"refresh\" content=\"1; url=/\" /> Save cfg OK"
#define MSQ_SAVE_NOTOK    "<meta http-equiv=\"refresh\" content=\"1; url=/\" /> Save cfg NOT OK"

void Server_InitWebServer();
void Server_handleNotFound(AsyncWebServerRequest *);
void Server_SendPhotoToPrusaBackend();
String Server_GetJsonData();

#endif

/* EOF */
