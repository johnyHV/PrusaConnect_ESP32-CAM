#ifndef _MCU_CFG_H_
#define _MCU_CFG_H_

#define SW_VERSION         "1.1.1"

#define HOST_URL           "https://webcam.connect.prusa3d.com/c/snapshot" /* Prusa Connect server URL for photo */
#define DOMAIN             "webcam.connect.prusa3d.com"                    /* prusa connect server */

#define PHOTO_FRAME_SIZE    FRAMESIZE_SXGA  /* FRAMESIZE_UXGA (1600 x 1200), FRAMESIZE_SXGA (1280 x 1024), FRAMESIZE_XGA (1024 x 768), FRAMESIZE_SVGA (800 x 600), FRAMESIZE_VGA (640 x 480) */
#define PHOTO_QUALITY       10              /*10-63 lower number means higher quality */
#define PHOTO_VERTICAL_FLIP 0               /* 0 - disable, 1- enable */
#define FLASH_STATUS        LOW             /* enable/disable Flash LED. HIGH - turn ON, LOW - turn off */

#define SERVER_PORT         80              /* WEB server port */
#define SERIAL_PORT_SPEER   115200          /* baud rate */
#define WDT_TIMEOUT         50              /* wdg timeout second */
#define PHOTO_FRAGMENT_SIZE 9000            /* photo fragmentation size */

#endif

/* EOF */
