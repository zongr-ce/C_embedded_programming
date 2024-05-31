/*
 * Atmel interrupts.c
 *
 * Created: 5/29/2024 04:26:58 PM
 * Author : zongr
 */ 
#define F_CPU 16000000UL
#define LEDG_PIN PH5
#define LEDR_PIN PB6
#define int6_SWITCH_PIN PE6
#define pcint4_SWITCH_PIN PB4
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>


void blink_Leds(int i);
void red_LED_ON(void);
void red_LED_OFF(void);
void green_LED_ON(void);
void green_LED_OFF(void);
void LED_init(void);
void SWITCH_init(void);
void INTERRUPTS_init(void);

void red_LED_OFF(){
	PORTB |= (1 << LEDR_PIN);
}
void red_LED_ON(){
	PORTB &= ~(1 << LEDR_PIN);
}
void green_LED_OFF(){
	PORTH |= (1 << LEDG_PIN);
}
void green_LED_ON(){
	PORTH &= ~(1 << LEDG_PIN);
}
void blink_Leds(int i){
	for (int a = 0; a < i; a++){
		PORTH &= ~(1 << LEDG_PIN); // GREEN LED ON
		PORTB |= (1 << LEDR_PIN); // RED LED OFF
		_delay_ms(5);
		PORTH |= (1 << LEDG_PIN); // GREEN LED OFF
		PORTB &= ~(1 << LEDR_PIN); // RED LED ON
		_delay_ms(5);
		if (i == a-1){
			PORTB |= (1 << LEDG_PIN); // GREEN LED OFF
		}
	}
}

void LED_init(void){
	DDRB |= (1 << DDB6);		//Setting data direction for red LED to "output"
	DDRH |= (1 << DDH5);		//Setting data direction for green LED to "output"
}
void SWITCH_init(void){
	DDRE &= ~(1 << DDE6);
	PORTE |= (1 << int6_SWITCH_PIN); //Enabling PULL-UP on the switch for INT0
	
	DDRB &= ~(1 << DDB4);
	PORTB |= (1 << pcint4_SWITCH_PIN); //Enabling PULL-UP on the switch for PCINT0
}
void INTERRUPTS_init(void){
	
	EICRB |= (1 << ISC60);	// with ISC00 and ISC01 both set to 1, the rising edge of INT0 generates asynchronously an interrupt request
							// When changing the ISCn1/ISCn0 bits, the interrupt must be disabled by clearing its Interrupt Enable bit
	EICRB |= (1 << ISC61);	// in the EIMSK Register. Otherwise an interrupt can occur when the bits are changed.
	
	EIMSK |= (1 << int6_SWITCH_PIN);	// enables interrupt 6 (INT6)
	
	// EIFR |= (1 << INTF0);	// Clearing the interrupt flag (The flag is also cleared when the interrupt routine is executed.)
	
	PCICR |= (1 << PCIE0);	// Enabling pin change interrupt 0 (PCICR0)
	
	PCMSK0 |= (1 << pcint4_SWITCH_PIN); //enables the Pin Change Interrupt (PB4)
	
	sei();					// sets global interrupts by writing a 1 to the I-bit (interrupt-bit) in the Status Register (SREG)
}
void sleep_init(void){
	/* SMCR &= ~(1 << SM0);		|
	SMCR |= (1 << SM1);			| This is bit manipulating the Sleep Mode Control Register to activate the "Power-Down" Sleep Code
	SMCR &= ~(1 << SM2);		|
	*/
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // This is doing the same thing using a library
}

ISR (INT6_vect){
	if ((PINE & (1 << int6_SWITCH_PIN))){
		red_LED_ON();
		green_LED_ON();
		//red_LED_OFF();
		//_delay_ms(100);
		//green_LED_OFF();
		//red_LED_OFF();
	}
}
/*ISR (PCINT0_vect){
	if(PINB & (1 << pcint4_SWITCH_PIN)){
		red_LED_ON();
		//green_LED_OFF();
		//_delay_ms(100);
		//red_LED_OFF();
		green_LED_OFF();
	}
}*/

int main(void)
{
	LED_init();
	SWITCH_init();
	INTERRUPTS_init();
	sleep_init();
	
    while (1) 
    {
		if ((PINB & (1 << pcint4_SWITCH_PIN))){
			red_LED_OFF();
			green_LED_OFF();
			//sleep_enable; // enable sleep.
			//sleep_disable; // is called when the program continues as usual
			//sleep_cpu; // Invokes the SLEEP instruction
			sleep_mode(); // invokes sleep_enable(), then sleep_cpu()
			
		}
    }
}
