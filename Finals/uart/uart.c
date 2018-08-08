/*
 * uart.c
 *
 * Created: 6/29/2018 8:45:04 PM
 *  Author: Group 3
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include "uart.h"


FILE mystdout = FDEV_SETUP_STREAM(uart_putchar,NULL,_FDEV_SETUP_WRITE);
FILE mystdin = FDEV_SETUP_STREAM(NULL,uart_getchar,_FDEV_SETUP_READ);

int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0=c;
	return 0;
}

int uart_getchar(FILE *stream)
{
	loop_until_bit_is_set(UCSR0A,RXC0);
	return UDR0;
}

void init_uart(void)
{
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UBRR0=7;
	stdout= &mystdout;
	stdin = &mystdin;
}