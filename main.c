/********************************************************************
*																	*
*																	*
*                        SAMPLE TCS3200 USE							*
*		  															*
*																	*
********************************************************************/


#include "system.h"
#include <stdio.h>
#include "lcd_hd44780_lib.h"



int main(void)
{
	const int fd_r = 70;
	const int fd_g = 60;
	const int fd_b = 70;
	const int fw_r = 630;
	const int fw_g = 560;
	const int fw_b = 660;

	unsigned int R = 0;
	unsigned int G = 0;
	unsigned int B = 0;
	PeripheralInit();
	TCS3200_Init();
	LCD_Initialize();




	LCD_WriteCommand(HD44780_CLEAR);
	unsigned char licznikTekst1[5] = {"0\0"};
	unsigned char licznikTekst2[5] = {"0\0"};
	unsigned char licznikTekst3[5] = {"0\0"};
	unsigned char licznikTekst4[5] = {"0\0"};
	unsigned char licznikTekst5[5] = {"0\0"};


	TCS3200_COLOR ab = {0,0,0,0};
	Freg_Scaling scaling = F2p;
	//Filter colorss;
	//colorss = Red;
	unsigned int one;


    while(1)
    {

		TCS3200_MEASURE_ALL_COLOR_START(scaling);

    	if(TCS3200_FLAG_READY_ALL() == 	1)
    	{
    		TCS3200_FLAG_CLEAR_ALL();
       		ab = TCS3200_GET_ALL_COLOR();
       		R = (255*(ab.Red - fd_r))/ (fw_r - fd_r);
       		G = ((255*(ab.Green - fd_g))/ (fw_g - fd_g)) +30;
       		B = ((255*(ab.Blue - fd_b))/ (fw_b - fd_b)) - 40;
    		LCD_WriteCommand(HD44780_CLEAR);
			sprintf((char *)licznikTekst1,"%4u",R);
			LCD_WriteTextXY(licznikTekst1,0,0);
			sprintf((char *)licznikTekst2,"%4u",B);
			LCD_WriteTextXY(licznikTekst2,1,1);
			sprintf((char *)licznikTekst3,"%4u",G);
			LCD_WriteTextXY(licznikTekst3,6,0);
			sprintf((char *)licznikTekst4,"%4u",ab.Clear);
			LCD_WriteTextXY(licznikTekst4,6,1);
    	}
    	Delay_ms(1000);

    	//TCS3200_MEASURE_ONE_COLOR_START(scaling,colorss);

    	/*if(TCS3200_FLAG_READY_ONE() == 1)
    	{
    		TCS3200_FLAG_CLEAR_ONE();
    		one = TCS3200_GET_ONE_COLOR();

    		LCD_WriteCommand(HD44780_CLEAR);
    		sprintf((char *)licznikTekst5,"%4u",one);
    		LCD_WriteTextXY(licznikTekst5,6,1);
    		if (colorss == 3) colorss = Red;
    		else colorss++;

    	}
    	Delay_ms(1000); */

    }
}


