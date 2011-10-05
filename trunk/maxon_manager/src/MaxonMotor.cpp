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
	isInitialized = false;
}

MaxonMotor::~MaxonMotor() {
	// intentionally left empty
}

// Function that registers the KeyHandle and ErrorCode
//  Only open communications if the KeyHandle is still null
bool MaxonMotor::initializeMotor(void ** keyHandle, unsigned long &errorCode){
	// If no one connected yet at the given handle
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
	// Set the communications protocol for the whole stack
	if(VCS_SetProtocolStackSettings(*keyHandle, 1000000, 500, &errorCode))
		cout << "Set Protocol ... Success"  << endl;
	else {
		cout << "Set Protocol ... Failed" << endl;
		return false;
	}
	// Clear the faults
	if(VCS_ClearFault(*keyHandle, motorID, &errorCode))
		cout << "Clear Faults ... Success"  << endl;
	else {
		cout << "Clear Faults ... Failed" << endl;
		return false;
	}
	// Set state enabled
	if(VCS_SetEnableState(*keyHandle, motorID, &errorCode))
		cout << "Set Enabled ... Success" << endl;
	else {
		cout << "Set Enabled ... Failed" << endl;
		return false;
	}

	isInitialized = true;
	cout << "Motor Initialized ... Ready for Command" << endl;
	return true;
}

/*
 * Function to send the motor parameters from this class to the motor controller
 */
bool MaxonMotor::setMotorParameters(void * keyHandle, unsigned long &errorCode){
	// If the motor is not initialized the parameters cannot be set
	if(!isInitialized){
		cout << "Motor Not Initialized"  << endl;
		return false;
	}
	// Set Motor type
	if(VCS_SetMotorType(keyHandle, motorID, motorType, &errorCode) == 0) return false;
	switch(motorType){
		case MT_DC_MOTOR:
			if(VCS_SetDcMotorParameter(keyHandle, motorID, motorNominalCurrent, motorMaxCurrent,
					motorThermalTimeConstant, &errorCode) == 0) return false;
			break;
		case MT_EC_BLOCK_COMMUTATED_MOTOR:
			if(VCS_SetEcMotorParameter(keyHandle, motorID, motorNominalCurrent, motorMaxCurrent,
					motorThermalTimeConstant, motorNumberPolePairs, &errorCode) == 0) return false;
			break;
		case MT_EC_SINUS_COMMUTATED_MOTOR:
			if(VCS_SetEcMotorParameter(keyHandle, motorID, motorNominalCurrent, motorMaxCurrent,
					motorThermalTimeConstant, motorNumberPolePairs, &errorCode) == 0) return false;
			break;
		default:
			return false;
	}

	// Set Sensor type
	if(VCS_SetSensorType(keyHandle, motorID, sensorType, &errorCode) == 0) return false;
	switch(sensorType){
		case ST_INC_ENCODER_3CHANNEL:
			if(VCS_SetIncEncoderParameter(keyHandle, motorID, sensorIncEncoderResolution,
					sensorInversion, &errorCode) == 0) return false;
			break;
		case ST_INC_ENCODER_2CHANNEL:
			if(VCS_SetIncEncoderParameter(keyHandle, motorID, sensorIncEncoderResolution,
					sensorInversion, &errorCode) == 0) return false;
			break;
		case ST_HALL_SENSORS:
			if(VCS_SetHallSensorParameter(keyHandle, motorID, sensorInversion, &errorCode) == 0)
				return false;
			break;
		case ST_SSI_ABS_ENCODER_BINARY:
			if(VCS_SetSsiAbsEncoderParameter(keyHandle, motorID, sensorSSIEncoderDataRate,
					sensorSSIEncoderNumberMultiTurnBits, sensorSSIEncoderNumberSingleTurnBits,
					sensorInversion, &errorCode) == 0) return false;
			break;
		case ST_SSI_ABS_ENCODER_GREY:
			if(VCS_SetSsiAbsEncoderParameter(keyHandle, motorID, sensorSSIEncoderDataRate,
					sensorSSIEncoderNumberMultiTurnBits, sensorSSIEncoderNumberSingleTurnBits,
					sensorInversion, &errorCode) == 0) return false;
			break;
		default:
			return false;
	}

	// Set Safety Parameters
	if(VCS_SetMaxFollowingError(keyHandle, motorID, maxFollowingError, &errorCode) == 0)
		return false;
	if(VCS_SetMaxProfileVelocity(keyHandle, motorID, maxProfileVelocity, &errorCode) == 0)
		return false;
	if(VCS_SetMaxAcceleration(keyHandle, motorID, maxAcceleration, &errorCode) == 0)
		return false;

	// Set profile position and acceleration values
	if(VCS_SetPositionProfile(keyHandle, motorID, positionProfileVelocity, positionProfileAcceleration,
			positionProfileDeceleration, &errorCode) == 0) return false;
	if(VCS_SetVelocityProfile(keyHandle, motorID, velocityProfileAcceleration, velocityProfileDeceleration,
			&errorCode) == 0) return false;

	// Set PID Parameters if selected by user, otherwise get them and store them in the object
	if(setRegulationFromFile){
		if(VCS_SetPositionRegulatorGain(keyHandle, motorID, positionP, positionI, positionD, &errorCode) == 0)
			return false;
		if(VCS_SetVelocityRegulatorGain(keyHandle, motorID, velocityP, velocityI, &errorCode) == 0)
			return false;
		if(VCS_SetCurrentRegulatorGain(keyHandle, motorID, currentP, currentI, &errorCode) == 0)
			return false;
	}
	else{
		if(VCS_GetPositionRegulatorGain(keyHandle, motorID, &positionP, &positionI, &positionD,
				&errorCode) == 0) return false;
		if(VCS_GetVelocityRegulatorGain(keyHandle, motorID, &velocityP, &velocityI, &errorCode) == 0)
			return false;
		if(VCS_GetCurrentRegulatorGain(keyHandle, motorID, &currentP, &currentI, &errorCode) == 0)
			return false;
	}
	cout << "Motor Parameters Sent to EPOS2 ... Success" << endl;
	return true;
}

