/*
 * control.c
 *
 * Created: 7/20/2018 1:56:02 AM
 *  Author: Homepc
 */ 
#include "LIBS.h"
#include "main.h"
#include "uart/uart.h"
#include "control.h"
#include "helperFuncs.h"

enum STATES {
	PHASE_1, /* Reverse robot and go forward */
	PHASE_2, /* Make first turn and go forward */
	PHASE_3, /* Hit barrier OR detect middle IR Sensor, then make second turn and go forward */
	PHASE_4, /* Start Hugging reflective tape */
	PHASE_5, 
	PHASE_6, 
};

extern enum STATES state;
extern void HandleIRSensors(int left, int middle, int right);

void CorrectError(int LWE_linear_vel, int RWE_linear_vel);

extern void ReverseDirection(void);
extern void GoForward(void);
extern void GoBackward(void);
extern void TurnRight(void);
extern void TurnLeft(void);
extern void TurnHalfRight(void);
extern void TurnHalfLeft(void);
extern void InitUltrasonicSensor(void);
extern void LeftWheelHugTape(void);
extern void RightWheelHugTape(void);





/* Pin Change Interrupt (LEFT WHEEL ENCODER - PB0) */
ISR(PCINT1_vect) //this corresponds to PORTB Pin 0
{
	left	= left + 1;
	Left	= Left + 1;
}

/* Pin Change Interrupt (RIGHT WHEEL ENCODER - PC0) */
ISR(PCINT2_vect) //this corresponds to PORTC Pin 0
{
	right	= right + 1;
	Right	= Right + 1;
}

ISR(TIMER1_COMPA_vect)
{
	cnt++;
}

void transmit_c(int iterations)
{
	int i = 0;
	volatile int msg = 0xC2;    // bits opposite C
	
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1=0;
	ICR1=0x00;
	TCCR1A = (1<<COM1A0);
	TCCR1B = (1<<WGM12)|(1<<CS11);
	TIMSK1 = (1<<OCIE1A);
	
	//sei();
	//while (i<iterations)
	//{
		i++;
		DDRD |= (1<<PIND5);
		DDRD |= (1<<PIND1);
		DDRD |= (1<<PIND2);
		PORTD &= ~(1<<PIND1);
		PORTD |= (1<<PIND2);
		for(unsigned int bin = 128; bin >0; bin=bin/2)
		{
			if(msg>=bin)
			{
				TCNT1 = 0;
				OCR1A = 921;
				cnt =0;
				while (cnt < 18) {} //busy wait
				msg -= bin;
			}
			else
			{
				TCNT1 = 0;
				OCR1A = 306;
				cnt =0;
				while (cnt <54);
			}
		}
		msg = 0xC2;
		DDRD &= ~(1<<PIND5);
		DDRD |= (1<<PIND1);
		DDRD |= (1<<PIND2);
		PORTD &= ~(1<<PIND1);
		PORTD &= ~(1<<PIND2);
		_delay_ms(10);
	//}
}



void Control(void)
{
	_delay_ms(250);
	
	/* Calculating Left Wheel speed */
	LWE_dist				= encoder_radius * (45 * 3.14159 / 180) * Left;
	LWE_dist_total			= encoder_radius * (45 * 3.14159 / 180) * left;
	LWE_linear_vel			= LWE_dist;
	
	/* Calculating Right Wheel speed */
	RWE_dist				= encoder_radius * (45 * 3.14159 / 180) * Right;
	RWE_dist_total			= encoder_radius * (45 * 3.14159 / 180) * right;
	RWE_linear_vel			= RWE_dist;
	
	angle				   += (RWE_linear_vel - LWE_linear_vel) / length;
	x_pos				   += 0.5 * (LWE_linear_vel - RWE_linear_vel) * cos(angle);
	y_pos				   += 0.5 * (LWE_linear_vel - RWE_linear_vel) * sin(angle);
	
	dist					= 0.5 * (LWE_dist_total + RWE_dist_total);
	
	error					= LWE_linear_vel - RWE_linear_vel;							
	error2					= left-right;
	
	float percent_left_speed_err = ((LWE_linear_vel - RWE_linear_vel) / LWE_linear_vel) * 100;
		
	printf("\n");
	printf("OCR0A	OCR0B	Left_ticks	Right_ticks	V_Left		V_Right		Speed_Err\n");
	printf_P(PSTR("%d	%d		%d	  %d		%7.3f		%7.3f		%7.3f\n"), 
	OCR0A, OCR0B, left, right, LWE_linear_vel, RWE_linear_vel, percent_left_speed_err);
	printf("\n");
	
	//transmit_c(1000);
	CorrectError(LWE_linear_vel, RWE_linear_vel);
	
	
	/* Phase 1 - Reverse robot and go forward */
	switch (state)
	{
		case PHASE_1: /* Reverse robot and go forward */
			PHASE_1_CONTROL();
			if (turns == 1)
				state = PHASE_2;
			break;
		case PHASE_2:
	
			PHASE_2_CONTROL();
			if (turns == 2)
				state = PHASE_3;
			break;
		case PHASE_3:
			PHASE_3_CONTROL();
			if (turns == 3)
				state = PHASE_4;
			break;
		case PHASE_4:
			PHASE_4_CONTROL();
			if (turns == 4)
			state = PHASE_5;
			break;
		case PHASE_5:
			PHASE_5_CONTROL();
			//if (turns == 2)
			//state = PHASE_3;
			break;
		case PHASE_6:
			PHASE_4_CONTROL();
			//if (turns == 2)
			//state = PHASE_3;
			break;
	}
	
	time++;
	
	//reset Left and Right wheel ticks
	Left=0;
	Right=0;
}







