#ifndef PARALLELBUBBLESORT_H_
#define PARALLELBUBBLESORT_H_

enum split_mode { KeepFirstHalf, KeepSecondHalf };

// Function for allocating the memory and setting the initial values
void ProcessInitialization(double *&pData, int& DataSize, double *&pProcData, int& BlockSize);

// Function for computational process termination
void ProcessTermination(double *pData, double *pProcData);

// Function for simple setting the data to be sorted
void DummyDataInitialization(double*& pData, int& DataSize);

// Function for initializing the data by the random generator
void RandomDataInitialization(double *&pData, int& DataSize);

// Parallel bubble sort algorithm
void ParallelBubble(double *pProcData, int BlockSize);

// Data distribution among the processes
void DataDistribution(double *pData, int DataSize, double *pProcData, int BlockSize);

// Function for data collection
void DataCollection(double *pData, int DataSize, double *pProcData, int BlockSize);

// Function for data exchange between the neighboring processes
void ExchangeData(double *pProcData, int BlockSize, int DualRank, double *pDualData);

// Function for data exchange between the neighboring processes
void ExchangeData(double *pProcData, int BlockSize, int DualRank, double *pDualData, int DualBlockSize);

// Function for testing the data distribution
void TestDistribution(double *pData, int DataSize, double *pProcData, int BlockSize);

// Function for parallel data output
void ParallelPrintData(double *pProcData, int BlockSize);

// Function for testing the result of parallel bubble sort
void TestResult(double *pData, double *pSerialData, int DataSize);

#endif