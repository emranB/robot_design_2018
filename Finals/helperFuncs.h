/*
 * helperFuncs.h
 *
 * Created: 7/24/2018 6:16:47 PM
 *  Author: Homepc
 */ 

void ReverseDirection(void);
void GoForward(void);
void GoBackward(void);
void TurnRight(void);
void TurnLeft(void);
void InitUltrasonicSensor(void);
void LeftWheelHugTape(void);
void RightWheelHugTape(void);

enum DIRECTION {
	BACKWARD=0, FORWARD=1
};