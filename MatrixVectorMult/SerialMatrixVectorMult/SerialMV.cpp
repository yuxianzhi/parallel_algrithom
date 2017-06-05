#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function for simple definition of matrix and vector elements
void DummyDataInitialization (double* pMatrix, double* pVector, int Size) {
  int i, j;  // Loop variables

  for (i=0; i<Size; i++) {
    pVector[i] = 1;
    for (j=0; j<Size; j++)
      pMatrix[i*Size+j] = i;
  }
}

// Function for random definition of matrix and vector elements
void RandomDataInitialization(double* pMatrix, double* pVector, int Size) {
  int i, j;  // Loop variables
  srand(unsigned(clock()));
  for (i=0; i<Size; i++) {
    pVector[i] = rand()/double(1000);
    for (j=0; j<Size; j++)
      pMatrix[i*Size+j] = rand()/double(1000);
  }
}

// Function for memory allocation and data initialization
void ProcessInitialization (double* &pMatrix, double* &pVector, 
    double* &pResult, int &Size) {
  // Setting the size of the initial matrix and vector
 /* do {
    printf("\nEnter size of the initial objects: ");
    scanf("%d", &Size);
    printf("\nChosen objects' size = %d\n", Size);
	if (Size <= 0)
      printf("\nSize of objects must be greater than 0!\n");
  }
  while (Size <= 0);*/
  // Memory allocation 
  pMatrix = new double [Size*Size];
  pVector = new double [Size];
  pResult = new double [Size];
  // Definition of matrix and vector elements
  RandomDataInitialization(pMatrix, pVector, Size);
}

// Function for formatted matrix output
void PrintMatrix (double* pMatrix, int RowCount, int ColCount) {
  int i, j; // Loop variables
  for (i=0; i<RowCount; i++) {
    for (j=0; j<ColCount; j++)
	  printf("%7.4f ", pMatrix[i*RowCount+j]);
	printf("\n");
  }
}

// Function for formatted vector output
void PrintVector (double* pVector, int Size) {
  int i;
  for (i=0; i<Size; i++)
    printf("%7.4f ", pVector[i]);
}

// Function for matrix-vector multiplication
void ResultCalculation(double* pMatrix, double* pVector, double* pResult,
  int Size) {
  int i, j;  // Loop variables
  for (i=0; i<Size; i++) {
    pResult[i] = 0;
	for (j=0; j<Size; j++)
	  pResult[i] += pMatrix[i*Size+j]*pVector[j];
  }
}

// Function for computational process termination
void ProcessTermination(double* pMatrix,double* pVector,double* pResult) {
  delete [] pMatrix;
  delete [] pVector;
  delete [] pResult;
}


void Experiment(int Size) {
  double* pMatrix;  // The first argument - initial matrix
  double* pVector;  // The second argument - initial vector
  double* pResult;  // Result vector for matrix-vector multiplication 
  //int Size;		    // Sizes of initial matrix and vector
  time_t start, finish;
  double duration;

  //printf("Serial matrix-vector multiplication program\n");
  // Memory allocation and data initialization
  ProcessInitialization(pMatrix, pVector, pResult, Size);

  // Matrix and vector output
  //printf ("Initial Matrix \n"); 
  //PrintMatrix(pMatrix, Size, Size);
  //printf("Initial Vector \n");
  //PrintVector(pVector, Size);

  // Matrix-vector multiplication
  start = clock();
  ResultCalculation(pMatrix, pVector, pResult, Size);
  finish = clock();
  duration = (finish-start)/double(CLOCKS_PER_SEC);
  
  // Printing the result vector
  //printf ("\n Result Vector: \n");
 // PrintVector(pResult, Size);

  // Printing the time spent by matrix-vector multiplication
  FILE * stream = fopen("SerialMV.txt", "a+");
  fprintf(stream, "Size = %d, Time = %f\n", Size, duration);
  fclose(stream);

  // Computational process termination
  ProcessTermination(pMatrix, pVector, pResult);
  //getch();
}

int main()
{
	for (int i=1000; i<=10000; i=i+1000)
		Experiment(i);
	return 0;
}
