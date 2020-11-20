/*
How to run:
mpicc 3.c
mpirun -np 4 a.out 10

no. of processes = 4
number of elements =10

*/
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>


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
    

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    int len_subarr= n/np;
    long long int total=0;
    if(rank!=np-1)
    {
    	int random_arr[len_subarr];
    	int i=0;
    	printf("rank: %d, sub array: ",rank);
    	long long int local_sum[1] ={0};
    	for(i=0;i<len_subarr;i++)
	    {
	        random_arr[i]= len_subarr*rank+i;
	        local_sum[0]+=random_arr[i];
	        printf("%d ", random_arr[i]);
	    }
	    MPI_Send(local_sum, 1, MPI_INT, np-1, 0, MPI_COMM_WORLD);
	    printf("\n");
    }
    else{

    	int l = n- len_subarr*(np-1);
    	int random_arr[l];
    	int i=0;
    	long long int local_sum[1] ={0};
    	printf("rank: %d, sub array: ",rank);
    	for(i=0;i<l;i++)
	    {
	        random_arr[i]=len_subarr*rank+i;
	        local_sum[0]+=random_arr[i];
	        printf("%d ", random_arr[i]);
	    }
	    total+=local_sum[0];
	    for(i=0;i<np-1;i++)
        {
            MPI_Recv(local_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total+=local_sum[0];
        }
	    printf("\nTotal = %lld\n", total);
    }


   
    MPI_Finalize();

    return 0;
}
