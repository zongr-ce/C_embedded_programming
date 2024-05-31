/*
 * USART0 Hello World.cpp
 *
 * Created: 5/21/2024 1:15:34 PM
 * Author : zongr
 */
#define F_CPU 16000000UL
#define USART0_BAUD_RATE 9600
/* There are many ways to set the BAUD rate
	first way: USART0.BAUD = (uint16_t) USART0_BAUD_RATE(9600);
	Second way to use only if you know the clock speed of the system: 
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);
	*/

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void USART0_sendChar(char c);		// Declaration for sending characters to USART0
void USART0_sendString(char *str);  // Declaration for sending Characters from a string to USART0_sendChar
void USART0_init(void);				// Declaration for initializing USART0

// This next function enables the Transmitter (TX) and sets the BAUD rate USART0
void USART0_init(void){
	//	USART0.CTRLB |= USART_TXEN_bm; This line is identical to the one bellow, but more complicated
	UBRR0 = ((float)(F_CPU / (16 * (float)USART0_BAUD_RATE)) - 1); // Equation is found on page 203
	DDRE |= (1 << DDE1);		// sets the data direction of TX to output
	UCSR0B |= (1 << TXEN0);		// Enable TX for USART0
	
	DDRA |= (1 << DDA2);		//Set data direction to output on PA2
	PORTA |= (1 << PA2);		// Set PA2 to 1 (high) for RS232 USART0
}

// This next function sends the String "Hello World" or any other string. In general, a String is sent Character by Character.
void USART0_sendChar(char c){
	/*This next loop checks if the previous transmission is completed by checking the USARTn.STATUS register.
	This is done by polling the status register until the empty flag "STATUS.DREIF" is set.
	*/
	while (!(UCSR0A & (1 << UDRE0)))
	{
		;
	}
	UDR0 = c;
}

// This function sends the each character of the String to the UART0_sendChar function
void USART0_sendString(char *str){
	for(size_t i = 0; i < strlen(str); i++)	{
		USART0_sendChar(str[i]);
	}
}


int main(void)
{
	char usart_str[] = "Hello World!\r\n";
	USART0_init();
	
	while (1){
		USART0_sendString(usart_str);
		_delay_ms(500);
	}
}
