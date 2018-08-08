#pragma once
/*
 * main.h
 *
 * Created: 7/20/2018 1:29:09 AM
 *  Author: Homepc
 */ 


/************************************************************************/
/*                           Wheel Power                                */
/************************************************************************/
#define	DEFAULT_LW_BACK			170			/* Left Wheel */
#define	DEFAULT_RW_BACK			170			/* Right Wheel */

#define	DEFAULT_LW_FRONT		170			/* Left Wheel */
#define	DEFAULT_RW_FRONT		164			/* Right Wheel */


/************************************************************************/
/*                           Wheel Controls                             */
/*************************************************************************
	PB2 -      - PCINT10 -> Left Wheel Control (1|0)
	PB3 - OC0A - PCINT11 -> Left Wheel PWMs
	
	PB5 -	   - PCINT13 -> Right Wheel Control
	PB4 - OC0B - PCINT12 -> Right Wheel PWMs
	
	PB0 -	   - PCINT8  -> Left Wheel Encoder (input)
*************************************************************************/
#define PORTB_CONFIG		(1<<PINB2) | (1<<PINB3) | (1<<PINB5) | (1<<PINB4) | (0<<PINB1) | (0<<PINB6) | (0<<PINB0) | (0<<PINB1)

#define LW_BACKWARD			(1<<PINB2)
#define LW_FORWARD			(0<<PINB2)

#define RW_BACKWARD			(0<<PINB5)
#define RW_FORWARD			(1<<PINB5)
/************************************************************************/
/*                           Side Selector                              */
/*************************************************************************
	PC0 - PCINT16	-> Right Wheel Encoder (input)
	PC6 -			-> HIGH = On Left Side (input)
	PC7 -			-> HIGH = On Right Side (input)
	PC3 -			-> Left IR Sensor (input)
	PC4 -			-> MID IR Sensor (input)
	PC5 -			-> Right IR Sensor (input)
*************************************************************************/
#define PORTC_CONFIG		0x00
//#define PORTC_CONFIG		(0<<PINC6) | (0<<PINC7) | (0<<PINC5) | (0<<PINC4) | (0<<PINC3)| (0<<PINC0)
#define START_LEFT_SIDE		(PINC & (1<<PINC6))	// PC6 High = Starting on LEFT side
#define START_RIGHT_SIDE	(PINC & (1<<PINC7))	// PC7 High = Starting on RIGHT side
#define LEFT_IR_HI			(PINC & (1<<PINC3))	// PC3 LO = Left IR detected Light
#define MID_IR_HI			(PINC & (1<<PINC4))	// PC4 LO = Mid IR detected Light
#define RIGHT_IR_HI			(PINC & (1<<PINC5))	// PC5 LO = Right IR detected Light
/************************************************************************/
/*                           Ultrasonic Sensor                          */
/*************************************************************************
	PD2 - PCINT26 -    INT0 -> Ultrasonic ECHO Pin (input signals)
	PD7 - 0CR2A   - PCINT31 -> Ultrasonic Trigger Pin (output waveforms)
*************************************************************************/
#define PORTD_CONFIG		(1<<PIND7) | (0<<PIND2)



extern FILE mystdout;
extern FILE mystdin;

