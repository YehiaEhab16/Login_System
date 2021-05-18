/*
 * main.c
 *
 *  Created on: Sep 24, 2020
 *      Author: yehia
 */

#include "STD_TYPES.h"

#include <util/delay.h>

#include "DIO_interface.h"
#include "PORT_interface.h"
#include "ADC_interface.h"
#include "TIMERS_interface.h"

#include "CLCD_interface.h"
#include "KPD_interface.h"
#include "SSD_interface.h"
#include "DCM_interface.h"

#define WELCOME_TIME			3000
#define DELAY_TIME				200

u16 combine(u16 Copy_u16First, u8 Copy_u8Second);
u16 reverse(u16 Copy_u16Number);
s16 map(s16 Copy_s16InputNumber, s16 Copy_s16InputMinimum, s16 Copy_s16InputMaximum, s16 Copy_s16OutputMinimum, s16 Copy_s16OutputMaximum);

void main(void)
{
	u8 Local_u8Leds, Local_u8Keypad, Local_u8ReverseFlag=0, Local_u8Flag=0, Local_u8Trials=3, Local_u8XPos=9, Local_u8Clear[8]={0};
	u16 Local_u16Ldr, Local_u16Temperature, Local_u16Digital, Local_u16ID=0, Local_u16Password=0;

	PWM_t Local_PWM_tLed = {TIMER1_CHANNEL_B, 0};

	SSD_t Local_su8SevenSegment = {SSD_PORTB, SSD_PORTA, SSD_PIN2, SSD_u8COMMON_CATHODE};

	DCM_t Local_su8Motor = {DCM_PORTA, DCM_PIN3};

	PORT_voidInit();
	ADC_voidInit();
	PWM_voidInit(&Local_PWM_tLed);

	CLCD_voidInit();

	CLCD_voidGotoXY(2,0);
	CLCD_voidSendString("Welcome :)");
	_delay_ms(WELCOME_TIME);

	CLCD_voidClearDisplay();
	CLCD_voidSendString("Enter ID:");

	while(1)
	{
		Local_u8Keypad = KPD_u8GetPressedKey();

		if(Local_u8ReverseFlag==1)
		{
			SSD_voidSendNumber(&Local_su8SevenSegment, Local_u8Trials);
			SSD_voidDisplayOn(&Local_su8SevenSegment);
		}

		if(Local_u8ReverseFlag==0)
		{
			if(Local_u8Keypad!=255)
			{
				CLCD_voidSendNumber(Local_u8Keypad);
				Local_u16ID = combine(Local_u16ID, Local_u8Keypad);
				Local_u8Flag++;
			}
		}

		if(Local_u8ReverseFlag==1)
		{
			if(Local_u8Keypad!=255)
			{
				CLCD_voidSendNumber(Local_u8Keypad);
				_delay_ms(200);
				CLCD_voidWriteSpecialCharacter(Local_u8Clear, 0, Local_u8XPos, 1);
				CLCD_voidGotoXY(Local_u8XPos,1);
				Local_u8XPos++;
				CLCD_voidSendData('*');
				Local_u16Password = combine(Local_u16Password, Local_u8Keypad);
				Local_u8Flag+=3;
			}
		}

		if(Local_u8Flag==4)
		{
			Local_u8Flag=0;
			Local_u8ReverseFlag=1;
			_delay_ms(DELAY_TIME);
			CLCD_voidClearDisplay();

			CLCD_voidSendString("Enter           Password:");
		}

		if(Local_u8Flag==11)
		{
			Local_u8Flag=0;
			Local_u8XPos=6;
			CLCD_voidClearDisplay();
			CLCD_voidSendString("Try             Again:");
		}

		if(Local_u8Flag==12)
		{
			_delay_ms(DELAY_TIME);
			CLCD_voidClearDisplay();
			if(Local_u16Password==reverse(Local_u16ID))
			{
				SSD_voidDisplayOff(&Local_su8SevenSegment);
				while(1)
				{
				ADC_u8StartConversionSynch(0, &Local_u16Ldr);
				ADC_u8StartConversionSynch(1, &Local_u16Digital);

				Local_u16Temperature = ((u32)Local_u16Digital * 5000UL) / 256UL;
				Local_u16Temperature/=10;

				Local_PWM_tLed.PWM_Value = ~(Local_u16Ldr);

				Local_u8Leds = map(Local_PWM_tLed.PWM_Value, 20, 250, 0, 100);

				PWM_voidAnalogWrite(&Local_PWM_tLed);

				CLCD_voidGotoXY(0,0);
				CLCD_voidSendString("Temperature:");
				CLCD_voidSendNumber(Local_u16Temperature);

				CLCD_voidGotoXY(0,1);
				CLCD_voidSendString("Level:");
				CLCD_voidSendNumber(Local_u8Leds);
				CLCD_voidGotoXY(8,1);
				CLCD_voidSendString("        ");

				if(Local_u16Temperature>33)
					DCM_voidRotateA(&Local_su8Motor);
				else
					DCM_voidStopA(&Local_su8Motor);
				}
			}

			else if(Local_u16Password!=reverse(Local_u16ID))
			{
				Local_u8Trials--;
				Local_u8Flag=11;
				Local_u16Password=0;
				if(Local_u8Trials==0)
				{
					SSD_voidSendNumber(&Local_su8SevenSegment, 0);
					CLCD_voidGotoXY(3,0);
					CLCD_voidSendString("Bye Bye");
					_delay_ms(DELAY_TIME);
					for(Local_u8Flag=0; Local_u8Flag<=3; Local_u8Flag++)
					{
						SSD_voidSendNumber(&Local_su8SevenSegment, 3-Local_u8Flag);
						_delay_ms(1000);
					}
					CLCD_voidClearDisplay();
					SSD_voidDisplayOff(&Local_su8SevenSegment);
					while(1);
				}
			}
		}
	}
}

u16 combine(u16 Copy_u16First, u8 Copy_u8Second)
{
    u8 times=1;

    while(times<=Copy_u8Second)
        times *= 10;

    return Copy_u16First * times + Copy_u8Second;
}

u16 reverse(u16 Copy_u16Number)
{
    u16 Local_u16Reverse=0;

    while (Copy_u16Number != 0)
    {
        Local_u16Reverse = Local_u16Reverse * 10;
        Local_u16Reverse = Local_u16Reverse + Copy_u16Number%10;
        Copy_u16Number = Copy_u16Number/10;
    }
    return Local_u16Reverse;
}

s16 map(s16 Copy_s16InputNumber, s16 Copy_s16InputMinimum, s16 Copy_s16InputMaximum, s16 Copy_s16OutputMinimum, s16 Copy_s16OutputMaximum)
{
	s16 Local_s16OutputNumber = (s16)(((f32)(Copy_s16OutputMaximum - Copy_s16OutputMinimum) / (f32)(Copy_s16InputMaximum - Copy_s16InputMinimum))*(f32)(Copy_s16InputNumber - Copy_s16InputMinimum)) + Copy_s16OutputMinimum;

	return Local_s16OutputNumber;
}
