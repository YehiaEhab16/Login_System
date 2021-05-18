/*******************************************************/
/************		Author: Yehia Ehab 		************/
/************		Layer: HAL     		************/
/************		SWC: SSD 				************/
/************		Version: 1.00	 		************/
/************		Date: 2-9-2020 		************/
/*******************************************************/
/*******************************************************/


/* Preprocessor file guard */
#ifndef SSD_INTERFACE_H_
#define SSD_INTERFACE_H_

#define SSD_PORTA				DIO_u8PORTA
#define SSD_PORTB				DIO_u8PORTB
#define SSD_PORTC				DIO_u8PORTC
#define SSD_PORTD				DIO_u8PORTD

#define SSD_PIN0				DIO_u8PIN0
#define SSD_PIN1				DIO_u8PIN1
#define SSD_PIN2				DIO_u8PIN2
#define SSD_PIN3				DIO_u8PIN3
#define SSD_PIN4				DIO_u8PIN4
#define SSD_PIN5				DIO_u8PIN5
#define SSD_PIN6				DIO_u8PIN6
#define SSD_PIN7				DIO_u8PIN7

#define SSD_u8COMMON_CATHODE	0
#define SSD_u8COMMON_ANODE		1

typedef struct My_struct2
{
	u8 SSD_u8Port;
	u8 SSD_u8EnablePort;
	u8 SSD_u8EnablePin;
	u8 SSD_u8Type;
}SSD_t;

void SSD_voidSendNumber(SSD_t* Copy_pu8StructSSD, u8 Copy_u8Number);
void SSD_voidDisplayOn(SSD_t* Copy_pu8StructSSD);
void SSD_voidDisplayOff(SSD_t* Copy_pu8StructSSD);
void SSD_voidSendCustomCharacter(SSD_t* Copy_pu8StructSSD, u8 Copy_u8Character);


#endif

