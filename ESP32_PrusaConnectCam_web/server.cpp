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

  /* route for set fingerprint for authentification to prusa backend */
  server.on("/fingerprint", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("WEB server: set Fingerprint");
    request->send_P(200, F("text/html"), MSG_SAVE_OK);

    if (request->hasParam("fingerprint")) {
      sFingerprint = request->getParam("fingerprint")->value();
      Cfg_SaveFingerprint(sFingerprint);
    }
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
  data += "\"sw_ver\" : \"";
  data += SW_VERSION;
  data += "\" }";

  return data;
}

/* EOF */
