//============================================================================
// Name        : MaxonMotor.cpp
// Author      : Jon Anderson jonjake
// Version     : 1.0
// Date		   : Oct 2, 2011
// Copyright   : Worcester Polytechnic Institute
// Project	   : Oryx 2.0 (www.wpirover.com)
// Description : Class for a Maxon motor being controlled via an EPOS2
//============================================================================

#include "MaxonMotor.h"

MaxonMotor::MaxonMotor(){
	// intentionally left empty
}

MaxonMotor::~MaxonMotor() {
	// intentionally left empty
}

// Function that registers the KeyHandle and ErrorCode
//  Only open communications if the KeyHandle is still null
bool MaxonMotor::initializeMotor(void ** keyHandle, unsigned long &errorCode){
	// If no one connected yet
	if(*keyHandle == 0){
		// Try to connect
		*keyHandle = VCS_OpenDevice("EPOS2", "MAXON SERIAL V2", "USB", "USB0", &errorCode);
		// Connection failed;
		if(*keyHandle == 0){
			cout << "Connection ... Failed" << endl;
			return false;
		}
		else cout << "Connection ... Success" << endl;
	}
	// Clear faults
	if(VCS_SetProtocolStackSettings(*keyHandle, 1000000, 500, &errorCode))
		cout << "Protocol ... Set"  << endl;
	if(VCS_ClearFault(*keyHandle, motorID, &errorCode))
		cout << "Faults ... Cleared"  << endl;
	// Send down the parameters
	// Setup velocity and position profile modes
	// Set state enabled
	if(VCS_SetEnableState(*keyHandle, motorID, &errorCode))
		cout << "Enabled ... TRUE" << endl;
	/*
	VCS_SetOperationMode(keyHandle, motorID, OMD_PROFILE_VELOCITY_MODE, &errorCode);
	VCS_SetVelocityProfile(keyHandle, motorID, velocityProfileAcceleration,
		velocityProfileDeceleration, &errorCode);
	if(VCS_MoveWithVelocity(keyHandle, motorID, 3000, &errorCode))
		cout << "Moving ..." << endl;
	return true;
	*/
}

// Function to set motor a specific speed
bool MaxonMotor::SetVelocity(void * keyHandle, long int velocity, unsigned
		long &errorCode){
	VCS_SetOperationMode(keyHandle, motorID, OMD_PROFILE_VELOCITY_MODE, &errorCode);
	VCS_SetVelocityProfile(keyHandle, motorID, velocityProfileAcceleration,
			velocityProfileDeceleration, &errorCode);
	if(VCS_MoveWithVelocity(keyHandle, motorID, velocity, &errorCode) != 0)
		return true;
	else return false;
}

// Function to move motor to a relative position

// Reads in the motor parameters from the .ini file passed in.
void MaxonMotor::readInMotorParameters(string paramFile) {

	ConfigFile config(paramFile);

	//Read in all the parameters from the file
	config.readInto (motorName, "MotorName");
	config.readInto(motorID, "MotorID");
	config.readInto(motorType, "MotorType");
	config.readInto(motorNominalCurrent, "MotorNominalCurrent");
	config.readInto(motorMaxCurrent, "MotorMaxCurrent");
	config.readInto(motorThermalTimeConstant, "MotorThermalTimeConstant");
	config.readInto(motorNumberPolePairs, "MotorNumberPolePairs");
	config.readInto(sensorType, "SensorType");
	config.readInto(sensorInversion, "SensorInversion");
	config.readInto(sensorIncEncoderResolution, "IncEncoderResolution");
	config.readInto(sensorSSIEncoderDataRate, "SsiDataRate");
	config.readInto(sensorSSIEncoderNumberMultiTurnBits, "SsiMultiTurnBits");
	config.readInto(sensorSSIEncoderNumberSingleTurnBits, "SsiSingleTurnBits");
	config.readInto(maxFollowingError, "MaxFollowingError");
	config.readInto(maxProfileVelocity, "MaxProfileVelocity");
	config.readInto(maxAcceleration, "MaxAcceleration");
	config.readInto(setRegulationFromFile, "SetRegulation");
	config.readInto(positionP, "PositionPGain");
	config.readInto(positionI, "PositionIGain");
	config.readInto(positionD, "PositionDGain");
	config.readInto(velocityP, "VelocityPGain");
	config.readInto(velocityI, "VelocityIGain");
	config.readInto(currentP, "CurrentPGain");
	config.readInto(currentI, "CurrentIGain");
	config.readInto(positionProfileVelocity, "PositionProfileVel");
	config.readInto(positionProfileAcceleration, "PositionProfileAcc");
	config.readInto(positionProfileDeceleration, "PositionProfileDec");
	config.readInto(velocityProfileAcceleration, "VelocityProfileAcc");
	config.readInto(velocityProfileDeceleration, "VelocityProfileDec");

}

string MaxonMotor::getMotorName(){
	return motorName;
}

