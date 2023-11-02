#include "cfg.h"

/* init cfg module */
void Cfg_Init() {
  Serial.print("Init cfg module [");
  Serial.print(EEPROM_SIZE);
  Serial.println("]");
  EEPROM.begin(EEPROM_SIZE);

  /* check, when it is first MCU start. If yes, then set default CFG */
  if (Cfg_CheckFirstMcuStart() == true) {
    Cfg_SaveFirstMcuStartFlag(CFG_FIRST_MCU_START_NAK);
    Cfg_DefaultCfg();
  }

  /* read cfg from eeprom */
  Cfg_ReadCfg();
}

/* read cfg from EEPROM */
void Cfg_ReadCfg() {
  Cfg_LoadRefreshInterval();
  Cfg_LoadToken();
  Cfg_LoadFingerprint();
  Cfg_LoadPhotoQuality();
  Cfg_LoadFrameSize();
  Cfg_LoadBrightness();
  Cfg_LoadContrast();
  Cfg_LoadSaturation();
  Cfg_LoadHmirror();
  Cfg_LoadVflip();
  Cfg_LoadLensCorrect();
  Cfg_LoadExposureCtrl();
  Cfg_LoadCameraFlash();
  Cfg_LoadCameraFlashDuration();
}

/* set default cfg */
void Cfg_DefaultCfg() {
  Serial.println("Set default configuration!");

  Cfg_SaveRefreshInterval(10);
  Cfg_SaveToken("");
  Cfg_GetFingerprint();
  Cfg_SavePhotoQuality(20);
  Cfg_SaveFrameSize(0);
  Cfg_SaveBrightness(0);
  Cfg_SaveContrast(0);
  Cfg_SaveSaturation(0);
  Cfg_SaveHmirror(0);
  Cfg_SaveVflip(0);
  Cfg_SaveLensCorrect(1);
  Cfg_SaveExposureCtrl(1);
  Cfg_SaveCameraFlash(false);
  Cfg_SaveCameraFlashDuration(200);
}

void Cfg_GetFingerprint() {
  String Id = "";
  for (size_t i = 0; i < UniqueIDsize; i++) {
    Id += String(UniqueID[i]);
  }
  String encoded = base64::encode(Id + " " + WifiMacAddr);

  Cfg_SaveFingerprint(encoded);

  Serial.print("UniqueID: ");
  Serial.println(Id);

  Serial.print("WiFi MAC: ");
  Serial.println(WifiMacAddr);

  Serial.print("Encoded: ");
  Serial.println(encoded);
}

/* Function for check if it's first MCU start */
bool Cfg_CheckFirstMcuStart() {
  Serial.print("Read FirstMcuStart: ");
  uint8_t flag = EEPROM.read(EEPROM_ADDR_FIRST_MCU_START_FLAG_START);
  Serial.print(flag);

  if (CFG_FIRST_MCU_START_NAK == flag) {
    Serial.println(" - FALSE!");
    return false;
  } else {
    Serial.println(" - TRUE!");
    return true;
  }

  return false;
}

/* transform uint8_t from web interface to framesize_t */
framesize_t Cfg_TransformFrameSizeDataType(uint8_t i_data) {
  if ((i_data >= 0) && (i_data <= 6)) {
    switch (i_data) {
      case 0:
        return FRAMESIZE_QVGA;
      case 1:
        return FRAMESIZE_CIF;
      case 2:
        return FRAMESIZE_VGA;
      case 3:
        return FRAMESIZE_SVGA;
      case 4:
        return FRAMESIZE_XGA;
      case 5:
        return FRAMESIZE_SXGA;
      case 6:
        return FRAMESIZE_UXGA;
    }
  } else {
    return FRAMESIZE_QVGA;
  }

  return FRAMESIZE_QVGA;
}

