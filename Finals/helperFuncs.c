/*
 * helperFunc.c
 *
 * Created: 7/24/2018 6:16:32 PM
 *  Author: Homepc
 */ 
#include "LIBS.h"
#include "main.h"
#include "helperFuncs.h"

extern enum DIRECTION direction;
enum STATES {
	PHASE_1, /* Reverse robot and go forward */
	PHASE_2, /* Make first turn and go forward */
	PHASE_3, /* Hit barrier OR detect middle IR Sensor, then make second turn and go forward */
	PHASE_4, /* Start Hugging reflective tape */
	PHASE_5,
	PHASE_6,
};

extern enum STATES state;
extern int turns;
extern int time;

/* Reverse direction of movement */
void ReverseDirection(void)
{
	printf("Reversing\n");
	if (direction == BACKWARD)
	{
		GoForward();
		direction = FORWARD;
	}
	else if (direction == FORWARD)
	{
		GoBackward();
		direction = BACKWARD;
	}
}


/* Go Forward */
void GoForward(void)
{
	PORTB	  = (LW_FORWARD) | (RW_FORWARD);
	OCR0A	  = DEFAULT_LW_FRONT;
	
	OCR0B	  = DEFAULT_RW_FRONT - 50;
	_delay_ms(250);
	OCR0B	  = DEFAULT_RW_FRONT;
	
	direction = FORWARD;
}


/* Go Backward */
void GoBackward(void)
{
	PORTB     = (LW_BACKWARD) | (LW_BACKWARD);
	OCR0A	  = DEFAULT_LW_BACK;
	OCR0B	  = DEFAULT_RW_BACK;
	direction = BACKWARD;
}


/* Turn Right */
void TurnRight(void)
{
	OCR0A = 120;
	OCR0B = 255;
	_delay_ms(650);
	OCR0A = 255;
	OCR0B = 255;
}


/* Turn Left */
void TurnLeft(void)
{
	OCR0A = 255;
	OCR0B = 120;
	_delay_ms(650);
	OCR0A = 255;
	OCR0B = 255;
}


/* Turn Half-Right */
void TurnHalfRight(void)
{
	OCR0A = 120;
	OCR0B = 255;
	_delay_ms(425);
	OCR0A = 255;
	OCR0B = 255;
}


/* Turn Left */
void TurnHalfLeft(void)
{
	OCR0A = 255;
	OCR0B = 120;
	_delay_ms(425);
	OCR0A = 255;
	OCR0B = 255;
}


/* Handle IR Sensors */
void HandleIRSensors(int left, int middle, int right)
{
	int temp = 0;
	/* Left IR */
	if (left && !(PINC & (1<<PINC3)))
	{
		temp = OCR0A;
		OCR0A = 50;
		_delay_ms(100);
		OCR0A = temp;
	}
	/* Middle IR */
	if (middle && !(PINC & (1<<PINC4)))
	{
		OCR0A = 255;
		OCR0B = 255;
		_delay_ms(750);
		GoBackward();
		_delay_ms(750);
		OCR0A = 255;
		OCR0B = 255;
		GoForward();
	}
	/* Right IR */
	if (right && !(PINC & (1<<PINC5)))
	{
		temp = OCR0B;
		OCR0B = 70;
		_delay_ms(100);
		OCR0B = temp;
	}
	
}


/* Handle Ultrasonic Sensors */
void InitUltrasonicSensor(void)
{
	
}


/* Robot Hugs its Left side's tape */
int OCR0B_change = 0;
int noDetectCnt = 0;
int temp = 0;
void LeftWheelHugTape(void)
{
	printf("Hugging left %d - %d\n", OCR0A, OCR0B);
	
	if (!LEFT_IR_HI)
	{
		/* Pause Before Shifting Right */
		OCR0A = 255;
		OCR0B = 255;
		_delay_ms(80);
		
		/* Shift right a little, then move forward */
		OCR0A = 50;
		_delay_ms(60);
		OCR0A = DEFAULT_LW_FRONT;
		OCR0B = DEFAULT_RW_FRONT;
	}
	
	if (time < 20)
	{
		OCR0A = 50;
		_delay_ms(30);
		OCR0A = DEFAULT_RW_FRONT
	
		if (!MID_IR_HI)
		{
			ReverseDirection();
			_delay_ms(1000);                                                                              
			ReverseDirection();
			OCR0A -= 10;
			_delay_ms(200);
			OCR0A = DEFAULT_LW_FRONT;
		}
	}
	else
	{
		OCR0B = DEFAULT_RW_FRONT - 20;
	
		if (!MID_IR_HI)
		{
			ReverseDirection();
			_delay_ms(1000);
			ReverseDirection();
			OCR0A += 10;
			OCR0B -= 30;
			_delay_ms(200);
			OCR0A = DEFAULT_LW_FRONT;
		}
	}

}


/* Robot Hugs its Right side's tape */
void RightWheelHugTape(void)
{
	printf("Hugging left %d - %d\n", OCR0A, OCR0B);
	
	if (!RIGHT_IR_HI)
	{
		/* Pause Before Shifting Right */
		OCR0A = 255;
		OCR0B = 255;
		_delay_ms(80);
		
		/* Shift right a little, then move forward */
		OCR0B = 50;
		_delay_ms(60);
		OCR0A = DEFAULT_LW_FRONT;
		OCR0B = DEFAULT_RW_FRONT;
	}
	
	if (time < 20)
	{
		OCR0A = 50;
		_delay_ms(30);
		OCR0A = DEFAULT_RW_FRONT
		
		if (!MID_IR_HI)
		{
			ReverseDirection();
			_delay_ms(1000);
			ReverseDirection();
			OCR0B -= 10;
			_delay_ms(200);
			OCR0B = DEFAULT_RW_FRONT;
		}
	}
	else
	{
		OCR0A = DEFAULT_LW_FRONT - 20;
		
		if (!MID_IR_HI)
		{
			ReverseDirection();
			_delay_ms(1000);
			ReverseDirection();
			OCR0B += 10;
			OCR0A -= 30;
			_delay_ms(200);
			OCR0B = DEFAULT_RW_FRONT;
		}
	}
}