#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "Arduino.h"
#include "mcu_cfg.h"

extern String photo;                  /* here is saved photo */
extern uint8_t RefreshInterval;       /* here is saved refresh interval */
extern String BackendReceivedData;    /* here is saved response from prusa backend. Maybe is good idea send response to WEB page ? or not ? */

extern String sToken;                 /* token for authentification to prusa backend */
extern String sFingerprint;           /* fingerprint for autentification to prusa backend */
extern String WifiMacAddr;            /* WiFi MAC address */

struct CameraCfg_struct {
  uint8_t PhotoQuality;
  uint8_t FrameSize;
  int8_t brightness;
  int8_t contrast;
  int8_t saturation;

  bool hmirror;
  bool vflip;
  bool lensc;
  bool exposure_ctrl;

  bool CameraFlashStatus;
  uint16_t CameraFlashDuration;
};

extern struct CameraCfg_struct CameraCfg;

#endif

/* EOF */
