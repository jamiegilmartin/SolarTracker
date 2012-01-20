//
//  SolarTracker.ino
//
//  Created by jamie gilmartin on 2012-01-19.
//  Copyright (c) 2012 jamiegilmartin. All rights reserved.
//


/*
* The idea for this project came from http://hilaroad.com/
* & their video http://www.youtube.com/embed/4abMkyPoGcs.
* 
* Here's my write up: http://jamiegilmartin.com/ee/solarTracker.shtml
* 
* 5V
* pin D9 = horizontal servo
* pin D10 = vertical servo
* pin A0 = horizontal photoresistor with 10k resistor to pin (*white tape)
* pin A1 = vertical photoresistor with 10k resistor to pin (*blue tape)
*
* Each photoresistor is two photocells twisted together. The V+ goes to one
* photocell, the ground to the other and the legs that are twisted together go to
* the analog in.
*
*
*/

#include <Servo.h> 

//create servo objects to control servos 
Servo horizontalServo;
Servo verticalServo; 

int photoresistorHorizontal = 0;
int photoresistorVertical = 1;

int horizontalDegree;
int verticalDegree;


//smoothing - http://arduino.cc/en/Tutorial/Smoothing
const int numReadings = 10;

int Hreadings[numReadings];      // the readings from the analog input
int Hindex = 0;                  // the index of the current reading
int Htotal = 0;                  // the running total
int Haverage = 0;                // the average

int Vreadings[numReadings];
int Vindex = 0;
int Vtotal = 0;
int Vaverage = 0;

void setup(){ 
	Serial.begin(9600);
	horizontalServo.attach(9);
	verticalServo.attach(10);
	pinMode(photoresistorHorizontal, INPUT);
	pinMode(photoresistorVertical, INPUT);
	
	// initialize all the readings to 0: 
	for (int thisHReading = 0; thisHReading < numReadings; thisHReading++){
		Hreadings[thisHReading] = 0;
	}
	for (int thisVReading = 0; thisVReading < numReadings; thisVReading++){
		Vreadings[thisVReading] = 0;
	}
}
 
void loop(){
	horizontalDegree = map(analogRead(photoresistorHorizontal),0,1023,0,179);
	horizontalDegree = 179 - horizontalDegree; //have to make opposite since soldering messup
	verticalDegree = map(analogRead(photoresistorVertical),0,1023,0,90); // 90 degrees max for vertical servo
	
	
	//average horizontal
	Htotal= Htotal - Hreadings[Hindex];
	Hreadings[Hindex] = horizontalDegree;
	Htotal= Htotal + Hreadings[Hindex];
	Hindex = Hindex + 1;
	if (Hindex >= numReadings){
		Hindex = 0;
	}              
	
	//average verticle
	Vtotal= Vtotal - Vreadings[Vindex];
	Vreadings[Vindex] = verticalDegree;
	Vtotal= Vtotal + Vreadings[Vindex];
	Vindex = Vindex + 1;                    
	
	if (Vindex >= numReadings){
		Vindex = 0;
	}                        

	// calculate the average:
	Haverage = Htotal / numReadings;
	Vaverage = Vtotal / numReadings;
	
	horizontalServo.write(Haverage);
	verticalServo.write(Vaverage);
	
	delay(15);
}

