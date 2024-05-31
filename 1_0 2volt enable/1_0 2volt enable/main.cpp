/*
 * 1_0 2volt enable.cpp
 *
 * Created: 4/26/2024 3:35:45 PM
 * Author : gzongo
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/* Prog Master Code
	
	DDRC |= (1 << DDC1); // Setting the EN_2.1V pin
	//DDRH |= (1 << DDH5); // Data direction for the green led
	DDRB |= (1 << DDB6); // Data direction for the red led
	PORTC |= (1 << PC1); // Enable 2.1
	while (1)
	{
		//PORTH &= ~(1 << PH5); // D2 GREEN LED ON___|= set; &= reset
		PORTB &= ~(1 << PB6); // D16 RED LED ON
		_delay_ms(1000);
		//PORTH |= (1 << PH5); // D2 GREEN LED OFF
		PORTB |= (1 << PB6); // D16 RED LED OFF
		_delay_ms(1000);
	*/

	/* Prog Slave 1 and slave 2 Code */
	
	DDRB |= (1 << DDB0); // Data direction for the green led
	DDRB |= (1 << DDB1); // Data direction for the red led
	while (1)
	{
		PORTB &= ~(1 << PB0); // GREEN LED ON (D5 for Slave 2 D7 for Slave 1)
		PORTB &= ~(1 << PB1); // RED LED ON   (D6 for Slave 2 D7 for Slave 1)
		_delay_ms(100);
		PORTB |= (1 << PB0); // GREEN LED OFF (D5 for Slave 2 D7 for Slave 1)
		PORTB |= (1 << PB1); // RED LED OFF   (D6 for Slave 2 D7 for Slave 1)
		_delay_ms(100);

	}
}
