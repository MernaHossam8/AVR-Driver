/**************************************/
/* Description : LCD c File           */
/* Author      : aaboelno             */
/* Version     : 0.1V                 */
/* Date        : 13:45:20  03 Dec 2022*/
/* History     : 0.1V Initial Creation*/
/**************************************/
#include "STD_Types.h"
#include "BIT_Math.h"
#include <util/delay.h>

#include "DIO.h"

#include "LCD.h"
#include "LCD_cfg.h"
#include "LCD_priv.h"


void LCD_voidInit(void)
{
	uint8 u8CntrLoc = 0 ;
	uint8 u8CmdLoc  = 0 ;
	for ( u8CntrLoc = 0 ; u8CntrLoc < LCD_MAX_NUM ; u8CntrLoc ++)
	{
		if (LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8Mode  == LCD_8BIT_MODE )
		{
			/* Step 1 */
			_delay_ms(30);
			/* Step 2 */
			u8CmdLoc = (0b00110000)  |
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8UsedLines  << 3 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8CharFont   << 2 );

			voidWriteCmd(u8CntrLoc , u8CmdLoc);
			/* Step 3 */
			_delay_ms(1);
			/* Step 4 */
			u8CmdLoc =   (0b00001000) |
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8DisplayState     << 2 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8CursorState      << 1 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8CursorBlinkState << 0 );
			voidWriteCmd(u8CntrLoc , u8CmdLoc);
			/* Step 5 */
			_delay_ms(1);
			/* Step 6 */
			voidWriteCmd(u8CntrLoc , 0x01);
			/* Step 7 */
			_delay_ms(3);
			/* Step 8 */
			u8CmdLoc =   (0b00000100) |
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8IncrementMode  << 1 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8ShiftMode      << 0 );

			voidWriteCmd(u8CntrLoc , u8CmdLoc);
			/*Extra delay */
			_delay_ms(3);

		}
		else 
		{
			/* Step 1 */
			_delay_ms(30);
			/* Step 2 */
			voidWriteCmd(u8CntrLoc , 0b00100000);
			voidWriteCmd(u8CntrLoc , 0b00100000);
			u8CmdLoc = (0b00100000)  |
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8UsedLines  << 7 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8CharFont   << 6 );

			voidWriteCmd(u8CntrLoc , u8CmdLoc);
			/* Step 3 */
			_delay_ms(1);
			/* Step 4 */
			voidWriteCmd(u8CntrLoc , 0b00000000);

			u8CmdLoc =   (0b10000000) |
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8DisplayState     << 6 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8CursorState      << 5 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8CursorBlinkState << 4 );
			voidWriteCmd(u8CntrLoc , u8CmdLoc);
			/* Step 5 */
			_delay_ms(1);
			/* Step 6 */
			voidWriteCmd(u8CntrLoc , 0b00000000);
			voidWriteCmd(u8CntrLoc , 0b00010000);
			/* Step 7 */
			_delay_ms(3);
			/* Step 8 */
			voidWriteCmd(u8CntrLoc , 0b00000000);
			u8CmdLoc =   (0b01000000) |
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8IncrementMode  << 5 )|
					(LCD_astrConfigSet[u8CntrLoc].strLcdHwCfg.u8ShiftMode      << 4 );

			voidWriteCmd(u8CntrLoc , u8CmdLoc);
			/* Extra Delay*/
			_delay_ms(3);
		}
	}


}

void LCD_voidWriteCmd(uint8 u8LcdIndxCpy , uint8 u8CmdCpy)
{
	if (LCD_astrConfigSet[u8LcdIndxCpy].strLcdHwCfg.u8Mode  == LCD_8BIT_MODE )
	{
		voidWriteCmd(u8LcdIndxCpy ,u8CmdCpy );
		_delay_ms(1);
	}
	else
	{
		voidWriteCmd(u8LcdIndxCpy ,u8CmdCpy);  
		_delay_ms(1);
		voidWriteCmd(u8LcdIndxCpy ,u8CmdCpy<<4);
		_delay_ms(1);
	}

}

void LCD_voidDisplayChar(uint8 u8LcdIndxCpy , uint8 u8CharCpy)
{
	// Rs  1
	DIO_enuWritePin(LCD_astrConfigSet[u8LcdIndxCpy].strLcdConnectionCfg.u8Rs , DIO_HIGH );

	if (LCD_astrConfigSet[u8LcdIndxCpy].strLcdHwCfg.u8Mode  == LCD_8BIT_MODE )
	{
		voidLcdHelper(u8LcdIndxCpy , u8CharCpy);
		_delay_ms(1);
	}
	else
	{

		voidLcdHelper(u8LcdIndxCpy , u8CharCpy);  
		_delay_ms(1);
		voidLcdHelper(u8LcdIndxCpy , u8CharCpy<<4);
		_delay_ms(1);
	}

}

static void voidWriteCmd(uint8 u8LcdIndxCpy , uint8 u8CmdCpy)
{
	// Rs  0
	DIO_enuWritePin(LCD_astrConfigSet[u8LcdIndxCpy].strLcdConnectionCfg.u8Rs , DIO_LOW );

	voidLcdHelper( u8LcdIndxCpy, u8CmdCpy);


}

