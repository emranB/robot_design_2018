/*
 * forward.c
 *
 * Created: 7/20/2018 1:49:46 AM
 *  Author: Homepc
 */ 
#include "LIBS.h"

extern float dist;
extern void Control(void);


/* 
	Set PWM modes for wheels
	Pass Control to function 'control()'
*/
void forward(float dist_f)
{
	TCNT0	= 0;
	
	/*
		COM0A1 COM0A0 = 1 0 : Clear OC0A on Compare Match (non-inverting mode) - LEFT Wheel
		COM0B1 COM0B0 = 1 0 : Clear OC0B on Compare Match (non-inverting mode) - RIGHT Wheel
		WGM01  WGM00  = 1 1 : Fast PWM, TOP = OCRnx, TOV Flag set at MAX
	*/
	TCCR0A	= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
	/*
		Set pre-scalar to:
			CS02 - CS01 - CS00 = 0 1 0 = 8
			CS02 - CS01 - CS00 = 0 1 1 = 64
			CS02 - CS01 - CS00 = 1 0 0 = 256
	*/
	TCCR0B	= (0<<CS02) | (1<<CS01) | (1<<CS00);	// 64
	
	
	/* Wait until the required distance has been traveled */
	while (dist <= dist_f)
	{		
		Control();
	}

}