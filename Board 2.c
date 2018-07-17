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
 void UART_Init(void);
 void UART_Clear(void);
 void UART_Get(void);
 void UART_Poll(void);
 void UART_Put(void);
 void UART_On(void);
 void LCD_Write_Data(void);
 void LCD_Write_Command(void);
 void LCD_Read_Data(void);
 void LCD_Delay(void);
 void Mega328P_Init(void);
 void ADC_Get(void);
 void EEPROM_Read(void);
 void EEPROM_Write(void);
 void SETB(void);
 void SETC(void);
 void CLEARB(void);
 void CLEARC(void);
 void SETBAUD(void);
 void ADC_Poll(void);
 void EEMEMORYH(void);
 void EEMEMORYL(void);
 void EEMEMORYR(void);

 int EELOCH;						//memory location for EEPROM storing High and Low
 int EELOCL;
 int BAUDH;
 int BAUDL;
 int USARTDATA;					//used to set or clear bits in UCSR0B or UCSR0C
 int Acc;						//Accumulator for ADC use
 unsigned char ASCII;			//shared I/O variable with Assembly
 unsigned char DATA;				//shared internal variable with Assembly
 char HADC;						//shared ADC variable with Assembly
 char LADC;						//shared ADC variable with Assembly
 char temp[5];					//string buffer for ADC output

 void UART_Puts(const char *str)	//Display a string in the PC Terminal Program
 {
	 while (*str)
	 {
		 ASCII = *str++;
		 UART_Put();
	 }
 }

 void LCD_Puts(const char *str)	//Display a string on the LCD Module
 {
	 while (*str)
	 {
		 DATA = *str++;
		 LCD_Write_Data();
	 }
 }

 void Banner(void)				//Display Tiny OS Banner on Terminal
 {
	 UART_Puts(MS1);
	 UART_Puts(MS2);
	 UART_Puts(MS4);
 }

 void HELP(void)						//Display available Tiny OS Commands on Terminal
 {
	 UART_Puts(MS3);
 }

 void LCD(void)						//Lite LCD demo
 {
	 LCD_Write_Command();
	 DATA = 0x08;					//Turns display and cursor off
	 LCD_Write_Command();
	 DATA = 0x02;					//Returns home
	 LCD_Write_Command();
	 DATA = 0x06;					//Shifts Cursor to the right
	 LCD_Write_Command();
	 DATA = 0x0f;					//Display on cursor blinking
	 LCD_Write_Command();
	 DATA = 0x0c;					//Display on cursor off
	 LCD_Write_Command();
	 
	 LCD_Puts(output);				//Puts the output sting onto the LCD

	 for (int i = 0; i < 16; i++){	//Loop to shift a total of 16
		 DATA = 0x1c;				//Shifts data to the right 1
		 ASCII = '\0';
		 for (int j = 0; j < 50; j++){
			 UART_Poll();
			 LCD_Delay();
		 }
		 if (ASCII != '\0')
		 break;
		 
		 LCD_Write_Command();		//Writes after shift
	 }
 }

 void Temperature_ADC(void)						//Lite Demo of the Analog to Digital Converter
 {
	 double r, t;									//Values used for calculation
	 int rn = 10000;									//Value used for calculation
	 ASCII = '\0';
	 
	 while(ASCII == '\0' )
	 {
		 temp[0x2]='.';									//Reserves index 2 in char array for '.'
		 temp[0x4]=' ';									//Reserves index 4 in char array for a space
		 temp[0x5]= 0;									//Does nothing for space 5
		 
		 ADC_Get();										//Gets the ADC value from assembly file
		 Acc = (((int)HADC)*0x100+(int)(LADC));			//Calculates given value being read from the circuit into a int
		 r = (10000.0 * Acc) / (1024.0 - Acc);			//Equation 1 from given sheet
		 t = (3950*298.15)/(298.15*log(r/rn) + 3950);	//Equation 2 from given sheet
		 t = t - 273.15;									//Convert to C from K
		 t = t*(9/5) + 32;								//Convert to F from C

		 int i = t*10;									//creates an integer i from the the float. Multiplies by 10 to move over decimal point
		 int j = t;										//explicit cast of float to int to
		 
		 temp[0x0] = i / 100 + 48;						//Calculates the first digit in the 2 digits left of the "."
		 temp[0x1] = j % 10 + 48;						//Calculates the second digit in the 2 digits left of the "."
		 temp[0x3] = i % 10 + 48;						//Calculates the values for the decimal

		 UART_Puts(temp);								//Puts the full char array of 'temp'
		 UART_Puts(MS7);									//Puts MS7 which is ºF

		 ASCII = '\0';
		 UART_Poll();
		 
		 temp[0x0] = 48;									//sets the first digit to 0 to clear
		 temp[0x1] = 48;									//sets the second digit to 0 to clear
		 temp[0x3] = 48;									//sets the decimal digit to 0 to clear
		 
		 _delay_ms(5000);
	 }
	 ASCII = '\0';
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