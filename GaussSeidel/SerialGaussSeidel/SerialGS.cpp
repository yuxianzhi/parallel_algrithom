#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// Function for formatted matrix output
void PrintMatrix (double* pMatrix, int RowCount, int ColCount) {
  int i, j; // Loop variables
  for (i=0; i<RowCount; i++) {
    for (j=0; j<ColCount; j++)
      printf("%7.4f ", pMatrix[i*ColCount+j]);
    printf("\n");
  }
}
// Function for the Gauss-Seidel algoritm
void ResultCalculation(double* pMatrix, int Size, double &Eps, 
  int &Iterations) {
  int i, j;  // Loop variables  
  double dm, dmax,temp;
  Iterations = 0;   
  do {
    dmax = 0;
    for (i = 1; i < Size - 1; i++)
      for(j = 1; j < Size - 1; j++) {
        temp = pMatrix[Size * i + j];
        pMatrix[Size * i + j] = 0.25 * (pMatrix[Size * i + j + 1] + 
                                        pMatrix[Size * i + j - 1] + 
                                        pMatrix[Size * (i + 1) + j] + 
                                        pMatrix[Size * (i - 1) + j]);
        dm = fabs(pMatrix[Size * i + j] - temp);
        if (dmax < dm) dmax = dm;
      }    
      Iterations++;
	  if (Iterations==1) PrintMatrix (pMatrix, Size, Size);
  }
  while (dmax > Eps);  
}

// Function for computational process termination
void ProcessTermination(double* pMatrix) {
  delete [] pMatrix;
}

// Function for simple setting the grid node values
void DummyDataInitialization (double* pMatrix, int Size) {
  int i, j;  // Loop variables
  double h = 1.0 / (Size - 1);
  // Setting the grid node values
  for (i=0; i<Size; i++) {
    for (j=0; j<Size; j++) 
      if ((i==0) || (i== Size-1) || (j==0) || (j==Size-1))
        pMatrix[i*Size+j] = 100;
      else
        pMatrix[i*Size+j] = 0;
  }
}
// Function for memory allocation and setting the initial values
void ProcessInitialization (double* &pMatrix, int &Size, double &Eps) {
  // Setting the size of initial matrix
  do {
    printf("\nEnter the grid size of the initial objects: ");
    scanf("%d", &Size);
    printf("\nChosen grid size = %d", Size);
    if (Size <= 2)
      printf("\nSize of grid must be greater than 2!\n"); 
  }
  while (Size <= 2);
  // Setting the required accuracy
  do {
    printf("\nEnter the required accuracy: ");
    scanf("%lf", &Eps);
    printf("\nChosen accuracy = %lf", Eps);
    if (Eps <= 2)
      printf("\nAccuracy must be greater than 0!\n"); 
  }
  while (Eps <= 0);
  // Memory allocation 
  pMatrix = new double [Size*Size];

  // Setting the grid node values
  DummyDataInitialization(pMatrix, Size);
}
// Function for simple definition of matrix and vector elements
int main() {
  double* pMatrix;     // The matrix of the grid nodes
  int     Size;       // The matrix size
  double  Eps;        // The required accuracy
  int     Iterations; // The iteration number  
  
  printf ("Serial Gauss - Seidel algorithm\n");
  ProcessInitialization(pMatrix, Size, Eps);
  // Matrix output
  printf ("Initial Matrix: \n");
  PrintMatrix (pMatrix, Size, Size);
  // The Gauss-Seidel method
  ResultCalculation(pMatrix, Size, Eps, Iterations);

  // Printing the result
  printf("\n Number of iterations: %d\n",Iterations);
  printf ("\n Result matrix: \n");
  PrintMatrix (pMatrix, Size, Size);

  getch(); 
  // Computational process termination
  ProcessTermination(pMatrix);
  return 0;
}
