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
#include <stdio.h>

#define NUMBER_OF_ARCHIVES 6

// - - - -  INITIALIZATION - - - -

int currentVolumePointer = NULL;
int lastArchivedOutput = NULL;
float volumeSamples[NUMBER_OF_DATA_POINTS_PER_ARCHIVE];
float archivedOutput[NUMBER_OF_ARCHIVES];


// Initializes all variables for the storage class
// param frequencyOfCollection: number of samples taken per hour
void initializeStorage() {

	currentVolumePointer = 0;
	lastArchivedOutput = 0;
	memset(volumeSamples, 0, sizeof(volumeSamples));
	memset(archivedOutput, 0, sizeof(archivedOutput)); 
}

// - - - - STORAGE - - - -

// Clears all the data currently in storage
void clearData() {

	memset(volumeSamples, 0, sizeof(volumeSamples));
	memset(archivedOutput, 0, sizeof(archivedOutput));
	currentVolumePointer = 0;
	lastArchivedOutput = 0; 

}

// Takes in the total amount of liquid currently in the container (summed over
// all columns in mL) and adds the datapoint.
float addData(float liquidAmount) {

	//Store and then increment the pointer (index of the next slot to be filled that's not currently filled)
	volumeSamples[currentVolumePointer] = liquidAmount;
	currentVolumePointer++;

}

// called when the more specific data should be compressed into a bundle. For the current
// implementation, this will be called once at every hour.
void archiveData() {
	printf("Archiving Data: \n");
	//Store and then increment the pointer (index of the next slot ot be filled that's not currently filled)
	archivedOutput[lastArchivedOutput] = volumeSamples[(currentVolumePointer == 0 ? 0 : currentVolumePointer - 1)] - volumeSamples[0];
	printf("Data Saved: (%i, %d)\n", lastArchivedOutput, archivedOutput[lastArchivedOutput]);
	lastArchivedOutput ++;
	lastArchivedOutput = (lastArchivedOutput >= NUMBER_OF_ARCHIVES ? 0 : lastArchivedOutput);
	printf("New Archived Pointer: %i\n\n", lastArchivedOutput);
	currentVolumePointer = 0;

}

// - - - - MATH - - - -

// takes in the highest conductor of an arbitrary column and returns
// the volume of liquid in that column (mL)
float volumeFromHighestConductor(unsigned int highestConductor) {

	double calculation = 0.0;
	float actuationDepth = 0.0;
	float unitVolume = 0.0;

	if (highestConductor > 0) {
		//2.5
		actuationDepth = 1.654;
		//3.175
		unitVolume = 2.101;
		calculation = actuationDepth + ((highestConductor - 1) * unitVolume);
		return calculation;
	} else {
		return 0.0; 
	}
}

//returns the conductivity in Siemens from the voltage
float conductivityFromVoltage(unsigned int voltage) {
	return 0;
}

// - - - - CALCULATIONS ON DATA - - - -

// returns the total amount of liquid currently in the container in mL. For example, 
// if the patient was admitted at 7:00, the container was emptied at 8:00, then it
// should return the amount of liquid since 8:00.
float currentLiquidLevel() {

	return volumeSamples[(currentVolumePointer == 0 ? 0 : currentVolumePointer - 1)]; 
}

// returns the total amout of output in mL from just the last hour
float totalOutputFromTheLastHour() {
	
	return archivedOutput[(lastArchivedOutput == 0 ? 0 : lastArchivedOutput - 1)];

}

// returns the total amount of liquid added to the container since
// the last hour. For example, if it's 7:15, then this function should
// return how much liquid has been added since 7:00.
float totalOutputSinceTheLastHour() {
	
	return volumeSamples[(currentVolumePointer == 0 ? 0 : currentVolumePointer - 1)] - volumeSamples[0]; 
}