/*
 * Function to set the motor into profile velocity mode and set the velocity
 */
bool MaxonMotor::setVelocity(void * keyHandle, long int velocity, unsigned
		long &errorCode){
	// If the motor is not initialized the speed cannot be set
	if(!isInitialized){
		cout << "Motor Not Initialized"  << endl;
		return false;
	}
	// Set the operation mode to profile velocity
	if(VCS_SetOperationMode(keyHandle, motorID, OMD_PROFILE_VELOCITY_MODE, &errorCode) == 0)
		return false;
	if(VCS_MoveWithVelocity(keyHandle, motorID, velocity, &errorCode) != 0)
		return true;
	else return false;
}

/*
 * Function to set the motor into profile position mode and set the relative position
 */
bool MaxonMotor::setRelativePosition(void * keyHandle, long int relPosition,
		unsigned long &errorCode){

	long int currentPosition = 0;
	long int desiredPosition = 0;
	// If the motor is not initialized the speed cannot be set
	if(!isInitialized){
		cout << "Motor Not Initialized"  << endl;
		return false;
	}
	if(VCS_GetPositionMust(keyHandle, motorID, &currentPosition, &errorCode) == 0)
		return false;
	desiredPosition = currentPosition + relPosition;
	// Set the operation mode to profile velocity
	if(VCS_SetOperationMode(keyHandle, motorID, OMD_PROFILE_POSITION_MODE, &errorCode) == 0)
		return false;
	// Move to the desired position, absolute, now
	if(VCS_MoveToPosition(keyHandle, motorID, desiredPosition, 1, 1, &errorCode) != 0)
		return true;
	else return false;
}

bool MaxonMotor::isMoving(void * keyHandle, unsigned long &errorCode){
	int isReached = 0;
	VCS_GetMovementState(keyHandle, motorID, &isReached, &errorCode);
	if(isReached){
		return false;
	}
	else return true;
}

/*
 * Function to read in the motor parameters from a parameter file.
 */
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

/*
 * Function that returns the name of the motor
 */
string MaxonMotor::getMotorName(){
	return motorName;
}

/*
 * Function that prints out all the parameters of the motor
 */
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
	cout << "Regulation Set From File:\t"  << setRegulationFromFile << endl;
	cout << "Position P Gain:\t\t" << positionP << endl;
	cout << "Position I Gain:\t\t" << positionI << endl;
	cout << "Position D Gain:\t\t" << positionD << endl;
	cout << "Velocity P Gain:\t\t" << velocityP << endl;
	cout << "Velocity I Gain:\t\t" << velocityI << endl;
	cout << "Current P Gain:\t\t" << currentP << endl;
	cout << "Current I Gain:\t\t" << currentI << endl;
	cout << "############# MODE SPECIFIC PARAMETERS ##############" << endl;
	cout << "Position Profile Velocity:\t" << positionProfileVelocity << endl;
	cout << "Position Profile Acceleration:\t" << positionProfileAcceleration << endl;
	cout << "Position Profile Deceleration:\t" << positionProfileDeceleration << endl;
	cout << "Velocity Profile Acceleration:\t" << velocityProfileAcceleration << endl;
	cout << "Velocity Profile Deceleration:\t" << velocityProfileDeceleration << endl;
	cout << "####################################################" << endl << endl;
}


