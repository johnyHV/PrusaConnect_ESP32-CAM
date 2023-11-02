#include "server.h"
#include "Certificate.h"

AsyncWebServer server(SERVER_PORT);

/* init WEB server */
void Server_InitWebServer() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: Get index.html");
    request->send_P(200, "text/html", index_html);
  });

  /*route for capture photo */
  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: Take photo");
    Camera_CapturePhoto();
    request->send_P(200, "text/plain", "Take Photo");
  });

  /* route for get last capture photo */
  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: get photo");
    request->send_P(200, "image/jpg", (uint8_t*) photo.c_str(), photo.length());
  });

  /* route for send photo to prusa backend */
  server.on("/send", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: send photo to cloud");
    Server_SendPhotoToPrusaBackend();
    request->send_P(200, "text/plain", "Send Photo");
  });

  /* route for change LED status */
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: Change LED status");
    digitalWrite(FLASH_GPIO_NUM, !digitalRead(FLASH_GPIO_NUM));
    request->send_P(200, "text/plain", "Change LED status");
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: Reset configuration to default!");
    Cfg_DefaultCfg();
    request->send_P(200, F("text/html"), MSG_SAVE_OK);
    Camera_Reinit();
  });

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: reboot MCU!");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);
    ESP.restart();
  });

  /* route for set refresh interval */
  server.on("/refresh", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set refresh interval");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("refresh")) {
      RefreshInterval = request->getParam("refresh")->value().toInt();
      Cfg_SaveRefreshInterval(RefreshInterval);
    }
  });

  /* route for set token for authentification to prusa backend*/
  server.on("/token", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set Token");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("token")) {
      sToken = request->getParam("token")->value();
      Cfg_SaveToken(sToken);
    }
  });

  /* route for set photo quality */
  server.on("/photo_quality", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set Fingerprint");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("photo_quality")) {
      CameraCfg.PhotoQuality = request->getParam("photo_quality")->value().toInt();
      Cfg_SavePhotoQuality(CameraCfg.PhotoQuality);
      //Camera_SetCameraCfg();
      Camera_Reinit();
    }
  });

  /* route for set framesize */
  server.on("/framesize", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set framesize");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("framesize")) {
      CameraCfg.FrameSize = request->getParam("framesize")->value().toInt();
      Cfg_SaveFrameSize(CameraCfg.FrameSize);
      //Camera_SetCameraCfg();
      Camera_Reinit();
    }
  });

  /* route for set brightness */
  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set brightness");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("brightness")) {
      CameraCfg.brightness = request->getParam("brightness")->value().toInt();
      Cfg_SaveBrightness(CameraCfg.brightness);
      Camera_SetCameraCfg();
    }
  });

  /* route for set contrast */
  server.on("/contrast", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set contrast");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("contrast")) {
      CameraCfg.contrast = request->getParam("contrast")->value().toInt();
      Cfg_SaveContrast(CameraCfg.contrast);
      Camera_SetCameraCfg();
    }
  });

  /* route for set saturation */
  server.on("/saturation", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set saturation");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("saturation")) {
      CameraCfg.saturation = request->getParam("saturation")->value().toInt();
      Cfg_SaveSaturation(CameraCfg.saturation);
      Camera_SetCameraCfg();
    }
  });

  /* route for change camera flash status */
  server.on("/camera_flash", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: Change camera_flash status");
    CameraCfg.CameraFlashStatus = !CameraCfg.CameraFlashStatus;
    Cfg_SaveCameraFlash(CameraCfg.CameraFlashStatus);
    request->send_P(200, F("text/html"), MSG_SAVE_OK);
  });

  /* route for set flash duration */
  server.on("/camera_flash_duration", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set saturation");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("flashduration")) {
      CameraCfg.CameraFlashDuration = request->getParam("flashduration")->value().toInt();
      Cfg_SaveCameraFlashDuration(CameraCfg.CameraFlashDuration);
    }
  });

  /* route for set actions */
  server.on("/action_page", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set saturation");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    /* check cfg for hmirror */
    if (request->hasParam("hmirror")) {
      CameraCfg.hmirror = request->getParam("hmirror")->value().toInt();
    } else {
      CameraCfg.hmirror = 0;
    }
    Cfg_SaveHmirror(CameraCfg.hmirror);

    /* check cfg for vflip */
    if (request->hasParam("vflip")) {
      CameraCfg.vflip = request->getParam("vflip")->value().toInt();
    } else {
      CameraCfg.vflip = 0;
    }
    Cfg_SaveVflip(CameraCfg.vflip);

    /* check cfg for lensc */
    if (request->hasParam("lenc")) {
      CameraCfg.lensc = request->getParam("lenc")->value().toInt();
    } else {
      CameraCfg.lensc = 0;
    }
    Cfg_SaveLensCorrect(CameraCfg.lensc);

    /* check cfg for exposure_ctrl */
    if (request->hasParam("exposure_ctrl")) {
      CameraCfg.exposure_ctrl = request->getParam("exposure_ctrl")->value().toInt();
    } else {
      CameraCfg.exposure_ctrl = 0;
    }

    /* save cfg */
    Cfg_SaveExposureCtrl(CameraCfg.exposure_ctrl);
    /* set parameters on the camera */
    Camera_SetCameraCfg();
  });

  /* route for json with cfg parameters */
  server.on("/json_input", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: get json_input");
    request->send_P(200, F("text/html"), Server_GetJsonData().c_str());
  });

  /* route for not found page */
  server.onNotFound(Server_handleNotFound);

  /* start WEB server */
  server.begin();
}

