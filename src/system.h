#ifndef _SYSTEM
#define _SYSTEM

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_flash.h"
#include "Delay.h"

#include "lcd_hd44780_lib.h"
#include "tcs3200.h"



void TimerInit();
void NvicInit();
void PeripheralInit(void);
void RccInit(void);
void GPIOInit();

#ifdef __cplusplus
}
#endif

#endif
