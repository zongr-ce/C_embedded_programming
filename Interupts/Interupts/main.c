/*
 * Interupts.c
 *
 * Created: 5/23/2024 10:22:24 AM
 * Author : zongr
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
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
	DDRH |= (1 << DDH6);		//Setting data direction for red LED to "output"
	DDRH |= (1 << DDH5);		//Setting data direction for green LED to "output"
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

void I_init(void){
	TCNT1 = 65535 - (F_CPU/1024); // selects timer counter 1
	TCCR1B = (1 << CS10) | (1 << CS12);  // selects clk i/o 1024 from the prescaler
	
	TCCR1A |= () 0x00;
	
	TIMSK1 |= (1 << TOIE1);  // Timer interrupt mask register
	
	sei();					// enables global interrupts
}

ISR (TIMER1_OVF_vect){
	PORTH ^= (1 << PH6);
	TCNT1 = 65535 - (F_CPU/1024);
}

int main(void)
{
    LED_init();
	I_init();

    while (1) 
    {
		;
    }
}

