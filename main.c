/*
 * UART_TX_TEST.cpp
 *
 * Created: 3/11/2020 5:44:28 PM
 * Author : micha
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR ((F_CPU/16UL/BAUD)-1)
#define LED_ON	PORTB |= (1<<PORTB5);
#define LED_OFF PORTB &= ~(1<<PORTB5);
#define LED_TOGGLE PINB |= (1<<PINB5);
#define RX_BUFFER_SIZE 255

char GPRMC[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
char charData;
char rxBuffer[RX_BUFFER_SIZE];
char rxChar;
char lat[7];
char lon[8];
char rmcString[59];
uint8_t rxReadPos = 0;
uint8_t rxWritePos = 0;
uint8_t buildPos = 0;
bool rmcBuilt = false;
char ret;

void USART_Transmit(unsigned char);
void USART_Init(void);
void GPS_Init(void);
char getChar(void);
void buildString(unsigned char);
void latExtract(void);
void lonExtract(void);


int main(void)
{	
	USART_Init();
	GPS_Init();
	DDRB |= (1<<DDB5); //Set LED as output
	
	LED_OFF;
	
	while(1)
	{
		if(rxWritePos != rxReadPos)
		{
			charData = getChar();
		}
		USART_Transmit(charData);

//		buildString(charData);
			
		if(rmcBuilt)
		{
			rmcBuilt = false;
		}
		//Extract values

	}
}
void USART_Init(void)
{
	UBRR0H = (MYUBRR>>8);
	UBRR0L = MYUBRR;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
	sei();
}
void USART_Transmit(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}
void GPS_Init(void)
{
	for(int i=0; i<=50; i++)
	{
		USART_Transmit(GPRMC[i]);
	}
}

ISR(USART0_RX_vect)
{
	//Read in data to buffer
	rxChar = UDR0;
	rxBuffer[rxWritePos] = rxChar; //PD0
	rxWritePos++;
	if(rxWritePos >= RX_BUFFER_SIZE)
	{
		rxWritePos = 0;
	}
}

char getChar(void)
{
	ret = rxBuffer[rxReadPos];
	
	rxReadPos++;
	
	if(rxReadPos >=  RX_BUFFER_SIZE)
	{
		rxReadPos = 0;
	}
	
	return ret;
}
void buildString(unsigned char c)
{
	if(c == '\x0a')
	{
		rmcBuilt = true;
		
		buildPos = 0;
	}
	else
	{
		rmcString[buildPos] = c;
		buildPos++;
	}
}
