#include "cfg.h"

/* init cfg module */
void Cfg_Init() {
  EEPROM.begin(EEPROM_SIZE);

  Cfg_ReadCfg();
}

/* read cfg from EEPROM */
void Cfg_ReadCfg() {
  Cfg_LoadRefreshInterval();
  Cfg_LoadToken();
  Cfg_LoadFingerprint();
}

/* save refresh interval to eeprom */
void Cfg_SaveRefreshInterval(uint8_t i_interval) {
  Serial.print("Save RefreshInterval: ");
  Serial.println(i_interval);

  EEPROM.write(EEPROM_ADDR_REFRESH_INTERVAL_START, i_interval);
  EEPROM.commit();
}

/* save token to EEPROM */
void Cfg_SaveToken(String i_token) {
  uint8_t len = i_token.length();
  Serial.print("Save Token[");
  Serial.print(len);
  Serial.print("]: ");
  Serial.println(i_token);

  if (len < EEPROM_ADDR_TOKEN_LENGTH) {
    EEPROM.write(EEPROM_ADDR_TOKEN_START, len);

    for (uint8_t i = EEPROM_ADDR_TOKEN_START + 1, j = 0; j < len; i++, j++) {
      EEPROM.write(i, i_token.charAt(j));
    }
    EEPROM.commit();
    Serial.println("Write done");
  } else {
    Serial.println("Skip write");
  }
}

/* save fingerprint to EEPROM */
void Cfg_SaveFingerprint(String i_fingerprint) {
  uint8_t len = i_fingerprint.length();
  Serial.print("Save Fingerprint[");
  Serial.print(len);
  Serial.print("]: ");
  Serial.println(i_fingerprint);

  if (len < EEPROM_ADDR_FINGERPRINT_LENGTH) {
    EEPROM.write(EEPROM_ADDR_FINGERPRINT_START, len);

    for (uint8_t i = EEPROM_ADDR_FINGERPRINT_START + 1, j = 0; j < len; i++, j++) {
      EEPROM.write(i, i_fingerprint.charAt(j));
    }
    EEPROM.commit();
    Serial.println("Write done");
  } else {
    Serial.println("Skip write");
  }
}

/* load refresh interval from eeprom */
void Cfg_LoadRefreshInterval() {
  RefreshInterval = EEPROM.read(EEPROM_ADDR_REFRESH_INTERVAL_START);
  Serial.print("RefreshInterval: ");
  Serial.println(RefreshInterval);
}

/* load token from eeprom */
void Cfg_LoadToken() {
  Serial.println(EEPROM_SIZE);
  String tmp = "";
  uint8_t len = EEPROM.read(EEPROM_ADDR_TOKEN_START);
  Serial.print("Read token [");
  Serial.print(len);
  Serial.print("]: ");

  if ((len <= EEPROM_ADDR_TOKEN_LENGTH) && (len > 0)){
    for (uint8_t i = EEPROM_ADDR_TOKEN_START + 1, j = 0; j < len; i++, j++) {
      tmp += (char) EEPROM.read(i);
    }
    sToken = tmp;
  }

  Serial.print(tmp);
  Serial.print(" -> ");
  Serial.println(sToken);
}

/* load fingerprint from eeprom */
void Cfg_LoadFingerprint() {
  String tmp = "";
  uint8_t len = EEPROM.read(EEPROM_ADDR_FINGERPRINT_START);
  Serial.print("Read fingerprint [");
  Serial.print(len);
  Serial.print("]: ");

  if ((len <= EEPROM_ADDR_FINGERPRINT_LENGTH) && (len > 0)) {
    for (uint8_t i = EEPROM_ADDR_FINGERPRINT_START + 1, j = 0; j < len; i++, j++) {
      tmp += (char) EEPROM.read(i);
    }
    sFingerprint = tmp;
  }

  Serial.print(tmp);
  Serial.print(" -> ");
  Serial.println(sFingerprint);
}

/* EOF */
