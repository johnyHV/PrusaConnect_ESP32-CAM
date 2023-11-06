#ifndef _CFG_H_
#define _CFG_H_

#include "Arduino.h"
#include <EEPROM.h>
#include <ArduinoUniqueID.h>
#include <base64.h>
#include "mcu_cfg.h"
#include "var.h"
#include "esp_camera.h"

void Cfg_Init();
void Cfg_ReadCfg();
void Cfg_DefaultCfg();
void Cfg_GetFingerprint();
bool Cfg_CheckFirstMcuStart();

framesize_t Cfg_TransformFrameSizeDataType(uint8_t);
String Cfg_TransformFrameSizeToString(uint8_t);

void Cfg_SaveRefreshInterval(uint8_t);
void Cfg_SaveAutoPhoto(bool);
void Cfg_SaveToken(String);
void Cfg_SaveFingerprint(String);
void Cfg_SavePhotoQuality(uint8_t);
void Cfg_SaveFrameSize(uint8_t);
void Cfg_SaveBrightness(int8_t);
void Cfg_SaveContrast(int8_t);
void Cfg_SaveSaturation(int8_t);
void Cfg_SaveHmirror(bool);
void Cfg_SaveVflip(bool);
void Cfg_SaveLensCorrect(bool);
void Cfg_SaveExposureCtrl(bool);
void Cfg_SaveFirstMcuStartFlag(uint8_t);
void Cfg_SaveCameraFlash(bool);
void Cfg_SaveCameraFlashDuration(uint16_t);

void Cfg_LoadRefreshInterval();
void Cfg_LoadAutoPhoto();
void Cfg_LoadToken();
void Cfg_LoadFingerprint();
void Cfg_LoadPhotoQuality();
void Cfg_LoadFrameSize();
void Cfg_LoadBrightness();
void Cfg_LoadContrast();
void Cfg_LoadSaturation();
void Cfg_LoadHmirror();
void Cfg_LoadVflip();
void Cfg_LoadLensCorrect();
void Cfg_LoadExposureCtrl();
void Cfg_LoadCameraFlash();
void Cfg_LoadCameraFlashDuration();

#endif

/* EOF */