/* transform uint8_t from web interface to string */
String Cfg_TransformFrameSizeToString(uint8_t i_data) {
  if ((i_data >= 0) && (i_data <= 6)) {
    switch (i_data) {
      case 0:
        return "QVGA (320 x 240)";
      case 1:
        return "CIF (352 x 288)";
      case 2:
        return "VGA (640 x 480)";
      case 3:
        return "SVGA (800 x 600)";
      case 4:
        return "XGA (1024 x 768)";
      case 5:
        return "SXGA (1280 x 1024)";
      case 6:
        return "UXGA (1600 x 1200)";
    }
  } else {
    return "QVGA (320 x 240)";
  }

  return "QVGA (320 x 240)";
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

void Cfg_SavePhotoQuality(uint8_t i_data) {
  Serial.print("Save PhotoQuality: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_PHOTO_QUALITY_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveFrameSize(uint8_t i_data) {
  Serial.print("Save FrameSize: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_FRAMESIZE_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveBrightness(int8_t i_data) {
  Serial.print("Save Brightness: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_BRIGHTNESS_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveContrast(int8_t i_data) {
  Serial.print("Save Contrast: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_CONTRAST_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveSaturation(int8_t i_data) {
  Serial.print("Save Saturation: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_SATURATION_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveHmirror(bool i_data) {
  Serial.print("Save Hmirror: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_HMIRROR_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveVflip(bool i_data) {
  Serial.print("Save vflip: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_VFLIP_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveLensCorrect(bool i_data) {
  Serial.print("Save lensc: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_LENSC_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveExposureCtrl(bool i_data) {
  Serial.print("Save exposure_ctrl: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_EXPOSURE_CTRL_START, i_data);
  EEPROM.commit();
}

/* save flag about first MCU start. for settings basic auth and more */
void Cfg_SaveFirstMcuStartFlag(uint8_t i_data) {
  Serial.print("Save first MCU start flag: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_FIRST_MCU_START_FLAG_START, i_data);
  EEPROM.commit();
}

void Cfg_SaveCameraFlash(bool i_data) {
  Serial.print("Save camera flash function status: ");
  Serial.println(i_data);

  EEPROM.write(EEPROM_ADDR_CAMERA_FLASH_FUNCTION, i_data);
  EEPROM.commit();
}

void Cfg_SaveCameraFlashDuration(uint16_t i_data) {
  Serial.print("Save camera flash duration: ");
  Serial.println(i_data);
  uint8_t highByte = highByte(i_data);
  uint8_t lowByte = lowByte(i_data);

  EEPROM.write(EEPROM_ADDR_CAMERA_FLASH_DURATION, highByte);
  EEPROM.write(EEPROM_ADDR_CAMERA_FLASH_DURATION + 1, lowByte);
  EEPROM.commit();
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

  if ((len <= EEPROM_ADDR_TOKEN_LENGTH) && (len > 0)) {
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

void Cfg_LoadPhotoQuality() {
  CameraCfg.PhotoQuality = EEPROM.read(EEPROM_ADDR_PHOTO_QUALITY_START);
  Serial.print("PhotoQuality: ");
  Serial.println(CameraCfg.PhotoQuality);
}

void Cfg_LoadFrameSize() {
  CameraCfg.FrameSize = EEPROM.read(EEPROM_ADDR_FRAMESIZE_START);
  Serial.print("FrameSize: ");
  Serial.println(CameraCfg.FrameSize);
}

void Cfg_LoadBrightness() {
  CameraCfg.brightness = EEPROM.read(EEPROM_ADDR_BRIGHTNESS_START);
  Serial.print("brightness: ");
  Serial.println(CameraCfg.brightness);
}

void Cfg_LoadContrast() {
  CameraCfg.contrast = EEPROM.read(EEPROM_ADDR_CONTRAST_START);
  Serial.print("contrast: ");
  Serial.println(CameraCfg.contrast);
}

void Cfg_LoadSaturation() {
  CameraCfg.saturation = EEPROM.read(EEPROM_ADDR_SATURATION_START);
  Serial.print("saturation: ");
  Serial.println(CameraCfg.saturation);
}

void Cfg_LoadHmirror() {
  CameraCfg.hmirror = EEPROM.read(EEPROM_ADDR_HMIRROR_START);
  Serial.print("hmirror: ");
  Serial.println(CameraCfg.hmirror);
}

void Cfg_LoadVflip() {
  CameraCfg.vflip = EEPROM.read(EEPROM_ADDR_VFLIP_START);
  Serial.print("vflip: ");
  Serial.println(CameraCfg.vflip);
}

void Cfg_LoadLensCorrect() {
  CameraCfg.lensc = EEPROM.read(EEPROM_ADDR_LENSC_START);
  Serial.print("lensc: ");
  Serial.println(CameraCfg.lensc);
}

void Cfg_LoadExposureCtrl() {
  CameraCfg.exposure_ctrl = EEPROM.read(EEPROM_ADDR_EXPOSURE_CTRL_START);
  Serial.print("exposure_ctrl: ");
  Serial.println(CameraCfg.exposure_ctrl);
}

void Cfg_LoadCameraFlash() {
  CameraCfg.CameraFlashStatus = EEPROM.read(EEPROM_ADDR_CAMERA_FLASH_FUNCTION);
  Serial.print("Camera flash function: ");
  Serial.println(CameraCfg.CameraFlashStatus);
}

void Cfg_LoadCameraFlashDuration() {
  CameraCfg.CameraFlashDuration = uint16_t(EEPROM.read(EEPROM_ADDR_CAMERA_FLASH_DURATION) << 8) | (EEPROM.read(EEPROM_ADDR_CAMERA_FLASH_DURATION + 1));
  Serial.print("Camera flash duration: ");
  Serial.println(CameraCfg.CameraFlashDuration);
}

/* EOF */
