#include "wifi_control.h"

/* Connect to the wifi network */
void Wifi_Connect(const char *ssid, const char *password) {

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

}

void Wifi_Reconnect(const char *ssid, const char *password) {
  Serial.println("***** WiFi restart *****");
    WiFi.disconnect();
		MDNS.end();
    Wifi_Connect(ssid, password);
}
