/*******************************************************/
/************		Author: Yehia Ehab 		************/
/************		Layer: HAL     		************/
/************		SWC: SSD 				************/
/************		Version: 1.00	 		************/
/************		Date: 2-9-2020 		************/
/*******************************************************/
/*******************************************************/


#include "STD_TYPES.h"

#include "DIO_interface.h"

#include "SSD_config.h"
#include "SSD_interface.h"
#include "SSD_private.h"

void SSD_voidSendNumber(SSD_t* Copy_pu8StructSSD, u8 Copy_u8Number)
{
	u8 Local_u8Seg[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};
	u8 Local_u8LoopCounter;

	if(Copy_pu8StructSSD->SSD_u8Type == SSD_u8COMMON_ANODE)
		for(Local_u8LoopCounter=0;Local_u8LoopCounter<10;Local_u8LoopCounter++)
				Local_u8Seg[Local_u8LoopCounter] = ~Local_u8Seg[Local_u8LoopCounter];

	if(Copy_u8Number<10)
		DIO_u8SetPortValue(Copy_pu8StructSSD->SSD_u8Port, Local_u8Seg[Copy_u8Number]);
}

void SSD_voidDisplayOn(SSD_t* Copy_pu8StructSSD)
{
	if(Copy_pu8StructSSD->SSD_u8Type == SSD_u8COMMON_ANODE)
		DIO_u8SetPinValue(Copy_pu8StructSSD->SSD_u8EnablePort, Copy_pu8StructSSD->SSD_u8EnablePin, DIO_u8PIN_HIGH);
	else
		DIO_u8SetPinValue(Copy_pu8StructSSD->SSD_u8EnablePort, Copy_pu8StructSSD->SSD_u8EnablePin, DIO_u8PIN_LOW);
}

void SSD_voidDisplayOff(SSD_t* Copy_pu8StructSSD)
{
	if(Copy_pu8StructSSD->SSD_u8Type == SSD_u8COMMON_ANODE)
		DIO_u8SetPinValue(Copy_pu8StructSSD->SSD_u8EnablePort, Copy_pu8StructSSD->SSD_u8EnablePin, DIO_u8PIN_LOW);
	else
		DIO_u8SetPinValue(Copy_pu8StructSSD->SSD_u8EnablePort, Copy_pu8StructSSD->SSD_u8EnablePin, DIO_u8PIN_HIGH);
}

void SSD_voidSendCustomCharacter(SSD_t* Copy_pu8StructSSD, u8 Copy_u8Character)
{
	DIO_u8SetPortValue(Copy_pu8StructSSD->SSD_u8Port, Copy_u8Character);
}
