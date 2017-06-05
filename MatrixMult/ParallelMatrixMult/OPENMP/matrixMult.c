// start.c :矩阵A*矩阵B放入矩阵C
//矩阵A和矩阵B大小都是1000x1000
//openmp版本
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include "sys/time.h"

#define matrix_size 1000
//设置线程数
#define threads 4

int main()
{
 struct timeval start,end;
 //获取开始时间
 gettimeofday(&start,NULL);
 //分配空间
 float all_time;
 int matrix_size_2=matrix_size*matrix_size;
 int *A = (int *)malloc(sizeof(int)*matrix_size_2);
 int *B = (int *)malloc(sizeof(int)*matrix_size_2);
 int *C = (int *)malloc(sizeof(int)*matrix_size_2);
 int row, col, i;

 //并行读入矩阵A，B（并把B转置）
 omp_set_num_threads(2);
 #pragma omp parallel for
 for(i=0; i<2; i++){
   if(i==0){
      FILE* File_A = fopen("a.txt", "r"); 
      int *temp=A;
      int *up=(int *)(A+matrix_size_2);
      while(temp<up){
         fscanf(File_A, "%d ", temp);
         temp++;
      } 
      fclose(File_A);
   }
   else{
      FILE* File_B = fopen("b.txt", "r"); 
      for(row=0; row<matrix_size; row++){
         for(col=0; col<matrix_size; col++){
            fscanf(File_B, "%d ", &B[col*matrix_size+row]);
        }
      }
      fclose(File_B);
   }
 }


 //矩阵乘法
 omp_set_num_threads(threads);
 #pragma omp parallel
 {
   int j, k;
   int local1;
   int temp1;
   #pragma omp for
   for(i=0; i<matrix_size; i++){
     temp1=i*matrix_size;
     for(j=0; j<matrix_size; j++){
        local1=0;
        #pragma ivdep
        for(k=0; k<matrix_size; k++){
           local1+=A[temp1+k]*B[j*matrix_size+k];
        }
        C[temp1+j]=local1;
     }
   }
 }

 //输出矩阵C
 FILE* File_C = fopen ("c.txt", "w" );
 int t;
 for(row=0; row<matrix_size; row++){
     t=row*matrix_size;
     for(col=0; col<matrix_size; col++){
        fprintf(File_C, "%d ", C[t+col]);
     }
     fprintf(File_C, " \n");
 }
 fclose(File_C);

 //获取结束时间
 gettimeofday(&end,NULL);
 //计算时间
 all_time = (end.tv_sec-start.tv_sec)+(float)(end.tv_usec-start.tv_usec)/1000000.0;
 printf("run time:\t%f s\n",all_time);

 //释放空间
 free(A);
 free(B);
 free(C);
 return 0;
}

