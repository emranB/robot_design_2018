#pragma once
/*
 * control.h
 *
 * Created: 7/24/2018 1:17:01 AM
 *  Author: Homepc
 */ 





volatile int		right	= 0;		// count right wheel ticks
volatile int		left	= 0;		// count left wheel ticks
volatile int		Left	= 0;		// count left wheel ticks 2
volatile int		Right	= 0;		// count right wheel ticks 2

volatile float		angle	= 0;
volatile float		x_pos	= 0;
volatile float		y_pos	= 0;

		 float		LWE_dist;
		 float		LWE_dist_total;
		 float		LWE_linear_vel;

		 float		RWE_dist;
		 float		RWE_dist_total;
		 float		RWE_linear_vel;

		 int		turns	 = 0;
		 int		barriers = 0;
		 int		time	 = 0;
		 float		error	 = 0;					/* Velocity error */
		 int		error2	 = 0;
		 float		dist	 = 0;					/* Initial distance traveled */
		 float		length	 = 0.14;					/* The distance between wheels in meters */
		 float		encoder_radius = (0.0065 / 2);	/* Diameter = 6.5cm = 0.0065m */
volatile int		cnt = 0;						/* Communication Systems */

		 
		 
void PHASE_1_CONTROL(void);
void PHASE_2_CONTROL(void);
void PHASE_3_CONTROL(void);
void PHASE_4_CONTROL(void);
void PHASE_5_CONTROL(void);

