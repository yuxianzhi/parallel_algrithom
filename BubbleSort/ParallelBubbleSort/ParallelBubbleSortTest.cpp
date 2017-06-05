#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include "ParallelBubbleSortTest.h"

using namespace std;

// Function for copying the sorted data
void CopyData(double *pData, int DataSize, double *pDataCopy) {
  copy(pData, pData + DataSize, pDataCopy);
}

// Function for comparing the data
bool CompareData(double *pData1, double *pData2, int DataSize) {
  return equal(pData1, pData1 + DataSize, pData2);
}

// Serial bubble sort algorithm
void SerialBubbleSort(double *pData, int DataSize) {
  double Tmp;

  for(int i = 1; i < DataSize; i++) 
    for(int j = 0; j < DataSize - i; j++)
      if(pData[j] > pData[j + 1]) {
        Tmp          = pData[j];
        pData[j]     = pData[j + 1];
        pData[j + 1] = Tmp;
      }
}

// Sorting by the standard library algorithm
void SerialStdSort(double *pData, int DataSize) {
  sort(pData, pData + DataSize);
}

// Function for formatted data output
void PrintData(double *pData, int DataSize) {
  for(int i = 0; i < DataSize; i++)
    printf("%7.4f ", pData[i]);
  printf("\n");
}
