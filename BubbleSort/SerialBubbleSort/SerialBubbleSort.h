#ifndef SERIALBUBBLESORT_H_
#define SERIALBUBBLESORT_H_

// Function for allocating the memory and setting the initial values
void ProcessInitialization(double *&Data, int& DataSize);

// Function for computational process termination
void ProcessTermination(double *Data);

// Function for simple setting the initial data
void DummyDataInitialization(double*& Data, int& DataSize);

// Function for initializing the data by the random generator
void RandomDataInitialization(double *&Data, int& DataSize);

// Serial bubble sort algorithm
void SerialBubble(double *Data, int DataSize);

#endif