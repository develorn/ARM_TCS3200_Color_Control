#include "tcs3200.h"

/************** LOCAL VARIABLES **************************/

unsigned int measure_red;
unsigned int measure_green;
unsigned int measure_blue;
unsigned int measure_clear;
unsigned int measure_one;
volatile unsigned int counter;
volatile uint8_t Start = 0;
volatile uint8_t FLAG_READY = 0;
volatile uint8_t FLAG_READY1 = 0;
uint8_t Check = 1;
uint8_t Check1 = 1;
uint8_t Check2 = 1;
uint8_t m_one = 0;

Filter Select_Filter = Red;

/************** LOCAL FUNCTION **************************/

void TCS3200_NVIC_INIT (void);
void TCS3200_EXTI_INIT(void);
void TCS3200_TIM_INIT(void);
inline void TCS3200_ON(void);
inline void TCS3200_OFF(void);
inline void TCS3200_LED_RED(void);
inline void TCS3200_LED_BLUE(void);
inline void TCS3200_LED_GREEN(void);
inline void TCS3200_LED_CLEAR(void);
inline void TCS3200_FREQ_2p(void);
inline void TCS3200_FREQ_20p(void);
inline void TCS3200_FREQ_100p(void);
inline void TCS3200_FREQ_POWER_DOWN(void);

/*********************************************************/

