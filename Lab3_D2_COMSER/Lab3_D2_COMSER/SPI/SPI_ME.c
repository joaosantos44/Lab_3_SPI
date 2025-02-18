/*
 * SPIM.c
 *
 * Created: 11/02/2025 16:40:26
 *  Author: joaos
 */ 

#include "SPI_ME.h"

void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClocksPhase){
	//PB2 = SS; PB3 = MOSI; PB4 = MISO; PB5 = SCK
	
	//Modo Maestro
	if(sType & (1<<MSTR)){
		DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
		DDRB &= ~(1 << DDB4);
		SPCR |= (1 << MSTR);
		
		uint8_t temp = sType & 0b00000111;
		switch(temp){
			case 0: //DIV2
				SPCR &= ~((1 << SPR1) | (1 << SPR0));
				SPSR |= (1 << SPI2X);
			break;
			case 1: //DIV 4
				SPCR &= ~((1 << SPR1) | (1 << SPR0));
				SPSR &= ~(1 << SPI2X);
			break;
			case 2: //DIV 8
				SPCR |= (1 << SPR0);
				SPCR &= ~(1 << SPR1);
				SPSR |= (1 << SPI2X);
			break;
			case 3: //DIV 16 
				SPCR |= (1 << SPR0);
				SPCR &= ~(1 << SPR1);
				SPSR &= ~(1 << SPI2X);
			break;
			case 4: //DIV 32
				SPCR &= ~(1 << SPR0);
				SPCR |= (1 << SPR1);
				SPSR |= (1 << SPI2X);
			break;
			case 5: //DIV 64
				SPCR &= ~(1 << SPR0);
				SPCR |= (1 << SPR1);
				SPSR &= ~(1 << SPI2X);
			break;
			case 6: //DIV 128
				SPCR |= (1 << SPR0);
				SPCR |= (1 << SPR1);
				SPSR &= ~(1 << SPI2X);
			break;
			
		}	
	}
	else{
		//SLAVE
		DDRB &= ~((1 << DDB2) | (1 << DDB3) | (1 << DDB5));
		DDRB |= (1 << DDB4);
		SPCR &= ~(1 << MSTR);
	}
	// Enable Data Order, Clock Polarity, Clock Phase
	SPCR |= (1 << SPE) | sDataOrder | sClockPolarity | sClocksPhase;
	
}

static void spiReceiveWait(){
	
	while (!(SPSR & (1 << SPIF))); //Wait for Data Recieve Complete
}



void spiWrite(uint8_t dat){
	//Writa data to SPI bus
	SPDR = dat;
}

unsigned spiDataRead(){
	//Check whether the data is ready to read
	
	if((SPSR & (1 << SPIF))){
		return 1;
	}else{
		return 0;
	}
}

char spiRead(){
	//Read the received data
	while (!(SPSR & (1 << SPIF))); //Wait for Data Recieve Complete
	return(SPDR);				   //Read the received data from the buffer
	
}