// Prints out all the data stored in the MaxonMotor object
void MaxonMotor::printParameterData(){
	cout << endl << "############# MAXON EPOS2 PARAMETERS ##############" << endl;
	cout << "Motor Name: \t\t\t" << motorName << endl;
	cout << "Node ID:\t\t\t" << motorID << endl;
	cout << "############# MOTOR PARAMETERS #############" << endl;
	cout << "Motor Type:\t\t\t";
	switch(motorType){
		case MT_DC_MOTOR:
			cout << "DC Motor" << endl;
			break;
		case MT_EC_BLOCK_COMMUTATED_MOTOR:
			cout << "EC Motor w/ Block Commutation" << endl;
			break;
		case MT_EC_SINUS_COMMUTATED_MOTOR:
			cout << "EC Motor w/ Sinus Commutation" << endl;
			break;
		default:
			cout << "Unknown Type" << endl;
	}
	cout << "Nominal Current:\t\t"  << motorNominalCurrent << " mA" << endl;
	cout << "Max Current:\t\t\t" << motorMaxCurrent << " mA"  << endl;
	cout << "Thermal Time Const:\t\t" << motorThermalTimeConstant << " sec" << endl;
	switch(motorType){
		case MT_DC_MOTOR:
			break;
		default:
			cout << "# of Pole Pairs:\t\t" << motorNumberPolePairs << endl;
	}
	cout << "############# SENSOR PARAMETERS #############" << endl;
	cout << "Sensor Type:\t\t\t";
	switch(sensorType){
		case ST_INC_ENCODER_3CHANNEL:
			cout << "Incremental Encoder, 3 CH" << endl;
			cout << "Encoder Resolution:\t\t" << sensorIncEncoderResolution <<
					" CPR" << endl;
			break;
		case ST_INC_ENCODER_2CHANNEL:
			cout << "Incremental Encoder, 2 CH" << endl;
			cout << "Encoder Resolution:\t\t" << sensorIncEncoderResolution <<
					" CPR" << endl;
			break;
		case ST_HALL_SENSORS:
			cout << "Hall Effect" << endl;
			break;
		case ST_SSI_ABS_ENCODER_BINARY:
			cout << "Absolute SSI Encoder, Binary" << endl;
			cout << "SSI Data Rate:\t\t\t" << sensorSSIEncoderDataRate <<
					" bits/sec" << endl;
			cout << "SSI Multi-Turn Bits:\t\t" << sensorSSIEncoderNumberMultiTurnBits
					<< " bits\t" << endl;
			cout << "SSI Single-Turn Bits:\t\t" << sensorSSIEncoderNumberSingleTurnBits
					<< " bits" << endl;
			break;
		case ST_SSI_ABS_ENCODER_GREY:
			cout << "Absolute SSI Encoder, Grey" << endl;
			cout << "SSI Data Rate:\t\t" << sensorSSIEncoderDataRate <<
					" bits/sec" << endl;
			cout << "SSI Multi-Turn Bits:\t\t" << sensorSSIEncoderNumberMultiTurnBits
					<< " bits\t";
			cout << "SSI Single-Turn Bits:\t\t" << sensorSSIEncoderNumberSingleTurnBits
					<< " bits" << endl;
			break;
		default:
			cout << "Unknown Type" << endl;
	}
	cout << "Sensor Inverted:\t\t";
	if (sensorInversion > 0)
		cout << "false" << endl;
	else
		cout << "true" << endl;
	cout << "############# SAFETY PARAMETERS #############" << endl;
	cout << "Max Following Error:\t\t" << maxFollowingError << endl;
	cout << "Max Profile Velocity:\t\t" << maxProfileVelocity << endl;
	cout << "Max Acceleration:\t\t" << maxAcceleration << endl;
	cout << "############# REGULATION PARAMETERS ##############" << endl;
	// Do the if thing
	cout << "Position P Gain: \t\t" << positionP << endl;
	cout << "Position I Gain: \t\t" << positionI << endl;
	cout << "Position D Gain: \t\t" << positionD << endl;
	cout << "Velocity P Gain: \t\t" << velocityP << endl;
	cout << "Velocity I Gain: \t\t" << velocityI << endl;
	cout << "Current P Gain: \t\t" << currentP << endl;
	cout << "Current I Gain: \t\t" << currentI << endl;
	cout << "############# MODE SPECIFIC PARAMETERS ##############" << endl;
	cout << "Position Profile Velocity: \t" << positionProfileVelocity << endl;
	cout << "Position Profile Acceleration: \t" << positionProfileAcceleration << endl;
	cout << "Position Profile Deceleration: \t" << positionProfileDeceleration << endl;
	cout << "Velocity Profile Acceleration: \t" << velocityProfileAcceleration << endl;
	cout << "Velocity Profile Deceleration: \t" << velocityProfileDeceleration << endl;
	cout << "####################################################" << endl << endl;
}

// Set Velocity (Vel Profile Mode) - set mode, move with velocity
// MoveRelativePosition (Pos Profile Mode) - set mode, move to position


