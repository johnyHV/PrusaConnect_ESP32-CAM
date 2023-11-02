#ifndef _MCU_CFG_H_
#define _MCU_CFG_H_

#define SW_VERSION         "1.1.6"

#define HOST_URL           "https://webcam.connect.prusa3d.com/c/snapshot" /* Prusa Connect server URL for photo */
#define DOMAIN             "webcam.connect.prusa3d.com"                    /* prusa connect server */

#define FLASH_STATUS        LOW             /* enable/disable Flash LED. HIGH - turn ON, LOW - turn off */

#define SERVER_PORT         80              /* WEB server port */
#define MDNS_RECORD_HOST    "prusa-esp32cam"/* mdns record http://MDNS_RECORD_HOST.local */
#define SERIAL_PORT_SPEER   115200          /* baud rate */
#define WDT_TIMEOUT         50              /* wdg timeout second */
#define PHOTO_FRAGMENT_SIZE 9000            /* photo fragmentation size */

/* ---------------- CFG FLAGS  ----------------*/
#define CFG_FIRST_MCU_START_ACK               0xFF    ///< flag first MCU start ACK -> yes, it's first mcu start
#define CFG_FIRST_MCU_START_NAK               0x0B    ///< flag first MCU start NAK -> no, it's not first MCU start

/* ---------------- EEPROM CFG ----------------*/

#define EEPROM_ADDR_REFRESH_INTERVAL_START    0     /* whre is stored first byte from refresh data */
#define EEPROM_ADDR_REFRESH_INTERVAL_LENGTH   1     /* how long is the refresh data variable stored in the eeprom [bytes] */

#define EEPROM_ADDR_FINGERPRINT_START         (EEPROM_ADDR_REFRESH_INTERVAL_START + EEPROM_ADDR_REFRESH_INTERVAL_LENGTH)    /* where is stored first byte from fingerprint */
#define EEPROM_ADDR_FINGERPRINT_LENGTH        80                                                                            /* how long is fingerprint [bytes] */

#define EEPROM_ADDR_TOKEN_START               (EEPROM_ADDR_FINGERPRINT_START + EEPROM_ADDR_FINGERPRINT_LENGTH)              /* where is stored first byte from token */
#define EEPROM_ADDR_TOKEN_LENGTH              40                                                                            /* how long is token [bytes] */

#define EEPROM_ADDR_FRAMESIZE_START           (EEPROM_ADDR_TOKEN_START + EEPROM_ADDR_TOKEN_LENGTH)
#define EEPROM_ADDR_FRAMESIZE_LENGTH          1

#define EEPROM_ADDR_BRIGHTNESS_START          (EEPROM_ADDR_FRAMESIZE_START + EEPROM_ADDR_FRAMESIZE_LENGTH)
#define EEPROM_ADDR_BRIGHTNESS_LENGTH         1

#define EEPROM_ADDR_CONTRAST_START            (EEPROM_ADDR_BRIGHTNESS_START + EEPROM_ADDR_BRIGHTNESS_LENGTH)
#define EEPROM_ADDR_CONTRAST_LENGTH           1

#define EEPROM_ADDR_SATURATION_START          (EEPROM_ADDR_CONTRAST_START + EEPROM_ADDR_CONTRAST_LENGTH)
#define EEPROM_ADDR_SATURATION_LENGTH         1

#define EEPROM_ADDR_HMIRROR_START             (EEPROM_ADDR_SATURATION_START + EEPROM_ADDR_SATURATION_LENGTH)
#define EEPROM_ADDR_HMIRROR_LENGTH            1

#define EEPROM_ADDR_VFLIP_START               (EEPROM_ADDR_HMIRROR_START + EEPROM_ADDR_HMIRROR_LENGTH)
#define EEPROM_ADDR_VFLIP_LENGTH              1

#define EEPROM_ADDR_LENSC_START               (EEPROM_ADDR_VFLIP_START + EEPROM_ADDR_VFLIP_LENGTH)
#define EEPROM_ADDR_LENSC_LENGTH              1

#define EEPROM_ADDR_EXPOSURE_CTRL_START       (EEPROM_ADDR_LENSC_START + EEPROM_ADDR_LENSC_LENGTH)
#define EEPROM_ADDR_EXPOSURE_CTRL_LENGTH      1

#define EEPROM_ADDR_PHOTO_QUALITY_START       (EEPROM_ADDR_EXPOSURE_CTRL_START + EEPROM_ADDR_EXPOSURE_CTRL_LENGTH)
#define EEPROM_ADDR_PHOTO_QUALITY_LENGTH      1

#define EEPROM_ADDR_FIRST_MCU_START_FLAG_START    (EEPROM_ADDR_PHOTO_QUALITY_START + EEPROM_ADDR_PHOTO_QUALITY_LENGTH)  
#define EEPROM_ADDR_FIRST_MCU_START_FLAG_LENGTH   1                                                                                       

#define EEPROM_ADDR_CAMERA_FLASH_FUNCTION         (EEPROM_ADDR_FIRST_MCU_START_FLAG_START + EEPROM_ADDR_FIRST_MCU_START_FLAG_LENGTH)
#define EEPROM_ADDR_CAMERA_FLASH_FUNCTION_LENGTH  1

#define EEPROM_ADDR_CAMERA_FLASH_DURATION         (EEPROM_ADDR_CAMERA_FLASH_FUNCTION + EEPROM_ADDR_CAMERA_FLASH_FUNCTION_LENGTH)
#define EEPROM_ADDR_CAMERA_FLASH_DURATION_LENGTH  2

#define EEPROM_SIZE                           (EEPROM_ADDR_REFRESH_INTERVAL_LENGTH + EEPROM_ADDR_FINGERPRINT_LENGTH + EEPROM_ADDR_TOKEN_LENGTH + EEPROM_ADDR_FRAMESIZE_LENGTH + EEPROM_ADDR_BRIGHTNESS_LENGTH + EEPROM_ADDR_CONTRAST_LENGTH + EEPROM_ADDR_SATURATION_LENGTH + EEPROM_ADDR_HMIRROR_LENGTH + EEPROM_ADDR_VFLIP_LENGTH + EEPROM_ADDR_LENSC_LENGTH + EEPROM_ADDR_EXPOSURE_CTRL_LENGTH + EEPROM_ADDR_PHOTO_QUALITY_LENGTH + EEPROM_ADDR_FIRST_MCU_START_FLAG_LENGTH + EEPROM_ADDR_CAMERA_FLASH_FUNCTION_LENGTH + EEPROM_ADDR_CAMERA_FLASH_DURATION_LENGTH) /* how many bits do we need for eeprom memory */

#endif

/* EOF */
