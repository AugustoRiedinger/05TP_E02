/**********
  * @file    main.c
  * @author  G. Garcia & A. Riedinger.
  * @version 0.1
  * @date    20.09.21
  * @brief   Se crea un nivel de continua desde 0V hasta VDD con paso de 100mV
  * 		 cada vez que se pulsa el UserButton PC13
  *
  * SALIDAS:
  	  *	LCD
  	  *	DAC CHN2 PA5
  *
  * ENTRADAS:
  	  * UserButton - PC13
**********/

/*------------------------------------------------------------------------------
LIBRERIAS:
------------------------------------------------------------------------------*/
#include "mi_libreria.h"

/*------------------------------------------------------------------------------
DEFINICIONES:
------------------------------------------------------------------------------*/
//Pines del UserButton:
#define UserButton_Port	GPIOC
#define UserButton		GPIO_Pin_13

//Pines del DAC:
#define DAC_Port GPIOA
#define DAC_Pin	 GPIO_Pin_5

//Led de aviso de pulso User LED Green:
#define UserLED_Green_Port GPIOB
#define UserLED_Green	   GPIO_Pin_0

//Paso cada cual se incrementa el DAC:
#define Delta_mV 100

//Parámetros de configuración del TIM3 para refresco del LCD:
#define Freq 	 4		//Equivalente a 250mseg
#define TimeBase 200e3
/*------------------------------------------------------------------------------
DECLARACION DE FUNCIONES LOCALES:
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
VARIABLES GLOBALES:
------------------------------------------------------------------------------*/
//Definicion de los pines del LCD:
LCD_2X16_t LCD_2X16[] = {
			// Name  , PORT ,   PIN      ,         CLOCK       ,   Init
			{ TLCD_RS, GPIOC, GPIO_Pin_10, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_E,  GPIOC, GPIO_Pin_11, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D4, GPIOC, GPIO_Pin_12, RCC_AHB1Periph_GPIOC, Bit_RESET },
			{ TLCD_D5, GPIOD, GPIO_Pin_2,  RCC_AHB1Periph_GPIOD, Bit_RESET },
			{ TLCD_D6, GPIOF, GPIO_Pin_6,  RCC_AHB1Periph_GPIOF, Bit_RESET },
			{ TLCD_D7, GPIOF, GPIO_Pin_7,  RCC_AHB1Periph_GPIOF, Bit_RESET }, };

//Varibale de manejo de los milivolts de salida del DAC:
uint32_t MiliVolt = 0;

int main(void)
{
/*------------------------------------------------------------------------------
CONFIGURACION DEL MICRO:
------------------------------------------------------------------------------*/
	SystemInit();

	//Inicializacion del DISPLAY LCD:
	INIT_LCD_2x16(LCD_2X16);

	//Inicializacion del UserButton por interrupcion externa:
	INIT_EXTINT(UserButton_Port, UserButton);

	//Inializacion de PA5 como salida DAC de continua:
	INIT_DAC_CONT(DAC_Port, DAC_Pin);

	//Led de aviso de pulso User LED Green;
	INIT_DO(UserLED_Green_Port, UserLED_Green);

	//Inicialización del TIM3 para refresco del LCD:
	INIT_TIM3();
	SET_TIM3(TimeBase, Freq);

/*------------------------------------------------------------------------------
BUCLE PRINCIPAL:
------------------------------------------------------------------------------*/
    while(1)
    {
    	/*Si el voltaje en el DAC es menor que el voltaje maximo de referencia, se sigue incrementando: */
    	if(MiliVolt <= MaxMiliVoltRef)
    		DAC_CONT(DAC_Port, DAC_Pin, MiliVolt);
    	/*Si el voltaje en el DAC es mayor o igual al de referencia, se resetea: */
    	else
    		MiliVolt = 0;
    }

}

/*------------------------------------------------------------------------------
INTERRUPCIONES:
------------------------------------------------------------------------------*/
/*Interrupcion al pulso ascendente por PC13: */
void EXTI15_10_IRQHandler(void)
{
	/*Si la interrupcion fue por linea 13 (PC13): */
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		/*Se enciende y apaga el led testigo de pulso: */
		GPIO_ToggleBits(UserLED_Green_Port, UserLED_Green);

		/*Se incrementa el valor de voltaje en un Delta_mV = 100mV: */
		MiliVolt += Delta_mV;

		/*Se resetea el flag de interrupcion: */
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

/*Interrupción por agotamiento de cuenta del TIM3 cada 250mseg (4 Hz): */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

		/*Buffers para mostrar valores de variables: */
		char BufferMiliVolt[BufferLength];

		/*Refresco del LCD: */
		CLEAR_LCD_2x16(LCD_2X16);

		/*Mostrar mensaje generico: */
		PRINT_LCD_2x16(LCD_2X16, 0, 0, "TDII  -  E02 TP5");

		/*Mostrar valor de voltaje en mV: */
		sprintf(BufferMiliVolt, "%d", MiliVolt);
		PRINT_LCD_2x16(LCD_2X16, 0, 1, "DAC = ");
		PRINT_LCD_2x16(LCD_2X16, 7, 1, BufferMiliVolt);
		PRINT_LCD_2x16(LCD_2X16, 12, 1, "mV");
	}
}
