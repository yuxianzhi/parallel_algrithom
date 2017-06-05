#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <mpi.h>

#define FIRST_VERSION 0

#include "ParallelFloyd.h"
#include "ParallelFloydTest.h"

using namespace std;

static int ProcRank;    // Rank of current process
static int ProcNum;     // Number of processes

const double InfinitiesPercent = 50.0;
const double RandomDataMultiplier = 10;

int Min(int A, int B) {
  int Result = (A < B) ? A : B;

  if((A < 0) && (B >= 0)) Result = B;
  if((B < 0) && (A >= 0)) Result = A;
  if((A < 0) && (B < 0))  Result = -1;

  return Result;
}

int main(int argc, char* argv[]) {
  int *pMatrix;      // Adjacency matrix
  int  Size;         // Size of adjacency matrix
  int *pProcRows;    // Process rows
  int  RowNum;       // Number of process rows

  double start, finish;
  double duration = 0.0;

  int *pSerialMatrix = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if(ProcRank == 0)
    printf("Parallel Floyd algorithm\n");

  // Process initialization
  ProcessInitialiazation(pMatrix, pProcRows, Size, RowNum);

  if (ProcRank == 0) {
	  // Matrix copying
    pSerialMatrix = new int[Size * Size];
    CopyMatrix(pMatrix, Size, pSerialMatrix);
  }

  start = MPI_Wtime();
  // Distributing the initial data between processes
  DataDistribution(pMatrix, pProcRows, Size, RowNum);
  // Testing the distribution
  //TestDistribution(pMatrix, pProcRows, Size, RowNum);

  // Parallel Floyd algorithm
  ParallelFloyd(pProcRows, Size, RowNum);
  //ParallelPrintMatrix(pProcRows, Size, RowNum);

  // Process data collection
  ResultCollection(pMatrix, pProcRows, Size, RowNum);
  //if(ProcRank == 0)
  //  PrintMatrix(pMatrix, Size, Size);
  finish = MPI_Wtime();

  //TestResult(pMatrix, pSerialMatrix, Size);

  duration = finish - start;
  if(ProcRank == 0)
    printf("Time of execution: %f\n", duration);

  if (ProcRank == 0)
    delete []pSerialMatrix;

  // Process termination
  ProcessTermination(pMatrix, pProcRows);

  MPI_Finalize();
  return 0;
}

#if FIRST_VERSION
// Function for allocating the memory and setting the initial values
void ProcessInitialiazation(int *&pMatrix, int *&pProcRows, int& Size, int& RowNum) {
  setvbuf(stdout, 0, _IONBF, 0);

  if(ProcRank == 0) {
    do {
      printf("Enter the number of vertices: ");

      scanf("%d", &Size);

      if(Size < ProcNum)
        printf("The number of vertices should be greater then number of processes\n");

      if(Size % ProcNum != 0)
        printf("The number of vertices should be divisible by number of processes\n");

    } while((Size < ProcNum) || (Size % ProcNum != 0));

    printf("Using graph with %d vertices\n", Size);
  }

  // Broadcast the number of vertices
  MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Number of rows for each process
  RowNum = Size / ProcNum;  

  // Allocate memory for the current process rows
  pProcRows = new int[Size * RowNum];

  if(ProcRank == 0) {
    // Allocate memory for the adjacency matrix
    pMatrix = new int[Size * Size];

    // Data initalization
    DummyDataInitialization(pMatrix, Size);
    //RandomDataInitialization(pMatrix, Size);
  }
}
#else
// Function for allocating the memory and setting the initial values
void ProcessInitialiazation(int *&pMatrix, int *&pProcRows, int& Size, int& RowNum) {
  setvbuf(stdout, 0, _IONBF, 0);

  if(ProcRank == 0) {
    do {
      printf("Enter the number of vertices: ");

      scanf("%d", &Size);

      if(Size < ProcNum)
        printf("The number of vertices should be greater then number of processes\n");
    } while(Size < ProcNum);

    printf("Using graph with %d vertices\n", Size);
  }

  // Broadcast the number of vertices
  MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Number of rows for each process
  int RestRows = Size;
  for(int i = 0; i < ProcRank; i++) 
    RestRows = RestRows - RestRows / (ProcNum - i);
  RowNum = RestRows / (ProcNum - ProcRank);

  // Allocate memory for the current process rows
  pProcRows = new int[Size * RowNum];

  if(ProcRank == 0) {
    // Allocate memory for the adjacency matrix
    pMatrix = new int[Size * Size];

    // Data initalization
    DummyDataInitialization(pMatrix, Size);
    //RandomDataInitialization(pMatrix, Size);
  }
}
#endif

