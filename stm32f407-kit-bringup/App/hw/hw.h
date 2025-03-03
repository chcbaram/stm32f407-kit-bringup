#ifndef HW_H_
#define HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#include "led.h"
#include "uart.h"
#include "cli.h"
#include "i2c.h"
#include "eeprom.h"
#include "gpio.h"
#include "spi.h"
#include "spi_flash.h"
#include "button.h"
#include "rtc.h"
#include "buzzer.h"
#include "sd.h"
#include "lcd.h"
#include "ws2812.h"
#include "can.h"


bool hwInit(void);


#ifdef __cplusplus
}
#endif

#endif
