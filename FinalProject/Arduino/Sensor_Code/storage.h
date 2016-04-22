// - - - -  INITIALIZATION - - - -

// Initializes all variables for the storage class
void initializeStorage(unsigned int frequencyOfCollection);

// - - - - STORAGE - - - -

// Clears all the data currently in storage
double clearData();

// Takes in the total amount of liquid currently in the container (summed over
// all columns in mL) and adds the datapoint. 
double addData(double liquidAmount);

// - - - - MATH - - - -

// takes in the highest conductor of an arbitrary column and returns
// the volume of liquid in that column (mL)
double volumeFromHighestConductor(unsigned int highestConductor);

//returns the conductivity in Siemens from the voltage
double conductivityFromVoltage(unsigned int voltage);

// - - - - CALCULATIONS ON DATA - - - -

// returns the total amount of output in mL from the 
// start of collection to the most recent data point
double totalOutputFromStartOfCollection();

// returns the total amout of output in mL from just the last hour
double totalOutputFromTheLastHour();

//
double averageOutputFromStartOfCollection();

