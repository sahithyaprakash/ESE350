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
float volumeSamples[NUMBER_OF_DATA_POINTS_PER_ARCHIVE + 10];

// Initializes all variables for the storage class
// param frequencyOfCollection: number of samples taken per hour
void initializeStorage(unsigned int frequencyOfCollection) {

	//volumeSamples = malloc(frequencyOfCollection * sizeof(float));
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

	volumeSamples[index] = (float) liquidAmount;
	index = (index >= totalNumOfSamples - 1 ? 0 : index + 1);
	time_counter ++;
}

// called when the more specific data should be compressed into a bundle. For the current
// implementation, this will be called once at every hour.
void archiveData() {
	
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
	return 0;
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
	float totalOutput = 0;
	if (volumeSamples[totalNumOfSamples - 1] == 0) {
		//the array is not full, so we subtract the start from the finish
		totalOutput = volumeSamples[index] - volumeSamples[0];
	} else {
		//the array is full, so we can subtract the current index from the next one
		int nextIndex = (index + 1 > totalNumOfSamples - 1 ? 0 : index + 1);
		totalOutput = volumeSamples[index] - volumeSamples[nextIndex];
	}
	outputLastHour = totalOutput;

	return volumeSamples[index];
}

// returns the total amount of liquid added to the container since
// the last hour. For example, if it's 7:15, then this function should
// return how much liquid has been added since 7:00.
float totalOutputSinceTheLastHour() {
	return 0.0;
}