/* if the page was not found on ESP, then print which page is not there */
void Server_handleNotFound(AsyncWebServerRequest * request) {
  String message = "URL not Found\n\n";

  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++) {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  request->send(404, "text/plain", message);
}

/* send photo to prusa backend server */
void Server_SendPhotoToPrusaBackend() {
  WiFiClientSecure client;

  /* check fingerprint and token length */
  if ((sFingerprint.length() > 0) && (sToken.length() > 0)) {
    client.setCACert(root_ca);
    Serial.println("\nConnecting to server...");

    /* connecting to server */
    if (!client.connect(DOMAIN, 443)) {
      Serial.println("Connection failed!");

    } else {
      /* send data to server */
      Serial.println("Connected to server!");
      client.println("PUT " + String(HOST_URL) + " HTTP/1.0");
      client.println("Host: " + String(DOMAIN));
      client.println("User-Agent: ESP32-CAM");
      client.println("Connection: close");

      client.println("Content-Type: image/jpg");
      client.println("fingerprint: " + sFingerprint);
      client.println("token: " + sToken);
      client.print("Content-Length: ");
      client.println(photo.length());
      client.println();

      /* photo fragmentation */
      esp_task_wdt_reset();
      for (int index = 0; index < photo.length(); index = index + PHOTO_FRAGMENT_SIZE) {
        client.print(photo.substring(index, index + PHOTO_FRAGMENT_SIZE));
        Serial.println(index);
      }
      Serial.println("Send done!");
      esp_task_wdt_reset();

      /* check response header */
      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.print("Headers received: ");
          Serial.println(line);
          break;
        }
      }

      /* check response data */
      Serial.print("Received data[");
      BackendReceivedData = "";
      while (client.available()) {
        BackendReceivedData += (char) client.read();
      }
      Serial.print(BackendReceivedData.length());
      Serial.print("]: ");
      Serial.println(BackendReceivedData);

      client.stop();
    }
  } else {
    /* err message */
    Serial.println("ERROR SEND PICTURE TO SERVER! INVALID DATA!");
    Serial.print("Fingerprint: ");
    Serial.println(sFingerprint);
    Serial.print("Token: ");
    Serial.println(sToken);
    if (sFingerprint.length() == 0) {
      Serial.println("INVALID FINGERPRINT DATA!");
    } else if (sToken.length() == 0) {
      Serial.println("INVALID TOKEN DATA!");
    }
  }
}

/* make json data for WEB page on the ESP32*/
String Server_GetJsonData() {
  String data = {""};
  data = "{\"token\" : \"";
  data += sToken;
  data += "\", ";

  data += "\"fingerprint\" : \"";
  data += sFingerprint;
  data += "\", ";

  data += "\"refreshInterval\" : \"";
  data += String(RefreshInterval);
  data += "\", ";

  data += "\"photoquality\" : \"";
  data += String(CameraCfg.PhotoQuality);
  data += "\", ";

  data += "\"framesize\" : \"";
  data += Cfg_TransformFrameSizeToString(CameraCfg.FrameSize);
  data += "\", ";

  data += "\"brightness\" : \"";
  data += String(CameraCfg.brightness);
  data += "\", ";

  data += "\"contrast\" : \"";
  data += String(CameraCfg.contrast);
  data += "\", ";

  data += "\"saturation\" : \"";
  data += String(CameraCfg.saturation);
  data += "\", ";

  data += "\"hmirror\" : \"";
  data += (CameraCfg.hmirror == 1) ? "true" : "false";
  data += "\", ";

  data += "\"vflip\" : \"";
  data += (CameraCfg.vflip == 1) ? "true" : "false";
  data += "\", ";

  data += "\"lensc\" : \"";
  data += (CameraCfg.lensc == 1) ? "true" : "false";
  data += "\", ";

  data += "\"exposure_ctrl\" : \"";
  data += (CameraCfg.exposure_ctrl == 1) ? "true" : "false";
  data += "\", ";

  bool led = digitalRead(FLASH_GPIO_NUM);
  data += "\"led\" : \"";
  data += (led == true) ? "ON" : "OFF";
  data += "\", ";

  data += "\"camera_flash\" : \"";
  data += (CameraCfg.CameraFlashStatus == 1) ? "true" : "false";
  data += "\", ";

  data += "\"camera_flash_duration\" : \"";
  data += String(CameraCfg.CameraFlashDuration);
  data += "\", ";

  data += "\"rssi\" : \"";
  data += String(WiFi.RSSI());
  data += "\", ";

  data += "\"sw_ver\" : \"";
  data += SW_VERSION;
  data += "\" }";

  return data;
}

/* EOF */
