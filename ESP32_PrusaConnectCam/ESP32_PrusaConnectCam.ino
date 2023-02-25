/*
 * This code is adapted for the ESP32-CAM board from Ai Thinker
 * 
 * It's neccesary install support for ESP32 board to the arduino IDE. In the board manager we need add next link
 * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 * Then we can install "ESP32 by Espressif Systems" board in the board manager. 
 * On the internet is too many manuals how to install support ESP32 board on the arduino IDE
 * 
 * Tools -> Board -> ESP32 Arduino -> AI Thinker ESP32
 * 
 * Project: ESP32 PrusaConnect Camera
 * Author: Miroslav Pivovarsky
 * e-mail: miroslav.pivovarsky@gmail.com
 * Version 1.0
 * 
 */

/* includes */
#include "Arduino.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "soc/soc.h"           /* Disable brownour problems */
#include "soc/rtc_cntl_reg.h"  /* Disable brownour problems */
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <esp_task_wdt.h>

#include "Certificate.h"
#include "Camera_cfg.h"

/* CFG variable */
/* Replace with your network credentials */
const char* ssid     =      "SSID";
const char* password =      "password";

String HOST_URL      =      "https://webcam.connect.prusa3d.com/c/snapshot"; /* Prusa Connect server URL for photo */
String DOMAIN        =      "webcam.connect.prusa3d.com";                    /* prusa connect server */
String TOKEN         =      "token";                          				/* replace with your TOKEN */
String FINGERPRINT   =      "fingerprint";      							/* replace with your fingerprint */

#define PHOTO_FRAME_SIZE    FRAMESIZE_SXGA  /* FRAMESIZE_UXGA (1600 x 1200), FRAMESIZE_SXGA (1280 x 1024), FRAMESIZE_XGA (1024 x 768), FRAMESIZE_SVGA (800 x 600), FRAMESIZE_VGA (640 x 480) */
#define PHOTO_QUALITY       10              /*10-63 lower number means higher quality */
#define PHOTO_VERTICAL_FLIP 1               /* 0 - disable, 1- enable */
#define FLASH_STATUS        LOW             /* enable/disable Flash LED. HIGH - turn ON, LOW - turn off */
  
#define REFRESH_INTERVAL    15      /* refresh interval in the second */
#define SERIAL_PORT_SPEER   115200  /* baud rate */
#define WDT_TIMEOUT         10      /* wdg timeout second */

/* function */
void InitCamera();
void CapturePhoto();
void SendPhoto();

/* global variable */
String photo = "";

void setup() {
  /* Serial port for debugging purposes */
  Serial.begin(SERIAL_PORT_SPEER);

  /* Connect to Wi-Fi */
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");

  /* Print ESP32 Local IP Address */
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  /* init camera interface */
  InitCamera();

  /* GPIO cfg */
  pinMode(FLASH_GPIO_NUM, OUTPUT);
  digitalWrite(FLASH_GPIO_NUM, FLASH_STATUS);

  /* init wdg */
  esp_task_wdt_init(WDT_TIMEOUT, true); /* enable panic so ESP32 restarts */
  esp_task_wdt_add(NULL);               /* add current thread to WDT watch */
  esp_task_wdt_reset();                 /* reset wdg */
}

void loop() {
  CapturePhoto();
  SendPhoto();

  /* reset wdg */
  for (uint32_t i = 0; i < REFRESH_INTERVAL; i++) {
    esp_task_wdt_reset();
    delay(1000);
  }
}

/* Capture Photo and Save it to string array */
void CapturePhoto() {
  camera_fb_t * fb = NULL;

  do {
    Serial.println("Taking a photo...");

    /* get photo */
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
      
    } else {
      /* copy photo from buffer to string array */
      photo = "";
      for (uint32_t i = 0; i < fb->len; i++) {
        photo += (char) fb->buf[i];
      }

      Serial.print("The picture has been saved. ");
      Serial.print(" - Size: ");
      Serial.print(photo.length());
      Serial.println(" bytes");
    }
    esp_camera_fb_return(fb);

    /* check if photo is correctly saved */
  } while ( !( photo.length() > 100));
}