/************************************************************************/
/*								PHASES                                  */
/************************************************************************/

/*
	If robot comes to a stop
		- Reverse wheels and move forward
*/
void PHASE_1_CONTROL(void)
{
	printf("In Phase 1\n");
	HandleIRSensors(1, 1, 1);
		
	/* Wait 250ms to see if Robot has actually come to a stop */
	_delay_ms(1200);
	
	ReverseDirection();
	
	turns = 1;	
	time = 0;
}



/*
	If middle IR sensor detects light OR both left and right IR sensors detect light
		- Make first turn
*/
void PHASE_2_CONTROL(void)
{
	HandleIRSensors(1, 0, 1);
	
	/* If Mid IR, OR, BOTH Left and Right IR detect light (and go low) */
	if (turns == 1 && !MID_IR_HI)
	{
		printf("Mid or Left and Right detected\n");
		/* Back off a little bit */
		ReverseDirection();
		_delay_ms(1100);
		ReverseDirection();
		
		OCR0A = 255;
		OCR0B = 255;
		
		/* If on Left side of arena, turn Right */
		if (START_LEFT_SIDE)
			TurnRight();
		
		/* If on Right side of arena, turn Left */
		if (START_RIGHT_SIDE)
			TurnLeft();
		
		/* Done turning, now go forward */
		ReverseDirection();
		ReverseDirection();
		
		turns = 2;
		time = 0;
		
		/* Enable Ultrasonic Sensor for next Phase */
		InitUltrasonicSensor();
	}
}



/*
	If Robot hits a barrier and comes to stop, OR, Mid IT detects light
		- Reverse direction, move back a little, turn left or right, then start Hugging Reflective tape
			(Use ADC maybe)?
*/
void PHASE_3_CONTROL(void)
{
	printf("In Phase 3, turns = %d\n", turns);
	HandleIRSensors(1, 1, 1);
	
	_delay_ms(500);

	/* If Barrier Detected */
	if (LWE_linear_vel == 0.000 && RWE_linear_vel == 0.000)
	{
		/* Back off a little */
		ReverseDirection();
		_delay_ms(1000);
		ReverseDirection();
		
		/* If Started left side */
		if (START_LEFT_SIDE)
		{
			TurnRight();
		}
		else if (START_RIGHT_SIDE)
		{
			TurnLeft();
		}
		
		turns = 3;
	}
	

	///* If Robot hits a barrier and comes to a stop */
	//if (time == 7 || (LWE_linear_vel == 0.000 && RWE_linear_vel == 0.000))
	//{
	//
		//if (START_LEFT_SIDE)
		//{
			////TurnHalfRight();
		//}
		//else if (START_RIGHT_SIDE)
		//{
			////TurnHalfLeft();
		//}
		//
		//barriers = 1;
		//turns = 3;
		//
		//OCR0A = DEFAULT_LW_FRONT;
		//OCR0B = DEFAULT_RW_FRONT;
		//_delay_ms(500);
		//
		//time = 0;
		//GoForward();
	//}
	///* Else if Robot stops because IR sensors both detect light (in case the 2nd door is open) */
	//else if ((!LEFT_IR_HI && !RIGHT_IR_HI) || (!MID_IR_HI))
	//{
		//GoBackward();
		//_delay_ms(1000);
		//if (START_LEFT_SIDE)
		//{
			//TurnRight();
		//}
		//else if (START_RIGHT_SIDE)
		//{
			//TurnLeft();
		//}
		//turns = 3;
	//}
}


void PHASE_4_CONTROL(void)
{
	if (START_LEFT_SIDE)
	{
		LeftWheelHugTape();
	}
	else if (START_RIGHT_SIDE)
	{
		RightWheelHugTape();
	}
	
	GoForward();
}



void PHASE_5_CONTROL(void)
{
	HandleIRSensors(1, 0, 1);
	
	if (!MID_IR_HI)
	{
		OCR0A = 255;
		OCR0B = 255;
	}
}




void CorrectError(int LWE_linear_vel, int RWE_linear_vel)
{
	if (LWE_linear_vel == RWE_linear_vel)
		return;
		
	if (LWE_linear_vel > RWE_linear_vel) {
		if (OCR0A < 255)
		{
			OCR0A += 3;
		}
	}
	else {
		if (OCR0A > 10)
		{
			OCR0A -= 3;
		}
	}
}








void Delay(unsigned int Delay)
{
	int i;
	for(i=0; i<Delay; i++)
	{
		TCCR1A	= 0;
		TCCR1B	= (1<<CS12)|(1<<CS10);	//Selecting clock source
		OCR1A	= 144;					//Input captured at this value
		TCNT1	= 0;
		TIFR1	= (1<<OCF1A);
		while(!(TIFR1 & (1<<OCF1A)));
	}
	TCCR1B	= 0;
}


