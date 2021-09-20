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

int main(void)
{
/*------------------------------------------------------------------------------
CONFIGURACION DEL MICRO:
------------------------------------------------------------------------------*/
	SystemInit();

	//Inicializacion del UserButton por interrupcion externa:
	INIT_EXTINT(UserButton_Port, UserButton);

	//Inializacion del led interno de prueba:
	INIT_DO(GPIOB, GPIO_Pin_0);

/*------------------------------------------------------------------------------
BUCLE PRINCIPAL:
------------------------------------------------------------------------------*/
    while(1)
    {

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
		GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}
