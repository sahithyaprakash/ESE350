// storage.c
//
// ESE 350 Final Project
// by Phillip Trent & Sahithya Prakash
// University of Pennsylvania
//
// [description of the file]
//
// author: ptrent
// email: ptrent@seas.upenn.edu
// blog: philliphtrentiii.info/embedded-systems-blog

#include "storage.h"
#include <stdlib.h>
#include <Math.h>
#include <String.h>
#define ARBITRARY_SIZE 1000


// - - - -  INITIALIZATION - - - -

int index;
int totalNumOfSamples;
int time_counter;
float totalOutputFromBeginning;
float outputLastHour;
float* volumeSamples;

// Initializes all variables for the storage class
// param frequencyOfCollection: number of samples taken per hour 
void initializeStorage(unsigned int frequencyOfCollection) {

	volumeSamples = malloc(frequencyOfCollection * sizeof(float)); 
	index = 0; 
	totalNumOfSamples = frequencyOfCollection; 
	time_counter = 0; 
	totalOutputFromBeginning = 0.0; 
	outputLastHour = 0.0; 


}

// - - - - STORAGE - - - -

// Clears all the data currently in storage
void clearData() {

	totalOutputFromBeginning += volumeSamples[totalNumOfSamples - 1]; 
	memset(volumeSamples, 0, sizeof(volumeSamples));
	index = 0; 

}

// Takes in the total amount of liquid currently in the container (summed over
// all columns in mL) and adds the datapoint. 
float addData(double liquidAmount) {

	volumeSamples[index] = liquidAmount;

	if (index < totalNumOfSamples) {
		index ++;
	} else {
		index = 0; 
	}

	time_counter ++; 
}

// - - - - MATH - - - -

// takes in the highest conductor of an arbitrary column and returns
// the volume of liquid in that column (mL)
float volumeFromHighestConductor(unsigned int highestConductor) {

	if (highestConductor > 0) { 
		float actuationDepth = 2.5;
		float unitVolume = 3.175;
		return (actuationDepth + ((highestConductor - 1) * unitVolume));
	}
}

//returns the conductivity in Siemens from the voltage
float conductivityFromVoltage(unsigned int voltage) {
}

// - - - - CALCULATIONS ON DATA - - - -

// returns the total amount of output in mL from the 
// start of collection to the most recent data point
float totalOutputFromStartOfCollection() {

	float totalOutput = volumeSamples[totalNumOfSamples - 1] - volumeSamples[0]; 
	outputLastHour = totalOutput; 
	return totalOutput; 
}

// returns the total amout of output in mL from just the last hour
float totalOutputFromTheLastHour() {

	return outputLastHour; 
}

//Total output / total time 
float averageOutputFromStartOfCollection() {

	float total_time = time_counter / totalNumOfSamples; 
	return (totalOutputFromBeginning / total_time);
}

