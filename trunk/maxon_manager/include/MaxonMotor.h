//============================================================================
// Name        : MaxonMotor.h
// Author      : Jon Anderson jonjake
// Version     : 1.0
// Date		   : Oct 2, 2011
// Copyright   : Worcester Polytechnic Institute
// Project	   : Oryx 2.0 (www.wpirover.com)
// Description : Class for a Maxon motor being controlled via an EPOS2
//============================================================================

#ifndef MAXONMOTOR_H_
#define MAXONMOTOR_H_

using namespace std;

#include "Definitions.h"
#include "ConfigFile.h"

class MaxonMotor {
public:
	MaxonMotor();
	virtual ~MaxonMotor();

	bool initializeMotor(void ** keyHandle, unsigned long &errorCode);
	bool SetVelocity(void * keyHandle, long int velocity, unsigned
			long &errorCode);
	void readInMotorParameters(string paramFile);
	string getMotorName();
	void printParameterData();

protected:
	// A string containing the name of the motor
	string motorName;

	// ID # of the the EPOS2 controller, set with DIP switches on device
	unsigned short motorID;

	/************************ MOTOR PARAMETERS ****************************/

	// Select one from the following motor types:
	// DC Motor 						(MT_DC_MOTOR)
	// Brushless w/Sinus Commutation 	(MT_EC_SINUS_COMMUTATED_MOTOR)
	// Brushless w/Block Commutation 	(MT_EC_BLOCK_COMMUTATED_MOTOR)
	unsigned short motorType;

	// Select the motor parameters (all motors):
	// Nominal Current (mA), Maximum Output Current (mA),
	// Thermal Time Constant (s)
	unsigned short motorNominalCurrent;
	unsigned short motorMaxCurrent;
	unsigned short motorThermalTimeConstant;
	// Number of Pole Pairs (EC motors only)
	unsigned char motorNumberPolePairs;

	/************************ SENSOR PARAMETERS ****************************/

	// Select one from the following sensor types:
	// None								(ST_UNKNOWN)
	// Inc. Encoder 3 Channel 			(ST_INC_ENCODER_3CHANNEL)
	// Inc. Encoder 2 Channel 			(ST_INC_ENCODER_2CHANNEL)
	// Hall Effect						(ST_HALL_SENSORS)
	// SSI Abs. Encoder Binary			(ST_SSI_ABS_ENCODER_BINARY)
	// SSI Abs. Encoder Gray			(ST_SSI_ABS_ENCODER_GRAY)
	unsigned short sensorType;

	// Select the sensor parameters:
	// Invert Sensor Polarity (-1=Yes, 1=No)
	int sensorInversion;

	// Inc. encoder resolution
	unsigned long sensorIncEncoderResolution;

	// SSI encoder parameters:
	unsigned short sensorSSIEncoderDataRate;
	unsigned short sensorSSIEncoderNumberMultiTurnBits;
	unsigned short sensorSSIEncoderNumberSingleTurnBits;

	/************************ SAFETY PARAMETERS ****************************/

	// Set these parameters as maximums for safety
	unsigned long maxFollowingError;
	unsigned long maxProfileVelocity;
	unsigned long maxAcceleration;

	/**************************** REGULATION ******************************/

	// These settings should only be set from the file if they are known,
	// otherwise the regulation tuning utility should be used
	bool setRegulationFromFile;

	// Position Regulation Settings
	unsigned short positionP;
	unsigned short positionI;
	unsigned short positionD;

	// Velocity Regulation Settings
	unsigned short velocityP;
	unsigned short velocityI;

	// Current Regulation Settings
	unsigned short currentP;
	unsigned short currentI;

	/********************** PROFILE POSITION MODE *************************/

	// Set the desired velocity for when using the profile position mode
	unsigned long positionProfileVelocity;

	// Set the desired acceleration for when using the profile position mode
	unsigned long positionProfileAcceleration;

	// Set the desired deceleration for when using the profile position mode
	unsigned long positionProfileDeceleration;

	/********************** PROFILE VELOCITY MODE *************************/

	// Set the desired acceleration for when using the profile velocity mode
	unsigned long velocityProfileAcceleration;

	// Set the desired deceleration for when using the profile position mode
	unsigned long velocityProfileDeceleration;
};

#endif /* MAXONMOTOR_H_ */
