 // Lab3P1.c
 //
 // Created: 6/21/2018 4:04:52 AM
 // Author : Group 1
 // Copyright 2018, All Rights Reserved

 #define F_CPU 1000000L

 #include <math.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>

 const char MS1[] = ;
 const char MS2[] = ;
 const char MS3[] = ;
 const char MS4[] = ;

 int keyStroke = 0;

 void LCD_Init(void);			//external Assembly functions
 void LCD_Write_Data(void);
 void LCD_Write_Command(void);
 void LCD_Read_Data(void);
 void LCD_Delay(void);
 void Mega328P_Init(void);

 char temp[5];					//string buffer for ADC output

 void LCD_Puts(const char *str)	//Display a string on the LCD Module
 {
	 while (*str)
	 {
		 DATA = *str++;
		 LCD_Write_Data();
	 }
 }

 void LCD(void)						//Lite LCD demo
 {
	 LCD_Write_Command();
	 DATA = 0x02;					//Returns home
	 LCD_Write_Command();
	 DATA = 0x0c;					//Display on cursor off
	 LCD_Write_Command();
	 
	 LCD_Puts();				//Puts the output sting onto the LCD

	 }
 }

 void Command(void)					//command interpreter
 {
	 UART_On();
	 UART_Puts(MS3);
	 ASCII = '\0';
	 while (ASCII == '\0')
	 {
		 UART_Get();
	 }
	 switch (ASCII)
	 {
		 case 'L' | 'l': LCD();
		 break;
		 case 'A' | 'a': Temperature_ADC();
		 break;
		 case 'E' | 'e': EEPROM();
		 break;
		 default:
		 UART_Puts(MS5);
		 HELP();
		 break;  			//Add a 'USART' command and subroutine to allow the user to reconfigure the 						//serial port parameters during runtime. Modify baud rate, # of data bits, parity, 							//# of stop bits.
	 }
 }

 int main(void)
 {
	 Mega328P_Init();
	 Banner();
	 while (1)
	 {
		 Command();				//infinite command loop
	 }
 }