/* Init camera */
void InitCamera() {
  /* Turn-off the 'brownout detector' */
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  /* OV2640 camera module pinout and cfg*/
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; /* YUV422,GRAYSCALE,RGB565,JPEG */

  /* OV2640
    FRAMESIZE_UXGA (1600 x 1200)
    FRAMESIZE_QVGA (320 x 240)
    FRAMESIZE_CIF (352 x 288)
    FRAMESIZE_VGA (640 x 480)
    FRAMESIZE_SVGA (800 x 600)
    FRAMESIZE_XGA (1024 x 768)
    FRAMESIZE_SXGA (1280 x 1024)
  */
  config.frame_size = PHOTO_FRAME_SIZE; /* FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA */
  config.jpeg_quality = PHOTO_QUALITY;  /*10-63 lower number means higher quality */
  config.fb_count = 1;                  /* picture frame buffer alocation */

  /* Camera init */
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed. Error 0x%x", err);
    ESP.restart();
  }

  /* sensor configuration */
  sensor_t * sensor = esp_camera_sensor_get();
  sensor->set_brightness(sensor, 0);                  // -2 to 2
  sensor->set_contrast(sensor, 0);                    // -2 to 2
  sensor->set_saturation(sensor, 0);                  // -2 to 2
  sensor->set_special_effect(sensor, 0);              // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  sensor->set_whitebal(sensor, 1);                    // 0 = disable , 1 = enable
  sensor->set_awb_gain(sensor, 1);                    // 0 = disable , 1 = enable
  sensor->set_wb_mode(sensor, 0);                     // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  sensor->set_exposure_ctrl(sensor, 1);               // 0 = disable , 1 = enable
  sensor->set_aec2(sensor, 0);                        // 0 = disable , 1 = enable
  sensor->set_ae_level(sensor, 0);                    // -2 to 2
  sensor->set_aec_value(sensor, 300);                 // 0 to 1200
  sensor->set_gain_ctrl(sensor, 1);                   // 0 = disable , 1 = enable
  sensor->set_agc_gain(sensor, 0);                    // 0 to 30
  sensor->set_gainceiling(sensor, (gainceiling_t)0);  // 0 to 6
  sensor->set_bpc(sensor, 0);                         // 0 = disable , 1 = enable
  sensor->set_wpc(sensor, 1);                         // 0 = disable , 1 = enable
  sensor->set_raw_gma(sensor, 1);                     // 0 = disable , 1 = enable
  sensor->set_lenc(sensor, 1);                        // 0 = disable , 1 = enable
  sensor->set_hmirror(sensor, 0);                     // 0 = disable , 1 = enable
  sensor->set_vflip(sensor, PHOTO_VERTICAL_FLIP);     // 0 = disable , 1 = enable
  sensor->set_dcw(sensor, 1);                         // 0 = disable , 1 = enable
  sensor->set_colorbar(sensor, 0);                    // 0 = disable , 1 = enable
}

/* send photo to server */
void SendPhoto() {
  WiFiClientSecure client;

  client.setCACert(root_ca);
  Serial.println("\nConnecting to server...");
  if (!client.connect(DOMAIN.c_str(), 443))
    Serial.println("Connection failed!");

  else {
    Serial.println("Connected to server!");
    client.println("PUT " + HOST_URL + " HTTP/1.0");
    client.println("Host: " + DOMAIN);
    client.println("User-Agent: ESP32-CAM");
    client.println("Connection: close");

    client.println("Content-Type: image/jpg");
    client.println("fingerprint: " + FINGERPRINT);
    client.println("token: " + TOKEN);
    client.print("Content-Length: ");
    client.println(photo.length());
    client.println();

    /* photo fragmentation */
    for (int index = 0; index < photo.length(); index = index + 1000) {
      client.print(photo.substring(index, index + 1000));
    }

    Serial.println("Send done!");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }

    Serial.println("Received data: ");
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    Serial.println();

    client.stop();
  }
}

/* EOF */
