#ifndef PARALLELBUBBLESORTTEST_H_
#define PARALLELBUBBLESORTTEST_H_

// Function for copying the sorted data
void CopyData(double *pData, int DataSize, double *pDataCopy);

// Function for comparing the data
bool CompareData(double *pData1, double *pData2, int DataSize);

// Serial bubble sort algorithm
void SerialBubbleSort(double *pData, int DataSize);

// Sorting by the standard library algorithm
void SerialStdSort(double *pData, int DataSize);

// Function for formatted data output
void PrintData(double *pData, int DataSize);

#endif
