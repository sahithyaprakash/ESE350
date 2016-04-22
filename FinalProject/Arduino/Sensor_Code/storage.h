// - - - - STORAGE - - - -

//
double clearData();

// - - - - MATH - - - -

// takes in the highest conductor of an arbitrary column and returns
// the volume of liquid in that column (mL)
double volumeFromHighestConductor(unsigned int highestConductor);

//returns the conductivity in Siemens from the voltage
double conductivityFromVoltage(unsigned int voltage);

double

// - - - - CALCULATIONS ON DATA - - - -

//
double totalOutputFromStartOfCollection();

//
double totalOutputFromTheLastHour();

//
double averageOutputFromStartOfCollection();

