#ifndef PARALLELFLOYDTEST_H_
#define PARALLELFLOYDTEST_H_

// Function for copying the matrix
void CopyMatrix(int *pMatrix, int Size, int *pMatrixCopy);

// Function for comparing the matrices
bool CompareMatrices(int *pMatrix1, int *pMatrix2, int Size);

// Serial Floyd algorithm
void SerialFloyd(int *pMatrix, int Size);

// Function for formatted matrix output
void PrintMatrix(int *pMatrix, int RowCount, int ColCount);

#endif