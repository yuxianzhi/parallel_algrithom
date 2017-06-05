#ifndef SERIALFLOYD_H_
#define SERIALFLOYD_H_

int Min(int A, int B);

// Function for allocating the memory and setting the initial values
void ProcessInitialiazation(int *&pMatrix, int& Size);

// Function for computational process termination
void ProcessTermination(int *pMatrix);

// Function for simple setting the initial data
void DummyDataInitialization(int *pMatrix, int Size);

// Function for initializing the data by the random generator
void RandomDataInitialization(int *pMatrix, int Size);

// Serial Floyd algorithm
void SerialFloyd(int *pMatrix, int Size);

#endif
