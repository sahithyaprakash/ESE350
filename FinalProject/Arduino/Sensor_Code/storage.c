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
#define NUMBER_OF_DATA_POINTS_PER_ARCHIVE_LOW 10 //60
#define NUMBER_OF_DATA_POINTS_PER_ARCHIVE_MED 13 //80
#define NUMBER_OF_DATA_POINTS_PER_ARCHIVE_HIGH 20 //120

// - - - -  INITIALIZATION - - - -

int currentVolumePointer = NULL;
int lastArchivedOutput = NULL;
SpecificityOfData specificity = LOW;
float volumeSamplesLow[NUMBER_OF_DATA_POINTS_PER_ARCHIVE_LOW];
float volumeSamplesMed[NUMBER_OF_DATA_POINTS_PER_ARCHIVE_MED];
float volumeSamplesHigh[NUMBER_OF_DATA_POINTS_PER_ARCHIVE_HIGH];
float archivedOutput[NUMBER_OF_ARCHIVES];


// Initializes all variables for the storage class
// param frequencyOfCollection: number of samples taken per hour
void initializeStorage(SpecificityOfData s) {

	currentVolumePointer = 0;
	specificity = s;
	lastArchivedOutput = 0;
	memset(volumeSamplesLow, 0, sizeof(volumeSamplesLow));
	memset(volumeSamplesMed, 0, sizeof(volumeSamplesMed));
	memset(volumeSamplesHigh, 0, sizeof(volumeSamplesHigh));
	memset(archivedOutput, 0, sizeof(archivedOutput));
}

int numberOfSamplesForSpecificityLevel(SpecificityOfData s) {
	switch (specificity) {
	case LOW:
		return NUMBER_OF_DATA_POINTS_PER_ARCHIVE_LOW;
	case MEDIUM:
		return NUMBER_OF_DATA_POINTS_PER_ARCHIVE_MED;
	case HIGH:
		return NUMBER_OF_DATA_POINTS_PER_ARCHIVE_HIGH;
	}
}
// - - - - STORAGE - - - -

// Clears all the data currently in storage
void clearData() {

	memset(volumeSamplesLow, 0, sizeof(volumeSamplesLow));
	memset(volumeSamplesMed, 0, sizeof(volumeSamplesMed));
	memset(volumeSamplesHigh, 0, sizeof(volumeSamplesHigh));
	memset(archivedOutput, 0, sizeof(archivedOutput));
	currentVolumePointer = 0;
	lastArchivedOutput = 0;

}

void setVolumeSamples(float value) {
	switch (specificity) {
	case LOW:
		volumeSamplesLow[currentVolumePointer] = value;
		break;
	case MEDIUM:
		volumeSamplesMed[currentVolumePointer] = value;
		break;
	case HIGH:
		volumeSamplesHigh[currentVolumePointer] = value;
		break;
	}
	currentVolumePointer++;
}

float getVolumeSample(int index) {
	switch (specificity) {
	case LOW:
		return volumeSamplesLow[index];
	case MEDIUM:
		return volumeSamplesMed[index];
	case HIGH:
		return volumeSamplesHigh[index];
	}
}

// Takes in the total amount of liquid currently in the container (summed over
// all columns in mL) and adds the datapoint.
float addData(float liquidAmount) {
	setVolumeSamples(liquidAmount);
}

// called when the more specific data should be compressed into a bundle. For the current
// implementation, this will be called once at every hour.
void archiveData(SpecificityOfData newSpecificity) {
	//Store and then increment the pointer (index of the next slot ot be filled that's not currently filled)
	archivedOutput[lastArchivedOutput] = totalOutputSinceTheLastHour();
	//update the archive pointer
	lastArchivedOutput ++;
	lastArchivedOutput = (lastArchivedOutput >= NUMBER_OF_ARCHIVES ? 0 : lastArchivedOutput);
	currentVolumePointer = 0;
	specificity = newSpecificity;
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

	return getVolumeSample(currentVolumePointer == 0 ? 0 : currentVolumePointer - 1);
}

// returns the total amout of output in mL from just the last hour
float totalOutputFromTheLastHour() {

	return archivedOutput[(lastArchivedOutput == 0 ? 0 : lastArchivedOutput - 1)];

}

// returns the total amount of liquid added to the container since
// the last hour. For example, if it's 7:15, then this function should
// return how much liquid has been added since 7:00.
float totalOutputSinceTheLastHour() {

	return getVolumeSample((currentVolumePointer == 0 ? 0 : currentVolumePointer - 1)) - getVolumeSample(0);
}


