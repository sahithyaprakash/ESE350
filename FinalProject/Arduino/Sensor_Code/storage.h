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

// initializes all variables for the storage class
// param frequencyOfCollection: number of samples taken per hour 
void initializeStorage();

// - - - - MANAGE STORAGE - - - -

// clears all the data currently in storage
void clearData();

// takes in the total amount of liquid currently in the container (summed over
// all columns in mL) and adds the datapoint. 
float addData(double liquidAmount);

// called when the more specific data should be compressed into a bundle. For the current
// implementation, this will be called once at every hour.
void archiveData();

// - - - - MATH - - - -

// takes in the highest conductor of an arbitrary column and returns
// the volume of liquid in that column (mL)
float volumeFromHighestConductor(unsigned int highestConductor);

//returns the conductivity in Siemens from the voltage
float conductivityFromVoltage(unsigned int voltage);

// - - - - RETRIEVE DATA - - - -

// returns the total amount of liquid currently in the container in mL. For example, 
// if the patient was admitted at 7:00, the container was emptied at 8:00, then it
// should return the amount of liquid since 8:00.
float currentLiquidLevel();

// returns the total amout of output in mL from just the last hour
float totalOutputFromTheLastHour();

// returns the total amount of liquid added to the container since 
// the last hour. For example, if it's 7:15, then this function should
// return how much liquid has been added since 7:00. 
float totalOutputSinceTheLastHour();
