/*
 * Finals.c
 *
 * Created: 7/20/2018 1:22:19 AM
 * Author : Homepc
 */ 

#include "LIBS.h"
#include "uart/uart.h"
#include "main.h"
//#include "control.h"

enum DIRECTION {
	BACKWARD=0, FORWARD=1
};
enum DIRECTION direction;

enum STATES {
	PHASE_1, /* Reverse robot and go forward */
	PHASE_2, /* Make first turn and go forward */
	PHASE_3, /* Hit barrier OR detect middle IR Sensor, then make second turn and go forward */
};
enum STATES state = PHASE_1;

extern void forward(float dist_f);


extern volatile int cnt;
extern volatile int left;
extern volatile int Left;
extern volatile int right;
extern volatile int Right;


/* Interrupt service routine for Communication Devices */
//ISR(TIMER1_COMPA_vect) {
	//cnt++;
//}
//
//
///* Pin Change Interrupt (LEFT WHEEL ENCODER - PB0) */
//ISR(PCINT1_vect) //this corresponds to PORTB Pin 0
//{
	//left	= left + 1;
	//Left	= Left + 1;
//}
//
///* Pin Change Interrupt (RIGHT WHEEL ENCODER - PC0) */
//ISR(PCINT2_vect) //this corresponds to PORTC Pin 0
//{
	//right	= right + 1;
	//Right	= Right + 1;
//}








/* Initialize Global Settings */
void init (void) 
{
	/* Enable Global Interrupts */
	sei();
	
	/* Enable communication using UART */
	init_uart();

	/* Prevent ADC from shutting down by going into Power Saving mode */
	PRR0   |= (0 << PRADC);
}



int main(void)
{
	/* Pause for a second before starting Robot for the first time */
	_delay_ms(1000);
	
	/* Configure PORTB for Wheels */
	DDRB	= PORTB_CONFIG;
	
	/* Set login to move backwards */
	PORTB	  = (LW_BACKWARD) | (RW_BACKWARD);
	//PORTB	  = (LW_FORWARD) | (RW_FORWARD);
	
	direction = BACKWARD;
	
	/* Configure Port C as inputs for Wheel Encoders */
	DDRC	= PORTC_CONFIG;
		
	/* Configure Port D for Ultrasonic Sensor */
	DDRD	= PORTD_CONFIG;
	// TODO: Send out 10us pulses from PD7 - 0CR2A   - PCINT31
	

	
	/*
		Allow Interrupts for Pins PCINT[23:16] and PCINT[14:8].
		PCIE1 -> PCINT[15:8]  -> Enable Pins using PCMSK1
		PCIE2 -> PCINT[23:16] -> Enable Pins using PCMSK2
		PCMSK1 -> Enable Interrupts on PB0 (PCINT8)
		PCMSK2 -> Enable Interrupts on PC0 (PCINT16)
	*/
	PCICR	= (1<<PCIE2) | (1<<PCIE1);
	PCMSK1  = (1<<PCINT8);			/* Left Wheel */
	PCMSK2  = (1<<PCINT16);			/* Right Wheel */
	
	
	/* Start off Robot in Backward direction */
	OCR0A	= DEFAULT_LW_BACK;		/* Left Wheel - PB3 Duty Cycle */
	OCR0B	= DEFAULT_RW_BACK;		/* Left Wheel - PB4 Duty Cycle */
	
	
	/* 
		Initialize Globals:
		 - Interrupts
		 - Devices
		 - ADC
		 - Required Registers 
	*/
	init();
	
	while(1)
	{
		forward(25);
	}
	
}

