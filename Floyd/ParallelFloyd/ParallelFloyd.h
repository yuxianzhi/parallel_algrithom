#ifndef PARALLELFLOYD_H_
#define PARALLELFLOYD_H_

int Min(int A, int B);

// Function for allocating the memory and setting the initial values
void ProcessInitialiazation(int *&pMatrix, int *&pProcRows, int& Size, int& RowNum);

// Function for computational process termination
void ProcessTermination(int *pMatrix, int *pProcRows);

// Function for simple setting the initial data
void DummyDataInitialization(int *pMatrix, int Size);

// Function for setting the data by the random generator
void RandomDataInitialization(int *pMatrix, int Size);

// Data distribution among the processes
void DataDistribution(int *pMatrix, int *pProcRows, int Size, int RowNum);

// Function for process result collection
void ResultCollection(int *pMatrix, int *pProcRows, int Size, int RowNum);

// Parallel Floyd algorithm
void ParallelFloyd(int *pProcRows, int RowSize, int RowNum);

// Function for row broadcasting among all processes
void RowDistribution(int *pProcRows, int Size, int RowNum, int k, int *pRow);

// Function for formatted output of all stripes
void ParallelPrintMatrix(int *pProcRows, int Size, int RowNum);

// Function for testing the data distribution
void TestDistribution(int *pMatrix, int *pProcRows, int Size, int RowNum);

// Testing the result of parallel Floyd algorithm
void TestResult(int *pMatrix, int *pSerialMatrix, int Size);

#endif