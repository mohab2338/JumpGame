/*
 * File name: main.c
 * 
 * 	
 * 
 */

#define F_CPU 8000000UL
#include "util/delay.h"
/* FreeRTOS Main Header files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "E:\repos\Atmega32-16\Atmega32\Lib\std_types.h"
#include "E:\repos\Atmega32-16\Atmega32\Lib\BIT_MATH.h"

#include "LCD_interface.h"
#include "DIO_interface.h"

u8 crash = 1;
u8 Man[] = {
  0b00000,
  0b01110,
  0b01110,
  0b00100,
  0b01110,
  0b00100,
  0b01010,
  0b00000
};
u8 No_Man[] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
static u8 GameOver_Flag = 0;
void GameOver(void* pvParam)
{
	while(1)
	{

		if(GameOver_Flag == 1)
		{

			LCD_voidClearScreen();
			LCD_voidGotoxy(1,5);
			LCD_voidSendString("GAME OVER!");
			if(DIO_u8GetPinValue(DIO_u8PORT_A, DIO_u8PIN_1) == 0) GameOver_Flag = 0;
		}
		else if(GameOver_Flag == 0)
		{
			LCD_voidClearScreen();

		}
		vTaskDelay(1000);
	}


}
void hop(void* pvParam)
{
	while(1){
		if( (DIO_u8GetPinValue(DIO_u8PORT_A, DIO_u8PIN_0) == 1) && (GameOver_Flag == 0))
		{

			LCD_voidwriteSpecialCharacter(No_Man,1,3,2);
			LCD_voidwriteSpecialCharacter(Man,1,2,2);
			crash = 0;
			vTaskDelay(500);

		}
		else if(GameOver_Flag == 1)
		{
			LCD_voidGotoxy(3,2);
			LCD_voidSendData('x');

		}
		else
		{
			//LCD_voidGotoxy(3,2);
			//LCD_voidSendData('|');

			if(crash==0)
				{
				LCD_voidGotoxy(3,2);
				LCD_voidSendData('_');
				//
				}
			else
			{
				LCD_voidwriteSpecialCharacter(Man,1,3,2);
			}

		}
		vTaskDelay(30);
	}

}
void Frame1(void* pvParam)
{
	static u8 Frame_No = 0;
	while(1)
	{
		if(GameOver_Flag == 0)
		{
			switch(Frame_No)
			{
			case 0:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("____________________");crash = 1;
				Frame_No++;
				break;
			case 1:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("__________________#_");Frame_No++;crash = 1;
				break;
			case 2:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("_________________#__");		Frame_No++;crash = 1;

				break;
			case 3:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("________________#___");Frame_No++;crash = 1;
				break;
			case 4:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("_______________#____");Frame_No++;crash = 1;
				break;
			case 5:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("______________#_____");Frame_No++;crash = 1;
				break;
			case 6:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("_____________#______");Frame_No++;crash = 1;
				break;
			case 7:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("____________#_______");Frame_No++;crash = 1;
				break;
			case 8:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("___________#________");Frame_No++;crash = 1;
				break;
			case 9:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("__________#_________");Frame_No++;crash = 1;
				break;
			case 10:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("_________#__________");Frame_No++;crash = 1;
				break;
			case 11:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("________#___________");Frame_No++;crash = 1;
				break;
			case 12:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("_______#____________");Frame_No++;crash = 1;
				break;
			case 13:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("______#_____________");Frame_No++;crash = 1;
				break;
			case 14:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("_____#______________");Frame_No++;crash = 1;
				break;
			case 15:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("____#_______________");Frame_No++;crash = 1;
				break;
			case 16:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("___#________________");Frame_No++;
				break;
			case 17:
				if(crash == 0)
				{
					LCD_voidGotoxy(3,0);
					LCD_voidSendString("__#_________________");Frame_No++;
					crash = 1;
				}
				else
				{
					GameOver_Flag = 1;
				}

				break;
			case 18:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("_#__________________");Frame_No++;crash = 1;
				break;
			case 19:
				LCD_voidGotoxy(3,0);
				LCD_voidSendString("#___________________");Frame_No++;crash = 1;
				Frame_No = 0;
				break;
			}
		}

		vTaskDelay(100);

	}
}


TaskHandle_t		Tl_handle=NULL;
TaskHandle_t		T2_handle=NULL;
TaskHandle_t		T3_handle=NULL;

int main(void){
	LCD_voidInit();

	DIO_voidSetPinDirection(DIO_u8PORT_A, DIO_u8PIN_0, DIO_u8INPUT);
	DIO_voidSetPinDirection(DIO_u8PORT_A, DIO_u8PIN_1, DIO_u8INPUT);

	xTaskCreate(hop		, NULL, 100, NULL , 4, &Tl_handle);
	xTaskCreate(Frame1  , NULL, 100, NULL , 3, &T2_handle);
	xTaskCreate(GameOver, NULL, 50 , NULL , 2, &T3_handle);

	vTaskStartScheduler();
	while(1)
	{

	}

}

