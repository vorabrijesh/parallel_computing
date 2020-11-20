/*
How to run:
mpicc 8.c
mpirun -np 10 a.out 10

no. of processes = 10
number of elements =10

*/
// #include <random>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>


int main(int argc, char **argv)
{
    srand(0);

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

    if(rank==0)
    {
        printf("orginal vector is: ");
    }

    for(i=0;i<n;i++)
    {
        random_arr[i]= rand()%10;
        if(rank==0)
        {
            printf("%d ", random_arr[i]);
        }
    }
    printf("\n");
    
    int total;
    int len_subarr= n/np;
    int k=0;
    
    for(k=0;k<np-1;k++)
    {
        if(rank==k)
        {
            int i=0;
            int local_sum[1];
            local_sum[0] = 0;
            
            for(i=rank*len_subarr;i<(rank+1)*len_subarr;i++)
            {
                local_sum[0]+=random_arr[i]*random_arr[i];

            }
            printf("local_sum in rank %d is %d\n", rank, local_sum[0]);
            if(rank!=0)
                MPI_Send(local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            if(rank==0)
                total+=local_sum[0];
        }
        
    }
    if(rank==np-1)
    {
        int i=0;
        int local_sum[1];
        local_sum[0] = 0;
        for(i=rank*len_subarr;i<n;i++)
        {
            local_sum[0]+=random_arr[i]*random_arr[i];
        }
        printf("local_sum in rank %d is %d\n", rank, local_sum[0]);
        MPI_Send(local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
    }
     if(rank==0)
    {
        int local_sum[0];
        for(i=0;i<np-1;i++)
        {
            MPI_Recv(local_sum, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total+=local_sum[0];
        }
        printf("total sum = %d\n",total);
    }

   
    MPI_Finalize();

    return 0;
}
