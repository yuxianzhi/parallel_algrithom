// start.c :矩阵A*矩阵B放入矩阵C
//矩阵A和矩阵B大小都是1000x1000
//pthread版本
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include<pthread.h>
#include "sys/time.h"

#define min(a,b) ((a < b) ? a : b)
#define matrix_size 1000
//设置线程数
#define threads 4
int *A, *B, *C;
int matrix_size_2;
int block;




//读A矩阵
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

//读B转置矩阵
void readB(){
      int row, col;
      FILE* File_B = fopen("b.txt", "r"); 
      for(row=0; row<matrix_size; row++){
         for(col=0; col<matrix_size; col++){
            fscanf(File_B, "%d ", &B[col*matrix_size+row]);
        }
      }
      fclose(File_B);
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

//多线程矩阵乘法
void mul(void* rank){
   long thread=(long)rank;
   int start=(int)thread*block;
   int end=min(start+block, matrix_size);
   int i, j, k, temp1, local1;
   for(i=start; i<end; i++){
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
int main()
{
 struct timeval start,end;
 //获取开始时间
 gettimeofday(&start,NULL);
 //分配空间
 float all_time;
 matrix_size_2=matrix_size*matrix_size;
 A = (int *)malloc(sizeof(int)*matrix_size_2);
 B = (int *)malloc(sizeof(int)*matrix_size_2);
 C = (int *)malloc(sizeof(int)*matrix_size_2);
 block=(threads-1+matrix_size)/threads;

 long thread;
 pthread_t* thread_handles=malloc(sizeof(pthread_t)*threads);
 
 //并行读入矩阵A，B（并把B转置）
 #pragma omp parallel for
 for(thread=0; thread<2; thread++)
   if(thread==0)
     readA();
   else
     readB();

 //矩阵乘法
 for(thread=0; thread<threads; thread++){
     pthread_create(&thread_handles[thread], NULL, mul, (void *)thread);
 }
 
 for(thread=0; thread<threads; thread++){
     pthread_join(thread_handles[thread], NULL);
 }
 //输出矩阵C
 writeC();
 //获取结束时间
 gettimeofday(&end,NULL);
 //计算时间
 all_time = (end.tv_sec-start.tv_sec)+(float)(end.tv_usec-start.tv_usec)/1000000.0;
 printf("run time:\t%f s\n",all_time);

 //释放空间
 free(A);
 free(B);
 free(C);
 free(thread_handles);
 return 0;
}

