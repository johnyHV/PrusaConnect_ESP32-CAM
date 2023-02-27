#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "soc/soc.h"           /* Disable brownour problems */
#include "soc/rtc_cntl_reg.h"  /* Disable brownour problems */


#include "Camera_cfg.h"
#include "Arduino.h"
#include "mcu_cfg.h"
#include "var.h"

void Camera_InitCamera();
void Camera_CapturePhoto();

#endif

/* EOF */
