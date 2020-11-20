/*

How to run:
mpic++ 2.c
mpirun -np 4 a.out 10

no. of processes = 4
number of elements = 10


*/
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>
#include <random>
#include <iostream>
#include <chrono>
#include<time.h> 
using namespace std::chrono;


int main(int argc, char **argv)
{

    

    int rank;
    int np;
    MPI_Init(&argc, &argv);

    char *num = argv[5];
    char num1[strlen(num)];
    int i,j=0;
    for(i=10;i<strlen(num);i++)
    {
        num1[j]+=num[i];
        j++;
    }
    int n = atoi(num1);
    // int n= give_number_of_elements_of_array(argv[5]);
    // printf("%d\n", n);
    int random_arr[n];
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    
    srand(time(0)+rank);
    
    printf("Original order for rank %d: ",rank);
    for(i=0;i<n;i++)
    {
        random_arr[i]= rand()%10;
        printf("%d ", random_arr[i]);
    }
    printf("\n");
    
    printf("New order for rank %d: ",rank);
  
    int temp[n];
    
    MPI_Send(random_arr, n, MPI_INT, ((rank + 1) % np), 0, MPI_COMM_WORLD);  
    if(rank!=0)
    {  
      MPI_Recv(temp, n, MPI_INT, (rank- 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       for(int i=0;i<n;i++)
      {
        random_arr[i] = temp[i];
      }
    }
    if(rank==0)
    {  
      MPI_Recv(temp, n, MPI_INT, np-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       for(int i=0;i<n;i++)
      {
        random_arr[i] = temp[i];
      }
    } 
    for(i=0;i<n;i++)
    {
        // random_arr[i]= rand()%10;
        printf("%d ", random_arr[i]);
    }
    printf("\n");

  // i = 0;
  // printf("In rank: %d var[%d]: %d\n", rank, i, var[i]);
  MPI_Finalize();

  return 0;
}
