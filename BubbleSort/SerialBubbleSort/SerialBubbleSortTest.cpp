#include <algorithm>
#include <cstdio>
using namespace std;

// Function for formatted data output
void PrintData(double *pData, int DataSize) {
  for(int i = 0; i < DataSize; i++)
    printf("%7.4f ", pData[i]);
  printf("\n");
}

// Sorting by the standard library algorithm
void SerialStdSort(double *pData, int DataSize) {
  sort(pData, pData + DataSize);
}
