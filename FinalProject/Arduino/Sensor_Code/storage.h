// storage.h
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

// - - - -  INITIALIZATION - - - -

// Initializes all variables for the storage class
// param frequencyOfCollection: number of samples taken per hour 
void initializeStorage(unsigned int frequencyOfCollection);

// - - - - STORAGE - - - -

// Clears all the data currently in storage
void clearData();

// Takes in the total amount of liquid currently in the container (summed over
// all columns in mL) and adds the datapoint. 
float addData(double liquidAmount);

// - - - - MATH - - - -

// takes in the highest conductor of an arbitrary column and returns
// the volume of liquid in that column (mL)
float volumeFromHighestConductor(unsigned int highestConductor);

//returns the conductivity in Siemens from the voltage
float conductivityFromVoltage(unsigned int voltage);

// - - - - CALCULATIONS ON DATA - - - -

// returns the total amount of output in mL from the 
// start of collection to the most recent data point
float totalOutputFromStartOfCollection();

// returns the total amout of output in mL from just the last hour
float totalOutputFromTheLastHour();

//
float averageOutputFromStartOfCollection();

