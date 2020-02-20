#define F_CPU 16000000UL
#include <avr/io.h>
#include "Communication.c"
#include <avr/interrupt.h>
//Battery Level (Drone and Charger)
//Analog : 5V -> Max_Resolution = 10 bits or 12 bits -> Pins A0 - A5

int Battery_Level;
int Battery_Percent;


ISR(0x002A) //ADC Conversion Complete Interrupt
{
	Battery_Level = ADC;
}

void ADCInit()
{
		ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Set ADC Prescaler to 128 125kHz
		ADMUX |= (1 << REFS0); //Set ADC Reference to AVCC
		ADMUX |= (1 << ADLAR); //Set ADC Left Adjust
		ADCSRA |= (1 << ADATE); //Set ADC to Free-Running
		ADCSRA |= (1 << ADIE); //Enable ADC Interrupt
		sei(); //Enable interrupts
}

void PWM()
{
	
}

void Systems()
{
	ADCInit();
	int Step_Down_Ratio;
	int Battery_Capacity;
	Battery_Level = 5 * Step_Down_Ratio;
	Battery_Percent = (Battery_Level / Battery_Capacity) * 100;
	
}
