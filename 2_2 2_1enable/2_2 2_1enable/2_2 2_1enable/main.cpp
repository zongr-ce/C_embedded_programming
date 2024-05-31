/*
 * 2_0 2_1volt enable.cpp
 *
 * Created: 4/19/2024 3:46:58 PM
 * Author : gzongo
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	DDRC |= (1 << DDC1); // Setting the EN_2.1V pin
	DDRH |= (1 << DDH5); // Data direction for the green led
	DDRB |= (1 << DDH6); // Data direction for the red led
	PORTC |= (1 << PC1); // Enable 2.1
    while (1) 
    {
		PORTH &= ~(1 << PH5); // D19 GREEN LED ON___|= set; &= reset
		PORTB &= ~(1 << PH6); // D17 RED LED ON
		_delay_ms(1000);
		PORTH |= (1 << PH5); // D19 GREEN LED OFF
		PORTB |= (1 << PH6); // D17 RED LED OFF
		_delay_ms(1000);

    }
}

