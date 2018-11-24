/*
Extends the Serial class to encode SLIP over serial
*/

#ifndef Stepper_28BYJ_48_h
#define Stepper_28BYJ_48_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class Stepper_28BYJ_48 {
	

public:
	

	Stepper_28BYJ_48(int pin_1n1, int pin_1n2, int pin_1n3, int pin_1n4);
    void step(int);
   

private:
	void setOutput(int out);
	int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
    int motorSpeed = 1200;  //variable to set stepper speed
	int countsperrev = 512; // number of steps per full revolution
    int pin_1n1; int pin_1n2; int pin_1n3; int pin_1n4;

};


#endif