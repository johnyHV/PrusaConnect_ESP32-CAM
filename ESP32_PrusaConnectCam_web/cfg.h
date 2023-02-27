#ifndef _CFG_H_
#define _CFG_H_

#include "Arduino.h"
#include <EEPROM.h>
#include "mcu_cfg.h"
#include "var.h"

/* eeprom address configuration */
#define EEPROM_ADDR_REFRESH_INTERVAL_START    0     /* whre is stored first byte from refresh data */
#define EEPROM_ADDR_REFRESH_INTERVAL_LENGTH   1     /* how long is the refresh data variable stored in the eeprom [bytes] */

#define EEPROM_ADDR_FINGERPRINT_START         (EEPROM_ADDR_REFRESH_INTERVAL_START + EEPROM_ADDR_REFRESH_INTERVAL_LENGTH)    /* where is stored first byte from fingerprint */
#define EEPROM_ADDR_FINGERPRINT_LENGTH        50                                                                            /* how long is fingerprint [bytes] */

#define EEPROM_ADDR_TOKEN_START               (EEPROM_ADDR_FINGERPRINT_START + EEPROM_ADDR_FINGERPRINT_LENGTH)              /* where is stored first byte from token */
#define EEPROM_ADDR_TOKEN_LENGTH              30                                                                            /* how long is token [bytes] */

#define EEPROM_SIZE                           (EEPROM_ADDR_REFRESH_INTERVAL_LENGTH + EEPROM_ADDR_FINGERPRINT_LENGTH + EEPROM_ADDR_TOKEN_LENGTH) /* how many bits do we need for eeprom memory */

void Cfg_Init();
void Cfg_ReadCfg();

void Cfg_SaveRefreshInterval(uint8_t);
void Cfg_SaveToken(String);
void Cfg_SaveFingerprint(String);

void Cfg_LoadRefreshInterval();
void Cfg_LoadToken();
void Cfg_LoadFingerprint();

#endif

/* EOF */
