/*
 * TB3216 Receive Control Commands.c
 *
 * Created: 5/30/2024 10:10:12 AM
 * Author : zongr
 */ 

#define F_CPU 16000000UL
#define USART0_BAUD_RATE 9600
#define LEDR_PIN PH6
#define LEDG_PIN PH5
#define int_SWITCH_PIN PD3  // Test-Status Switch
#define pcint_SWITCH_PIN PE7 // MCU-Status Switch
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/sleep.h>
#include <util/delay.h>

void USART0_sendChar(char c);		// Declaration for sending characters to USART0
void USART0_sendString(char *str);  // Declaration for sending Characters from a string to USART0_sendChar
char USART0_read(void);				// Declaration for reading a character from the USART0
void execute(char *command);		// Executes sets of actions depending on the command
void blink_Leds(int x);
void red_LED_ON(void);
void red_LED_OFF(void);
void green_LED_ON(void);
void green_LED_OFF(void);
void LED_init(void);
void SWITCH_init(void);
void INTERRUPTS_init(void);
void USART0_init(void);
void sleep_init(void);

void LED_init(void){
	DDRH |= (1 << DDH6);		//Setting data direction for red LED to "output"
	DDRH |= (1 << DDH5);		//Setting data direction for green LED to "output"
}
void SWITCH_init(void){
	DDRD &= ~(1 << DDD3);
	PORTD |= (1 << int_SWITCH_PIN); //Enabling PULL-UP on the switch for INT0
	
	DDRE &= ~(1 << DDE7);
	PORTE |= (1 << pcint_SWITCH_PIN); //Enabling PULL-UP on the switch for PCINT0
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
void INTERRUPTS_init(void){
	
	EICRA |= (1 << ISC30);	// with ISC30 and ISC31 both set to 1, the rising edge of INT3 generates asynchronously an interrupt request
							// When changing the ISCn1/ISCn0 bits, the interrupt must be disabled by clearing its Interrupt Enable bit
	EICRA |= (1 << ISC31);	// in the EIMSK Register. Otherwise an interrupt can occur when the bits are changed.
	
	EIMSK |= (1 << INT3);	// enables interrupt 3 (INT6)
	
	// EIFR |= (1 << INTF0);	// Clearing the interrupt flag (The flag is also cleared when the interrupt routine is executed.)
	
	PCICR |= (1 << PCIE0);	// Enabling pin change interrupt 0 (PCICR0)
	
	PCMSK0 |= (1 << pcint_SWITCH_PIN); //enables the Pin Change Interrupt (PB4)
	
	sei();					// sets global interrupts by writing a 1 to the I-bit (interrupt-bit) in the Status Register (SREG)
}
void sleep_init(void){
	/* SMCR &= ~(1 << SM0);		|
	SMCR |= (1 << SM1);			| This is bit manipulating the Sleep Mode Control Register to activate the "Power-Down" Sleep Code
	SMCR &= ~(1 << SM2);		|
	*/
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // This is doing the same thing using a library
}

void red_LED_OFF(){
	PORTH |= (1 << LEDR_PIN);
}
void red_LED_ON(){
	PORTH &= ~(1 << LEDR_PIN);
}
void green_LED_OFF(){
	PORTH |= (1 << LEDG_PIN);
}
void green_LED_ON(){
	PORTH &= ~(1 << LEDG_PIN);
}
void blink_Leds(int x){
	green_LED_ON(); // GREEN LED ON
	red_LED_OFF(); // RED LED OFF
	for (int i = 0; i < x; i++){
		PORTH ^= (1 << LEDG_PIN); // GREEN LED ON
		PORTH ^= (1 << LEDR_PIN); // RED LED OFF
		_delay_ms(200);
	}
	green_LED_ON();
	red_LED_ON();
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
		blink_Leds(20);
		} else if (strcmp(command, "sleep") == 0){
		USART0_sendString("Device in sleep mode.\r\n");
		blink_Leds(3);
		red_LED_OFF();
		green_LED_OFF();
		sleep_mode();
		}else{
		USART0_sendString("Your Command is undefined.\r\n");
	}
}

ISR (INT3_vect){
	if ((PIND & (1 << int_SWITCH_PIN))){
		//sleep_disable();
		USART0_sendString("Device is awake.\r\n");
		blink_Leds(5);
		green_LED_ON();
		red_LED_ON();
	}
}

int main(void)
{
	
	char command[8];
	int index = 0;
	char c;
	INTERRUPTS_init();
	USART0_init();
	LED_init();
	SWITCH_init();
	sleep_init();
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

