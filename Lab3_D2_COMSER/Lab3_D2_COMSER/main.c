//********************************************************************************************************************************************
/*
Universidad del Valle de Guatemala
IE3054: Electronica Digital 2
Lab_1
Autor; Joao Santos
Lab_3
Hardware: Atemega328P
Creado: 11/02/2025
Ultima modificacion: 11/02/2025
*/
//********************************************************************************************************************************************

//********************************************************************************************************************************************
// Librerias
//********************************************************************************************************************************************

#define F_CPU 16000000

#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#include "SPI/SPI_ME.h"

//********************************************************************************************************************************************
// Variables Globales
//********************************************************************************************************************************************

volatile char indic;
//volatile uint8_t valorADC = 0;
//volatile char valorADC;
uint8_t contador = 0;
uint8_t valorSPI = 0;
//char valorSPI = 0;

//********************************************************************************************************************************************
// Declarar Funciones
//********************************************************************************************************************************************

void initUART9600(void);
void setup(void);
void refreshPORT(uint8_t valor);
void MandarP(char letra);
char ReceivP(void);
void CadenChar(const char *str);
void Binario(uint8_t value);
void GuardarBinario(uint8_t value);
int InterDeTeclas();


//********************************************************************************************************************************************
// Main
//********************************************************************************************************************************************


int main(void)
{
	setup();

	 while (1) {
		 CadenChar("Introduce un numero seguido de '+'- si quieres su binario:\r\n");
		CadenChar("Introduce una 'x' si quieres saber el pot 1:\r\n");
		CadenChar("Introduce una 'c' si quieres saber el pot 2:\r\n");
	
		 uint8_t numerito = InterDeTeclas(); // Leer número
		 // Verificar el valor convertido
		 //CadenChar("Valor convertido: ");
		 char str[4];
		 itoa(numerito, str, 10);
		 //CadenChar(str);
		 //CadenChar("\r\n");

		 // Imprime el número en binario
		 CadenChar("En binario: 0b");
		 Binario(numerito);
		 refreshPORT(numerito);

		
		/* 
		_delay_ms(5);
		despues de probar que funcione lo del lab para fer si jala 
		el SPI esto es lo que se supone que tenemos que hacer
		
		spiWrite(numerito);
		
		esto es un numero en binario asi que el esclavo lo debe de leer directamente
		te estoy mandando el numero no se si asi furule
		asi que en la interupcio manda este valor
		
		*/
		
		/*valorSPI = spiRead();
		_delay_ms(5);
		
		//refreshPORT(valorSPI);
		CadenChar(valorSPI);
		_delay_ms(5);
		
		PORTB |= (1 << PORTB2);
		
		_delay_ms(250);*/
	
    }
}

//********************************************************************************************************************************************
// Setup
//********************************************************************************************************************************************

void setup(void){
	
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD6) | (1 << DDD6) | (1 << DDD7);
	DDRB |= (1 << DDB0) | (1 << DDB1);
	DDRD &= ~((1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD6) | (1 << DDD6) | (1 << DDD7));
	DDRB &= ~((1 << DDB0) | (1 << DDB1));
	
	spiInit(SPI_MAESTRO_OSC_DIV8, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	initUART9600();
	
	sei(); 
}

//********************************************************************************************************************************************
// Sub_Funciones
//********************************************************************************************************************************************

void initUART9600(void){
	uint16_t ubrr = F_CPU / 16 / 9600 - 1;
	UBRR0H = (ubrr >> 8);   // Parte alta del baud rate
	UBRR0L = ubrr;          // Parte baja del baud rate
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilita TX y RX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos
}

void refreshPORT(uint8_t valor){
	if(valor & 0b10000000){
		PORTB |= (1 << PORTB0);
	}else{
		PORTB &= ~(1 << PORTB0);
	}
	if(valor & 0b01000000){
		PORTB |= (1 << PORTB1);
		}else{
		PORTB &= ~(1 << PORTB1);
	}
	if(valor & 0b00100000){
		PORTD |= (1 << PORTD2);
		}else{
		PORTD &= ~(1 << PORTD2);
	}
	if(valor & 0b00010000){
		PORTD |= (1 << PORTD3);
		}else{
		PORTD &= ~(1 << PORTD3);
	}
	if(valor & 0b00001000){
		PORTD |= (1 << PORTD4);
		}else{
		PORTD &= ~(1 << PORTD4);
	}
	if(valor & 0b00000100){
		PORTD |= (1 << PORTD5);
		}else{
		PORTD &= ~(1 << PORTD5);
	}
	if(valor & 0b00000010){
		PORTD |= (1 << PORTD6);
		}else{
		PORTD &= ~(1 << PORTD6);
	}
	if(valor & 0b00000001){
		PORTD |= (1 << PORTD7);
		}else{
		PORTD &= ~(1 << PORTD7);
	}
}


// Envía un caracter
void MandarP(char letra){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = letra;
}

//recive un caracter
char ReceivP(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

// Envía una cadena de texto
void CadenChar(const char *str) {
    while (*str) {
        MandarP(*str++);
    }
}

// Convierte un número a binario
void Binario(uint8_t valor) {
	for (int i = 7; i >= 0; i--) {
		MandarP((valor & (1 << i)) ? '1' : '0');
	}
	CadenChar("\r\n");
}


int InterDeTeclas() {
	char buffer[4] = {0};   // Almacena hasta 3 dígitos
	uint8_t index = 0;
	char received;

	while (1) {
		while (!(UCSR0A & (1 << RXC0)));  // Espera un dato (¡Aquí puede trabarse!)
		
		received = UDR0;  // Lee el dato recibido
		MandarP(received); // Imprime lo que recibe (eco)

		// Si encuentra '+', termina
		if (received == '+') {
			break;
		}

		// Acepta solo dígitos (0-9)
		if (received >= '0' && received <= '9' && index < 3) {
			buffer[index++] = received;  // Guarda el número
		}
		if (received == 'x')
		{
			for (int i=0; i<=2; i++)
			{
				PORTB &= ~(1 << PORTB2);
				spiWrite('x');
				_delay_ms(250);
				valorSPI = spiRead();
				MandarP(valorSPI);
				MandarP('a');

				PORTB |= (1 << PORTB2);
				//_delay_ms(250);
				
			}
			CadenChar(" \r\npot1 \r\n");
			break;
		}
	}

	return (uint8_t)atoi(buffer);  // Convierte a entero
}

//********************************************************************************************************************************************
// Interrupciones
//********************************************************************************************************************************************
/*
ISR(USART_RX_vect){
	indic = UDR0;
	
}


ISR(SPI_STC_vect){
	uint8_t spiValor = SPDR;
	MandarP(spiValor);
}
*/