void TCS3200_Init(void)
{
	TCS3200_TIM_CLK();

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin   =  TCS3200_S0|TCS3200_S1|TCS3200_S2|TCS3200_S3|TCS3200_OE;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;

	GPIO_Init(TCS3200_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   =  TCS3200_OUT;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING;

	GPIO_Init(TCS3200_GPIO, &GPIO_InitStructure);

	TCS3200_NVIC_INIT();
	TCS3200_EXTI_INIT();
	TCS3200_TIM_INIT();

	TCS3200_OFF();
	TCS3200_FREQ_POWER_DOWN();

}
void TCS3200_NVIC_INIT (void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/*********** EXTI ************************/

	NVIC_InitStructure.NVIC_IRQChannel = TCS3200_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_EXTILineConfig(TCS3200_PORT_SOURCE, TCS3200_PIN_SOURCE);

	/*********** TIM **************************/

	NVIC_InitStructure.NVIC_IRQChannel = TCS3200_TIM_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}

void TCS3200_EXTI_INIT(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;

		//Konfiguracja przerwania EXTI0 na linie 0
	EXTI_InitStructure.EXTI_Line = TCS3200_EXTI_LN;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void TCS3200_TIM_INIT(void)
{

	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	//System Timer
	TIM_InitStruct.TIM_ClockDivision = 0;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 100-1; //10Hz - 100ms
	TIM_InitStruct.TIM_Prescaler = 24000-1;
	TIM_TimeBaseInit(TCS3200_TIM, &TIM_InitStruct);

	TIM_ClearFlag( TCS3200_TIM, TIM_FLAG_Update );
	TIM_ITConfig(TCS3200_TIM, TIM_IT_Update, ENABLE);
	TIM_Cmd(TCS3200_TIM, DISABLE);
}

/******************************/
inline void TCS3200_ON(void)
{
	GPIO_ResetBits(TCS3200_GPIO,TCS3200_OE);
}
inline void TCS3200_OFF(void)
{
	GPIO_SetBits(TCS3200_GPIO,TCS3200_OE);
}
/******************************/
inline void TCS3200_LED_RED(void)
{
	GPIO_ResetBits(TCS3200_GPIO,TCS3200_S2 | TCS3200_S3);
}
inline void TCS3200_LED_BLUE(void)
{
	GPIO_ResetBits(TCS3200_GPIO,TCS3200_S2);
	GPIO_SetBits(TCS3200_GPIO,TCS3200_S3);
}
inline void TCS3200_LED_GREEN(void)
{
	GPIO_SetBits(TCS3200_GPIO,TCS3200_S2 | TCS3200_S3);
}
inline void TCS3200_LED_CLEAR(void)
{
	GPIO_ResetBits(TCS3200_GPIO,TCS3200_S3);
	GPIO_SetBits(TCS3200_GPIO,TCS3200_S2);
}
/******************************/
inline void TCS3200_FREQ_2p(void)
{
	GPIO_ResetBits(TCS3200_GPIO,TCS3200_S0);
	GPIO_SetBits(TCS3200_GPIO,TCS3200_S1);
}
inline void TCS3200_FREQ_20p(void)
{
	GPIO_ResetBits(TCS3200_GPIO,TCS3200_S1);
	GPIO_SetBits(TCS3200_GPIO,TCS3200_S0);
}
inline void TCS3200_FREQ_100p(void)
{
	GPIO_SetBits(TCS3200_GPIO,TCS3200_S0 | TCS3200_S1);
}
inline void TCS3200_FREQ_POWER_DOWN(void)
{
	GPIO_ResetBits(TCS3200_GPIO,TCS3200_S0 | TCS3200_S1);
}
/***********************************/


inline uint8_t TCS3200_FLAG_READY_ALL(void)
{
	if (FLAG_READY == 1)
	{
		return 1;
	}
	else return 0;
}
inline uint8_t TCS3200_FLAG_READY_ONE(void)
{
	if(FLAG_READY1 == 1)
	{
		return 1;
	}

	else return 0;
}

inline void TCS3200_FLAG_CLEAR_ALL(void)
{
	FLAG_READY = 0;
}

inline void TCS3200_FLAG_CLEAR_ONE(void)
{
	FLAG_READY1 = 0;
}


void TCS3200_MEASURE_ALL_COLOR_START(const Freg_Scaling value)
{
	if(Check1 == 1 && Check2 == 1)
	{
		Check1 = 0;
		switch (value)
		{
			case F2p:
			{
				TCS3200_FREQ_2p();
				break;
			}
			case F20p:
			{
				TCS3200_FREQ_20p();
				break;
			}
			case F100p:
			{
				TCS3200_FREQ_100p();
				break;
			}
		}
		Start = 1;
		TCS3200_ON();
		TIM_Cmd(TCS3200_TIM, ENABLE);
	}

}

void TCS3200_MEASURE_ONE_COLOR_START(const Freg_Scaling value, const Filter select_color)
{
	if(Check2 == 1 && Check1 == 1)
		{
			Check2 = 0;

			switch (value)
			{
				case F2p:
				{
					TCS3200_FREQ_2p();
					break;
				}
				case F20p:
				{
					TCS3200_FREQ_20p();
					break;
				}
				case F100p:
				{
					TCS3200_FREQ_100p();
					break;
				}
			}

			Select_Filter = select_color;

			Start = 1;
			m_one = 1;
			TCS3200_ON();
			TIM_Cmd(TCS3200_TIM, ENABLE);
		}
}

TCS3200_COLOR TCS3200_GET_ALL_COLOR(void)
{
	Start = 0;
	counter = 0;
	TCS3200_COLOR color = {0,0,0,0};
	color.Red = measure_red*10;
	color.Blue = measure_blue*10;
	color.Green = measure_green*10;
	color.Clear = measure_clear*10;

	Check1 = 1;
	Check = 1;

	//TCS3200_OFF();
	//TCS3200_FREQ_POWER_DOWN();
	return color;
}

unsigned int TCS3200_GET_ONE_COLOR(void)
{
	Start = 0;
	counter = 0;

	Check2 = 1;
	Check = 1;

	TCS3200_OFF();
	TCS3200_FREQ_POWER_DOWN();

	measure_one = measure_one * 10;
	return measure_one;
}


void EXTI0_IRQHandler(void)
{

  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	  if(Start == 1) counter++;
      EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

void TIM2_IRQHandler(void)
{
	static volatile uint8_t repetition = 0;
	static volatile uint8_t get_counter = 0;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		repetition++;
		if(repetition == 2)
		{
			get_counter = 1;
			repetition = 0;
		}
		else get_counter = 0;

		if(FLAG_READY == 0 && Start == 1 )
		{
			switch (Select_Filter)
			{
				case Red:
				{
					TCS3200_LED_RED();
					if(get_counter == 1 && Check == 1)
					{
						if(m_one == 1)
						{
							m_one = 0;
							measure_one = counter;
							Check = 0;
							Start = 0;
							FLAG_READY1 = 1;
							Select_Filter = Red;
							TIM_Cmd(TCS3200_TIM, DISABLE);
						}
						else
						{
							measure_red = counter;
							Select_Filter = Green;

						}
						get_counter = 0;
					}
					break;
				}
				case Green:
				{
					TCS3200_LED_GREEN();
					if(get_counter == 1 && Check == 1)
					{
						if(m_one == 1)
						{
							m_one = 0;
							measure_one = counter;
							Check = 0;
							Start = 0;
							FLAG_READY1 = 1;
							Select_Filter = Red;
							TIM_Cmd(TCS3200_TIM, DISABLE);
						}
						else
						{
							measure_green = counter;
							Select_Filter = Blue;

						}
						get_counter = 0;
					}
					break;
				}
				case Blue:
				{
					TCS3200_LED_BLUE();
					if(get_counter == 1 && Check == 1)
					{
						if(m_one == 1)
						{
							m_one = 0;
							measure_one = counter;
							Check = 0;
							Start = 0;
							FLAG_READY1 = 1;
							Select_Filter = Red;
							TIM_Cmd(TCS3200_TIM, DISABLE);
						}
						else
						{
							measure_blue = counter;
							Select_Filter = Clear;

						}
						get_counter = 0;
					}
					break;
				}
				case Clear:
				{
					TCS3200_LED_CLEAR();
					if(get_counter == 1 && Check == 1)
					{
						if(m_one == 1)
						{
							m_one = 0;
							measure_one = counter;
							FLAG_READY1 = 1;
						}
						else
						{
							measure_clear = counter;
							FLAG_READY = 1;

						}
						Check = 0;
						Start = 0;
						get_counter = 0;
						Select_Filter = Red;
						TIM_Cmd(TCS3200_TIM, DISABLE);
					}
					break;
				}
				default:
					break;
			}
		}
		counter = 0;


	}


}






















