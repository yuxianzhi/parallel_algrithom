#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "cuda.h"

#define matrix_size 1000
#define grid_size matrix_size
#define block_size 256
#define matrix_size_2 matrix_size*matrix_size

int *A, *B, *C;


//读B矩阵（B按行存）
void readB(){
      FILE* File_B = fopen("b.txt", "r"); 
      int *temp=B;
      int *up=(int *)(B+matrix_size_2);
      while(temp<up){
         fscanf(File_B, "%d ", temp);
         temp++;
      } 
      fclose(File_B);
}

//读A转置矩阵(A按行存)
void readA(){
      FILE* File_A = fopen("a.txt", "r"); 
      int *temp=A;
      int *up=(int *)(A+matrix_size_2);
      while(temp<up){
         fscanf(File_A, "%d ", temp);
         temp++;
      } 
      fclose(File_A);
}

//输出矩阵C
void writeC(){
      FILE* File_C = fopen ("c.txt", "w" );
      int t, row, col;
      for(row=0; row<matrix_size; row++){
         t=row*matrix_size;
         for(col=0; col<matrix_size; col++){
            fprintf(File_C, "%d ", C[t+col]);
         }
         fprintf(File_C, " \n");
      }
      fclose(File_C);
}



//cuda矩阵乘法
__global__ void multiply(int *D_A, int *D_B, int *D_C){
  int t1= threadIdx.x;
  int t2= blockIdx.x;
  int t3= blockDim.x;
  int t4= t2 * matrix_size;
  int i, temp, j;
  __shared__ int shared_A[1000];
  for(i=t1; i<matrix_size; i+=t3){
     shared_A[i]=D_A[t4+i];
  }
  __syncthreads();

  for(i=t1; i<matrix_size; i+=t3){
     temp=0;
     for(j=0; j<matrix_size; j++){
        temp+=shared_A[j]*D_B[j*matrix_size+i];
     }
     D_C[t4+i]=temp;
  }
  
}


int main()
{
   cudaEvent_t event1, event2;
   cudaEventCreate(&event1);
   cudaEventCreate(&event2);
   
   cudaEventRecord(event1, 0 );
   int thread;

   //申请分配设备和主机内存
   int size=sizeof(int)*matrix_size_2;
   int *D_A, *D_B, *D_C;
   A = (int *)malloc(size);
   B = (int *)malloc(size);
   C = (int *)malloc(size);
   cudaMalloc((void **)&D_A, size);
   cudaMalloc((void **)&D_B, size);
   cudaMalloc((void **)&D_C, size);
  
   #ifdef _OPENMP
      printf("paralell read file\n");
      //并行读入矩阵A，B（并把B转置）
      #pragma omp parallel for
      for(thread=0; thread<2; thread++)
         if(thread==0)
           readA();
         else
           readB();
   #else
      printf("serial read file\n");
      readA();
      readB();
   #endif

   //向GPU上传输数据
   cudaMemcpy(D_A, A, size, cudaMemcpyHostToDevice);
   cudaMemcpy(D_B, B, size, cudaMemcpyHostToDevice);

   dim3 block(block_size, 1, 1), grid(grid_size, 1, 1);
   
   multiply<<<grid, block>>>(D_A, D_B, D_C);

   //将结果传输会主机
   cudaMemcpy(C, D_C, size, cudaMemcpyDeviceToHost);

   //输出矩阵C
   writeC();
   

   float time;
   
   cudaEventRecord(event2, 0 );
   cudaEventSynchronize(event2);
   cudaEventElapsedTime(&time, event1, event2);
   cudaEventDestroy(event1);
   cudaEventDestroy(event2);
   //printf("run time:\t%f s\n",time);

   //释放空间
   free(A);
   free(B);
   free(C);
   cudaFree(D_A);
   cudaFree(D_B);
   cudaFree(D_C);

   return 0;
}

