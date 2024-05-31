/*
 * TB3216 Receive Control Commands.c
 *
 * Created: 5/21/2024 3:20:12 PM
 * Author : zongr
 */ 

#define F_CPU 16000000UL
#define USART0_BAUD_RATE 9600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

void USART0_sendChar(char c);		// Declaration for sending characters to USART0
void USART0_sendString(char *str);  // Declaration for sending Characters from a string to USART0_sendChar
char USART0_read(void);				// Declaration for reading a character from the USART0
void execute(char *command);		// Executes sets of actions depending on the command
void blink_Leds(void);
void red_LED_ON(void);
void red_LED_OFF(void);
void green_LED_ON(void);
void green_LED_OFF(void);
void LED_init(void);

void LED_init(void){
	DDRH |= (1 << DDH6)		//Setting data direction for red LED to "output"
	DDRH |= (1 << DDH5)		//Setting data direction for green LED to "output"
}

void red_LED_OFF(){
	PORTH |= (1 << PH6);
}

void red_LED_ON(){
	PORTH &= ~(1 << PH6);
}

void green_LED_OFF(){
	PORTH |= (1 << PH5);
}

void green_LED_ON(){
	PORTH &= ~(1 << PH5);
}

void blink_Leds(){
	for (int i = 0; i < 11; i++){
		PORTH &= ~(1 << PH5); // GREEN LED ON
		PORTH |= (1 << PH6); // RED LED OFF
		_delay_ms(200);
		PORTH |= (1 << PH5); // GREEN LED OFF
		PORTH &= ~(1 << PH6); // RED LED ON
		_delay_ms(200);
		if (i == 10){
					PORTH |= (1 << PH5); // GREEN LED OFF
					PORTH |= (1 << PH6); // RED LED ON
		}
		
	}
}

void USART0_init(void){
	UBRR0 = ((float)(F_CPU / (16 * (float)USART0_BAUD_RATE)) - 1);
	
	DDRE &= ~(1 << DDE0);		// sets the data direction of RX to input
	UCSR0B |= (1 << RXEN0);		// Enable RX for USART0		
	DDRA |= (1 << DDA1);		//Set data direction to output on PA1
	PORTA &= ~(1 << PA1);		// Set PA1 to 0 (low) for RS232_00 USART0
		
	DDRE |= (1 << DDE1);		// sets the data direction of TX to output
	UCSR0B |= (1 << TXEN0);		// Enable TX for USART0
	DDRA |= (1 << DDA2);		//Set data direction to output on PA2
	PORTA |= (1 << PA2);		// Set PA2 to 1 (high) for RS232_01_02 USART0
}

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

void USART0_sendString(char *str){
	for(size_t i = 0; i < strlen(str); i++)	{
		USART0_sendChar(str[i]);
	}
}

char USART0_read(void){
	while (!(UCSR0A & (1 << RXC0)))
	{
		;
	}
	return UDR0;
}

void execute(char *command)
{
	if(strcmp(command, "rON") == 0){
		USART0_sendString("Red LED is now ON.\r\n");
		red_LED_ON();
	}
	else if (strcmp(command, "rOFF") == 0){
		USART0_sendString("Red LED is now OFF.\r\n");
		red_LED_OFF();
	}else if (strcmp(command, "gON") == 0){
		USART0_sendString("Green LED is now ON.\r\n");
		green_LED_ON();
	} else if (strcmp(command, "gOFF") == 0){
	USART0_sendString("Green LED is now OFF.\r\n");
	green_LED_OFF();
	} else if (strcmp(command, "blink") == 0){
		USART0_sendString("Green LED and Red LED are now blinking.\r\n");
		blink_Leds();
	} else{
		USART0_sendString("Your Command is undefined.\r\n");
	}
}

int main(void)
{
	
	char command[8];
	int index = 0;
	char c;
	
	USART0_init();
	LED_init();
    while (1) 
    {
		c = USART0_read();
		if (c != '\n' && c !='\r'){
			command[index++] = c;
			if (index > 8){
				index = 0;
			}
		}
		if (c == '\n'){
			command[index] = '\0';
			index = 0;
			execute(command);
		}
		
    }
}

