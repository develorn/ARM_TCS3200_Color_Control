#ifndef __TCS3200
#define __TCS3200

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"


/************************* PIN CONFIGURATION ****************************/
//Pins
#define TCS3200_GPIO			GPIOA
#define TCS3200_S0				GPIO_Pin_2
#define TCS3200_S1				GPIO_Pin_3
#define TCS3200_S2 				GPIO_Pin_4
#define TCS3200_S3 				GPIO_Pin_5
#define TCS3200_OE				GPIO_Pin_6
#define TCS3200_OUT				GPIO_Pin_0
//NVIC & EXTI & TIM
#define TCS3200_EXTI_IRQn		EXTI0_IRQn
#define TCS3200_EXTI_LN			EXTI_Line0
#define TCS3200_PORT_SOURCE		GPIO_PortSourceGPIOA
#define TCS3200_PIN_SOURCE		GPIO_PinSource0

#define TCS3200_TIM_CLK()		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE)
#define TCS3200_TIM				TIM2
#define TCS3200_TIM_IRQn		TIM2_IRQn

/***************************** DEFINES *********************************/

/***************************** Typedefs ********************************/
typedef struct{
	unsigned int Red;
	unsigned int Green;
	unsigned int Blue;
	unsigned int Clear;
}TCS3200_COLOR;

typedef enum  {F2p, F20p, F100p}Freg_Scaling; 									  // Select output frequency scaling
typedef enum  {Red, Green, Blue, Clear}Filter; 									  // Select color

/******************** GLOBAL VARIABLES ********************************/

/******************** GLOBAL FUNCTIONS ********************************/

void TCS3200_Init(void);

TCS3200_COLOR TCS3200_GET_ALL_COLOR(void);						  				  // Return values of all color sensors : Red/Green/Blue/Clear
unsigned int TCS3200_GET_ONE_COLOR(void); 										  // Return one selected color

void TCS3200_MEASURE_ALL_COLOR_START(const Freg_Scaling value); 		                    // Start measure from all color sensors
void TCS3200_MEASURE_ONE_COLOR_START(const Freg_Scaling value, const Filter select_color);    // Start measure one color

inline uint8_t TCS3200_FLAG_READY_ALL(void);				                      // Ready to use function GET_ALL_COLOR
inline uint8_t TCS3200_FLAG_READY_ONE(void);							          // Ready to use function GET_ONE_COLOR
inline void TCS3200_FLAG_CLEAR_ALL(void);										  // Clear flag when used ALL_COLOR_START
inline void TCS3200_FLAG_CLEAR_ONE(void);										  // Clear flag when used ONE_COLOR_START

/****************************************************************/

#ifdef __cplusplus
}
#endif

#endif