static void voidLcdHelper(uint8 u8LcdIndxCpy , uint8 u8CmdDataCpy)
{
	uint8 u8CntrLoc ;
	// RW  0
	DIO_enuWritePin(LCD_astrConfigSet[u8LcdIndxCpy].strLcdConnectionCfg.u8Rw , DIO_LOW );
	//latch start
	DIO_enuWritePin(LCD_astrConfigSet[u8LcdIndxCpy].strLcdConnectionCfg.u8En , DIO_HIGH );

	// check 4 bit 8
	if (LCD_astrConfigSet[u8LcdIndxCpy].strLcdHwCfg.u8Mode  == LCD_8BIT_MODE )
	{
		// send cmd or data 8 bit
		for (u8CntrLoc = 0; u8CntrLoc < 8 ; u8CntrLoc++)
		{                   /* Rakm elselka                                                          cmd or data  */
			DIO_enuWritePin(LCD_astrConfigSet[u8LcdIndxCpy].strLcdConnectionCfg.u8Data[u8CntrLoc] , GET_BIT(u8CmdDataCpy,u8CntrLoc) );

		}
	}
	else
	{
		// send cmd or data 4 bit
		for (u8CntrLoc = 4; u8CntrLoc < 8 ; u8CntrLoc++)
		{
			DIO_enuWritePin(LCD_astrConfigSet[u8LcdIndxCpy].strLcdConnectionCfg.u8Data[u8CntrLoc] , GET_BIT(u8CmdDataCpy,u8CntrLoc) );

		}
	}

	//latch
	_delay_ms(10);
	DIO_enuWritePin(LCD_astrConfigSet[u8LcdIndxCpy].strLcdConnectionCfg.u8En , DIO_LOW );
	_delay_ms(10);

}
void LCD_voidSendString(uint8 u8LcdIndxCpy,const uint8* Copy_pchString)
{
	uint8 Local_u8Counter=0;
	while(Copy_pchString[Local_u8Counter]!='\0')
	{
		LCD_voidDisplayChar(u8LcdIndxCpy,Copy_pchString[Local_u8Counter]);
		Local_u8Counter++;
	}
}
void lcd_home(uint8 u8LcdIndxCpy)
{
	LCD_voidWriteCmd (u8LcdIndxCpy,0x01);
	_delay_ms(2);
	LCD_voidWriteCmd (u8LcdIndxCpy, 0x80);
}
void lcdClearbit(uint8 u8LcdIndxCpy,uint8 Copy_u8XPos,uint8 Copy_u8YPos)
{
	LCD_voidGotoXY(u8LcdIndxCpy,(Copy_u8XPos-1), (Copy_u8YPos));
	LCD_voidWriteCmd (u8LcdIndxCpy,0x01);
	_delay_ms(2);

}
void LCD_voidGotoXY(uint8 u8LcdIndxCpy,uint8 Copy_u8XPos, uint8 Copy_u8YPos)
{
	uint8 Local_u8DDRAMAddress;
	if(Copy_u8YPos ==0)
	{
		/*Check if in first line*/
		Local_u8DDRAMAddress=Copy_u8XPos;
	}
	else if(Copy_u8YPos==1)
	{
		/*Check if in second line*/
		Local_u8DDRAMAddress=0x40+Copy_u8XPos;
	}

	/*Setbit number 7 for SetDDRAM Address command then send the command*/
	LCD_voidWriteCmd(u8LcdIndxCpy,Local_u8DDRAMAddress+128);
}

void LCD_voidWriteSpecialCharacter(uint8 u8LcdIndxCpy,uint8* Copy_pu8Pattern,uint8 Copy_u8PatternNumber,uint8 Copy_u8XPos,uint8 Copy_u8YPos)
{
	uint8 Local_u8CGRamAddress,Local_u8Counter;

	/*1- calculate CGRAM address knowing that every pattern consumes 8 bytes*/
	Local_u8CGRamAddress= Copy_u8PatternNumber*8;

	/*2-Send SET CGRAM address command to LCD with setting bit6 ,clearing bit7*/
	LCD_voidWriteCmd(u8LcdIndxCpy,Local_u8CGRamAddress+64);

	/*3- write the pattern into CGRAM*/
	for(Local_u8Counter=0;Local_u8Counter<8;Local_u8Counter++)
	{
		LCD_voidDisplayChar(u8LcdIndxCpy,Copy_pu8Pattern[Local_u8Counter]);
	}
	/*4- Back to the DDRAM to display the special pattern*/
	LCD_voidGotoXY(u8LcdIndxCpy,Copy_u8XPos,Copy_u8YPos);

	/*5- Display the special pattern*/
	LCD_voidDisplayChar(u8LcdIndxCpy,Copy_u8PatternNumber);
}
void LCD_voidSendNumber(uint8 u8LcdIndxCpy , uint32 u8IntCpy)
{
	uint32 u32NumRange = 1;

	while((u32NumRange/u8IntCpy) >= 10)
	{
		u32NumRange *= 10;
	}

	while(u32NumRange != 0)
	{
		LCD_voidDisplayChar(u8LcdIndxCpy ,( (u8IntCpy/u32NumRange) ));
		u8IntCpy %= u32NumRange;
		u32NumRange /= 10;
	}

//	LCD_voidDisplayChar(u8LcdIndxCpy ,(u8IntCpy + '0'));


}

