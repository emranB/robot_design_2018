/*
 * uart.h
 *
 * Created: 6/29/2018 8:45:13 PM
 *  Author: Group 3
 */ 

int uart_putchar(char c,FILE *stream);
int uart_getchar(FILE *stream);
void init_uart(void);