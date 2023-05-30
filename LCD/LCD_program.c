/*
 * LCD_program.c
 *
 *  Created on: 15 Aug 2021
 *      Author: omar
 */


#include "util/delay.h"
#include <math.h>
#include "E:\repos\Atmega32-16\Atmega32\Lib\std_types.h"
#include "E:\repos\Atmega32-16\Atmega32\Lib\BIT_MATH.h"

#include "E:\repos\Atmega32-16\Atmega32\DIO\Dio_interface.h"

#include "LCD_config.h"
#include "LCD_interface.h"

u16 Display_Array[REESOLUTION];

void LCD_voidSendCommand(u8 Copy_u8Command)
{
	/*1- set RS with low --> command*/
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_RS_PIN, DIO_u8LOW);
	/*2- set RW to low to write*/
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_RW_PIN, DIO_u8LOW);
	/*3- set command on data pins*/
	DIO_voidSetPortValue(LCD_DATA_PORT,Copy_u8Command);
	/*4- send enable pulse*/
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_E_PIN , DIO_u8HIGH);
	_delay_ms(2);
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_E_PIN, DIO_u8LOW);
}
void LCD_voidSendData(u8 Copy_u8Data)
{
	/*1- set RS with HIGH -->data */
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_RS_PIN, DIO_u8HIGH);
	/*2- set RW to low to write*/
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_RW_PIN, DIO_u8LOW);
	/*3- set Data on data pins*/
	DIO_voidSetPortValue(LCD_DATA_PORT,Copy_u8Data);
	/*4- send enable pulse*/
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_E_PIN, DIO_u8HIGH);
	_delay_us(50);
	DIO_voidSetPinValue(LCD_CTRL_PORT, LCD_E_PIN, DIO_u8LOW);
}
void LCD_voidInit(void)
{

	DIO_voidSetPortDirection(LCD_DATA_PORT,0xff);
	DIO_voidSetPinDirection(LCD_CTRL_PORT,LCD_RS_PIN,DIO_u8HIGH);
	DIO_voidSetPinDirection(LCD_CTRL_PORT,LCD_RW_PIN,DIO_u8HIGH);
	DIO_voidSetPinDirection(LCD_CTRL_PORT,LCD_E_PIN,DIO_u8HIGH);
	/**HALT 30ms
	 *
	 */
	_delay_ms(30);
	/*function set command ::2lines, 588 Font size*/
	LCD_voidSendCommand(0b00111000);
	/*function on off Display*/
	LCD_voidSendCommand(0b00001100);
	/*Clear command*/
	LCD_voidSendCommand(1);

}


void LCD_voidSendString(const char* Copy_pcString)
{
	u8 Local_u8Counter =0;
	while(Copy_pcString[Local_u8Counter]!='\0')
	{
		LCD_voidSendData(Copy_pcString[Local_u8Counter]);
		Local_u8Counter++ ;
	}

}
void LCD_voidGotoxy(u8 Copy_u8xPos, u8 Copy_u8yPos )
{
	u8 LocalAddress = 0;
	if((Copy_u8xPos <= 3) && (Copy_u8yPos >=0 )&&(Copy_u8yPos <= 15))
	{
		if(Copy_u8xPos==0)
		{
			LocalAddress = Copy_u8yPos;
		}
		else if(Copy_u8xPos==1)
		{
			LocalAddress = 0xC0 + Copy_u8yPos;
		}
		else if(Copy_u8xPos==2)
		{
			LocalAddress = 0x94 + Copy_u8yPos;
		}
		else if(Copy_u8xPos==3)
		{
			LocalAddress = 0xD4 + Copy_u8yPos;
		}



		//LocalAddress = LocalAddress + 128;
		LCD_voidSendCommand(LocalAddress);
	}
	else {/*wrong position*/}
}

void LCD_voidwriteSpecialCharacter(u8 Copy_pu8Pattern[], u8 Copy_u8PatternNumber, u8 Copy_u8xPos, u8 Copy_u8yPos )
{
	u8 Local_u8Address;
	u8 Local_u8Counter;
	/* 1- calculate CGRAM block number , start address of location*/
	Local_u8Address = Copy_u8PatternNumber *8;
	/*2- send command--> Address counter  points to CGRAM instead of DDRAM */
	LCD_voidSendCommand(Local_u8Address + 64);
	/*3- store /write in certain location in CGRAM*/
	for(Local_u8Counter=0 ;Local_u8Counter <8 ; Local_u8Counter++)
	{
		LCD_voidSendData(Copy_pu8Pattern[Local_u8Counter]);
	}

	/*4- it display on LCD --> change AC to Point to DDRAM*/
	LCD_voidGotoxy( Copy_u8xPos, Copy_u8yPos );

	/*5- Display the pattern in the location received */
	LCD_voidSendData( Copy_u8PatternNumber );

}

void LCD_voidClearScreen()
{
	/*Clear command*/
	LCD_voidSendCommand(1);
}

void LCD_VoidDisplayInteger(u32 Displayed_Value){
	u8 Local_resolution = 0;
	u32 temp = Displayed_Value;
	while(temp != 0 )
	{
		if( (temp = temp/10)==0)  break;
		Local_resolution++;
	}

	temp = Local_resolution;
	Local_resolution = 0;

	while(Local_resolution <= temp )
	{
		Display_Array[temp - Local_resolution] = (  Displayed_Value - ( (Displayed_Value/10 )*10 )  );
		Displayed_Value = (Displayed_Value/10 );
		Local_resolution++;
	}

	Local_resolution = 0;

	for(u8 i = 0; i <= temp ;i++)
	{
		LCD_voidSendData(Display_Array[i] + '0');
	}
}

void LCD_VoidDisplayFloat(f32 Displayed_Value)
{
	static u32 Temp_variable = 0;

	LCD_VoidDisplayInteger(Displayed_Value);
	LCD_voidSendData('.');
	Temp_variable = round( ( ( Displayed_Value - (u32)(Displayed_Value) ) *pow(10,DISPLAYED_FlOAT_RESOLUTION)));
	LCD_VoidDisplayInteger(Temp_variable);
}