// Function for computational process termination
void ProcessTermination(int *pMatrix, int *pProcRows) {
  if(ProcRank == 0)
    delete []pMatrix;

  delete []pProcRows;
}

// Function for simple setting the initial data
void DummyDataInitialization(int *pMatrix, int Size) {
  for(int i = 0; i < Size; i++)
    for(int j = i; j < Size; j++) {
      if(i == j) pMatrix[i * Size + j] = 0;
      else
        if(i == 0) pMatrix[i * Size + j] = j;
        else       pMatrix[i * Size + j] = -1;

      pMatrix[j * Size + i] = pMatrix[i * Size + j];
    }
}

// Function for setting the data by the random generator
void RandomDataInitialization(int *pMatrix, int Size) {
  srand( (unsigned)time(0) ); 
    
  for(int i = 0; i < Size; i++)
    for(int j = 0; j < Size; j++)
      if(i != j) {
        if((rand() % 100) < InfinitiesPercent)
          pMatrix[i * Size + j] = -1;
        else 
          pMatrix[i * Size + j] = rand() + 1;
      }
      else
        pMatrix[i * Size + j] = 0;
}
#if FIRST_VERSION
// Data distribution among the processes
void DataDistribution(int *pMatrix, int *pProcRows, int Size, int RowNum) {
  MPI_Scatter(pMatrix, RowNum * Size, MPI_INT, pProcRows, RowNum * Size,
    MPI_INT, 0, MPI_COMM_WORLD);
}
#else
// Data distribution among the processes
void DataDistribution(int *pMatrix, int *pProcRows, int Size, int RowNum) {
  int *pSendNum; // The number of elements sent to the process
  int *pSendInd; // The index of the first data element sent to the process

  int RestRows = Size; // Number of rows, that haven’t been distributed yet

  // Allocate memory for temporary objects
  pSendInd = new int[ProcNum];
  pSendNum = new int[ProcNum];

  // Define the disposition of the matrix rows for current process
  RowNum = Size / ProcNum;
  pSendNum[0] = RowNum * Size;
  pSendInd[0] = 0;
  for (int i = 1; i < ProcNum; i++) {
    RestRows   -= RowNum;
    RowNum      = RestRows / (ProcNum - i);
    pSendNum[i] = RowNum * Size;
    pSendInd[i] = pSendInd[i - 1] + pSendNum[i - 1];
  }

  // Scatter the rows
  MPI_Scatterv(pMatrix, pSendNum, pSendInd, MPI_INT, 
    pProcRows, pSendNum[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);

  // Free allocated memory
  delete []pSendNum;
  delete []pSendInd;
}
#endif

#if FIRST_VERSION
// Function for process result collection
void ResultCollection(int *pMatrix, int *pProcRows, int Size, int RowNum) {
  MPI_Gather(pProcRows, RowNum * Size, MPI_INT, pMatrix, RowNum * Size,
    MPI_INT, 0, MPI_COMM_WORLD);
}
#else
// Function for process result collection
void ResultCollection(int *pMatrix, int *pProcRows, int Size, int RowNum) {
  int *pReceiveNum;  // Number of elements, that current process sends
  int *pReceiveInd;  /* Index of the first element from current process 
                        in result vector */
  int RestRows = Size; // Number of rows, that haven’t been gathered yet

  // Allocate memory for temporary objects
  pReceiveNum = new int[ProcNum];
  pReceiveInd = new int[ProcNum];

  // Define the disposition of the result vector block of current process
  RowNum = Size / ProcNum;
  pReceiveInd[0] = 0;
  pReceiveNum[0] = RowNum * Size;

  for(int i = 1; i < ProcNum; i++) {
    RestRows      -= RowNum;
    RowNum = RestRows / (ProcNum - i);
    pReceiveNum[i] = RowNum * Size;
    pReceiveInd[i] = pReceiveInd[i - 1] + pReceiveNum[i - 1];
  }

  // Gather the whole matrix on process with rank 0
  MPI_Gatherv(pProcRows, pReceiveNum[ProcRank], MPI_INT,
    pMatrix, pReceiveNum, pReceiveInd, MPI_INT, 0, MPI_COMM_WORLD);

  // Free allocated memory
  delete []pReceiveNum;
  delete []pReceiveInd;
}
#endif

// Parallel Floyd algorithm
void ParallelFloyd(int *pProcRows, int Size, int RowNum) {
  int *pRow = new int[Size];
  int t1, t2;
  for(int k = 0; k < Size; k++) {
    // Distribute row among all processes 
    RowDistribution(pProcRows, Size, RowNum, k, pRow);

#if 0
	if(ProcRank == 0) { 
	  printf("Row %d after distribution:", k);
	  fflush(stdout);
      PrintMatrix(pRow, 1, Size);
	}
#else
    // Update adjacency matrix elements
    for(int i = 0; i < RowNum; i++) 
      for(int j = 0; j < Size; j++)
        if( (pProcRows[i * Size + k] != -1) &&
            (pRow     [j]            != -1)) {
          t1 = pProcRows[i * Size + j];
          t2 = pProcRows[i * Size + k] + pRow[j];

        pProcRows[i * Size + j] = Min(t1, t2);
      }
#endif
  }
    
  delete []pRow;
}
#if FIRST_VERSION
// Function for row broadcasting among all processes
void RowDistribution(int *pProcRows, int Size, int RowNum, int k, int *pRow) {
  int ProcRowRank = k / RowNum;  // Process rank with the row k
  int ProcRowNum  = k - ProcRowRank * RowNum; // Process row number

  if(ProcRowRank == ProcRank)
    // Copy Kth row to pRow array
    copy(&pProcRows[ProcRowNum * Size], &pProcRows[ProcRowNum * Size] + Size, pRow);

  // Broadcast row to all processes  
  MPI_Bcast(pRow, Size, MPI_INT, ProcRowRank, MPI_COMM_WORLD);
}
#else
// Function for row broadcasting among all processes
void RowDistribution(int *pProcRows, int Size, int RowNum, int k, int *pRow) {
  int ProcRowRank;  // Process rank with the row k
  int ProcRowNum;   // Process row number

  // Finding the process rank with the row k
  int RestRows = Size;
  int Ind = 0;
  int Num = Size / ProcNum;
  
  for(ProcRowRank = 1; ProcRowRank < ProcNum + 1; ProcRowRank ++) {
    if(k < Ind + Num ) break;
    RestRows -= Num;
    Ind      += Num;
    Num       = RestRows / (ProcNum - ProcRowRank);
  }
  ProcRowRank = ProcRowRank - 1;
  ProcRowNum  = k - Ind;

  if(ProcRowRank == ProcRank)
    // Copy the row to pRow array
    copy(&pProcRows[ProcRowNum*Size],&pProcRows[(ProcRowNum+1)*Size],pRow);

  // Broadcast row to all processes  
  MPI_Bcast(pRow, Size, MPI_INT, ProcRowRank, MPI_COMM_WORLD);
}
#endif

// Function for formatted output of all stripes
void ParallelPrintMatrix(int *pProcRows, int Size, int RowNum) {
  for(int i = 0; i < ProcNum; i++) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == i) {
      printf("ProcRank = %d\n", ProcRank); 
      fflush(stdout);
      printf("Proc rows:\n");
      fflush(stdout);
      PrintMatrix(pProcRows, RowNum, Size);
      fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
}

// Function for testing the data distribution
void TestDistribution(int *pMatrix, int *pProcRows, int Size, int RowNum) {
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    printf("Initial adjacency matrix:\n");
    PrintMatrix(pMatrix, Size, Size);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  ParallelPrintMatrix(pProcRows, Size, RowNum);
}

// Testing the result of parallel Floyd algorithm
void TestResult(int *pMatrix, int *pSerialMatrix, int Size) {
  MPI_Barrier(MPI_COMM_WORLD);

  if(ProcRank == 0) {
    SerialFloyd(pSerialMatrix, Size);
    if(!CompareMatrices(pMatrix, pSerialMatrix, Size)) {
      printf("Results of serial and parallel algorithms are "
        "NOT identical. Check your code\n");
    }
    else {
      printf("Results of serial and parallel algorithms are "
        "identical\n");
    }
  }
}
