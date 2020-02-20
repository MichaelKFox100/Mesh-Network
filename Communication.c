/*
 * SeniorDesign.c
 *
 * Created: 2/6/2020 11:42:42 AM
 * Author : Donovan Rowzee
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>

//SPI Initializer

void SPI_MasterInit(void)
{
	//Set MOSI and SCK Output
	DDRB = (1 << DDB3)|(1<<DDB5);
	//Enable SPI, Master, and set clock rate to fck/16
	SPCR0 = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
}

char[] SPI_MasterTransmit(char[] TxData)
{
	char[sizeof(TxData)] RxData; 
	//Transmit
	for(int i = 0; i < sizeof(TxData); i++)
	{
		SPDR0 = TxData[i];
		//Wait for completion
		while(!(SPSR0 & (1<<SPIF)));	
		RxData[i] = SPDR0;
	}
	return RxData;
}

char[] Communication(char[] input)
{
	char[sizeof(input)] Returned_Data;
    while (1) 
    {
		if(input)
		{
			Returned_Data = SPI_MasterTransmit(input);
		}
		else
		{
			Returned_Data = SPI_MasterTransmit(0x00); //Receive
		}
    }
	return Returned_Data;
}
