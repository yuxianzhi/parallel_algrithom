#include <cstdio>
#include "SerialFloydTest.h"

using namespace std;

// Function for formatted matrix output
void PrintMatrix(int *pMatrix, int RowCount, int ColCount) {
  for(int i = 0; i < RowCount; i++) {
    for(int j = 0; j < ColCount; j++)
      printf("%7d", pMatrix[i * ColCount + j]);
    printf("\n");
  }
}
