//============================================================================
// Name        : MaxonManager.cpp
// Author      : Jon Anderson jonjake
// Version     : 1.0
// Date		   : Oct 2, 2011
// Copyright   : Worcester Polytechnic Institute
// Project	   : Oryx 2.0 (www.wpirover.com)
// Description :
//============================================================================

#include "MaxonManager.h"
#include <new>
#include <memory>

int main (){

	// Create an array to hold each of the motor .ini files
	unsigned int numMotors = getNumberOfMotors();

	string motorIniFiles[numMotors];

	// Generate the array of .ini files
	ifstream file;
	file.open("Motors.list");
	cout << endl;
	unsigned int i;
	for(i = 0; i < numMotors; i++){
		getline(file, motorIniFiles[i]);
		cout << "Loading ... " << motorIniFiles[i] << endl;
	}
	file.close();

	// Create a motor for each .ini file
	// Allocate memory for and create all the new motors
	// This might be risky, because we are not dealocating the memory
	MaxonMotor * motors = new MaxonMotor[numMotors];

	for(i = 0; i < numMotors; i++){
		motors[i].readInMotorParameters(motorIniFiles[i]);
		motors[i].printParameterData();
	}

	void * keyHandle= NULL;
	unsigned long errorCode=0;

	for(i = 0; i < numMotors; i++){
		motors[i].initializeMotor(&keyHandle, errorCode);
	}

	motors[0].SetVelocity(keyHandle, 5000, errorCode);
	::sleep(5);
	motors[0].SetVelocity(keyHandle, 0, errorCode);
	::sleep(5);
	motors[0].SetVelocity(keyHandle, -5000, errorCode);
	::sleep(5);


	// Initialize all motors


	// Topics an info publisher and command subscriber for each motor
	// Info topic will contain Control Mode, IsMoving,
	// Value (pos or vel), Current
	// Command topic will take Control Mode, Value (pos or vel)

	// Want to use Profile Position and Profile Velocity Modes
	return 0;
}

unsigned int getNumberOfMotors(){
	ifstream file;
	string tempString;
	unsigned int numMotors = 0;

	// Open the file
	file.open("Motors.list");
	// Determine # of lines in file
	while(getline(file, tempString)){
		numMotors++;
	}
	file.close();
	return numMotors;
}

// Function that takes in a Motors.list file and generates an array of
// strings containing the files for the individual .ini files
