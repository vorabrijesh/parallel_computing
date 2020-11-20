/*
How to run:

To compile: mpic++ 10.cc
To run: $mpirun -np 4 a.out

Here the number of processes is 4.
you can change the number of elements in  array by changing the variable n and also iteration variable.

*/
#include <random>

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <mpi.h>
#include <mutex>         


using namespace std::chrono;

const int n = 5;              // number of elements in array
const int iteration = 1;      // number of iterations
//int num_process=4;

// double mat1[3][3];
// int sum[nthreads] = { 0 };
int main(int argc, char ** argv)
{

  
  	
    int rank;
    int np;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    
    double arr[n];
    

    // assign random values.
    double lbound = 0;
    double ubound = (rank+1)*10;
    std::uniform_real_distribution<double> urd(lbound, ubound);
    std::default_random_engine re;
    

    for (int i = 0; i < n; i++)
    {
        if(i==0)
        {
            printf("Initial rank %d has: ",rank);
        }
        arr[i] =  urd(re);
        std::cout<<arr[i]<<" ";
    }
    printf("\n");
    printf("\n");
    // printf("\n");

    lbound = 0;
    ubound = 100;
    std::uniform_real_distribution<double> urd1(lbound, ubound);
    std::default_random_engine re1;
   
    int old_x = 0;
    int new_x = ((int)urd1(re1) )% np;
    int temp[n]={0};
    // for(int j=0; j<iteration; j++)
    // {
        if(rank==0)
        {
            std::cout<<"old_x: " <<old_x<<"\n";
            std::cout<<"new_x: " <<new_x<<"\n";
        }
        if(rank == old_x)
        {
            MPI_Send(arr, n, MPI_INT, new_x, 0, MPI_COMM_WORLD);
        }
        if(rank == new_x)
        {

            MPI_Recv(arr, n, MPI_INT, old_x, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           
        } 
    // }
    
	for (int i = 0; i < n; i++)
    {
        if(i==0)
        {
            printf("Rank %d in iteration 0 has : ",rank);
        }
        std::cout<<arr[i]<<" ";
    }
    printf("\n");
	MPI_Finalize();
	
    return 0;
}

/*
Initial rank 0 has: 1.31538 4.5865 2.18959 6.78865 


old_x: 0
new_x: 1
Initial rank 1 has: 2.63076 9.173 4.37918 13.5773 


old_x: 0
new_x: 1
Initial rank 2 has: 3.94613 13.7595 6.56878 20.3659 


old_x: 0
new_x: 1
Rank 2 in iteration 0 has : 3.94613 13.7595 6.56878 20.3659 
Rank 0 in iteration 0 has : 1.31538 4.5865 2.18959 6.78865 
Initial rank 3 has: 5.26151 18.346 8.75837 27.1546 


old_x: 0
new_x: 1
Rank 1 in iteration 0 has : 1.31538 4.5865 4.37918 13.5773 
Rank 3 in iteration 0 has : 5.26151 18.346 8.75837 27.1546 


In the above output the initial rank 0 elements should be copied to rank 1 array
but all elements are not copied only 1st two elements are copied.

*/