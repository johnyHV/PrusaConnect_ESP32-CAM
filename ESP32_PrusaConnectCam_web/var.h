#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "Arduino.h"
#include "mcu_cfg.h"

extern String photo;                  /* here is saved photo */
extern uint8_t RefreshInterval;       /* here is saved refresh interval */
extern String BackendReceivedData;    /* here is saved response from prusa backend. Maybe is good idea send response to WEB page ? or not ? */

extern String sToken;                 /* token for authentification to prusa backend */
extern String sFingerprint;           /* fingerprint for autentification to prusa backend */

#endif

/* EOF */
