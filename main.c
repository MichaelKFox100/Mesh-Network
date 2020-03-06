/*
 * DAMNTest1.c
 *
 * Created: 2/10/2020 6:42:07 PM
 * Author : micha
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU	16000000UL
#define BAUD	9600
#define MYUBRR	((F_CPU/16/BAUD) - 1)
#define LED_ON	PORTB |= (1<<PORTB5);
#define LED_OFF PORTB &= ~(1<<PORTB5);
#define LED_TOGGLE PINB |= (1<<PINB5);
#define RX_BUFFER_SIZE 128

char rxBuffer[RX_BUFFER_SIZE];
uint8_t rxReadPos = 0;
uint8_t rxWritePos = 0;

char getChar(void);
char peekChar(void);

int main(void)
{
	//Baud Rate init
	UBRR0H =  (MYUBRR >> 8);
	UBRR0L = MYUBRR;
	//Enable RX & TX, RX Interrupt
	UCSR0B |= (1 << RXEN0) | (1<<TXEN0) | (1 << RXCIE0);
	//Char Size
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01); //8 bit
	//Turn on external interrupts
	sei();
	DDRB |= (1<<DDB5); //Set LED as output
	
	//PD0 = USART RXD
	

    //Parent Loop
    while (1) 
    {
		char coorData = getChar();
		if(coorData == 0x0)
		{
			LED_OFF;
			_delay_ms(500);
		}
		else
		{
			LED_TOGGLE;
		}
    }
}

//Interrupt fires when new data enters data reg
ISR(USART0_RX_vect)
{
	//Read in data to buffer
	rxBuffer[rxWritePos] = UDR0;
	
	rxWritePos++;
	
	if(rxWritePos >= RX_BUFFER_SIZE)
	{
		rxWritePos = 0;
	}
}

//In case of misalignment of data
char getChar(void)
{
	char ret = '\0';
	
	if(rxReadPos != rxWritePos)
	{
		ret = rxBuffer[rxReadPos];
		rxReadPos++;
		
		if(rxReadPos >=  RX_BUFFER_SIZE)
		{
			rxReadPos = 0;
		}
	}
	
	return ret;
}

char peekChar(void)
{
	char ret = '\0';
	
	if(rxReadPos != rxWritePos)
	{
		ret = rxBuffer[rxReadPos];
	}
	
	return ret;
}
