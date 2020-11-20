#include <stdio.h>
#include <mpi.h>

/*
 Graph : 0 <-> 1 <-> 2 <-> 3
 
A*A*A:
	[[0 2 0 1]
	 [2 0 3 0]
	 [0 3 0 2]
	 [1 0 2 0]]
A:
[[0 1 0 0]
 [1 0 1 0]
 [0 1 0 1]
 [0 0 1 0]]

	Here, there is a assumption that the number of processes are same as number of rows in adjacency matrix. 
	Thus, n = np. 
	Further, the adjacency matrix fits in single process.
	
*/

 int n =4;
 int mat[4][4] = {{0, 1, 0, 0},{1, 0, 1, 0},{0, 1, 0, 1},{0, 0, 1, 0}};
 int temp[4][4]= {{0, 1, 0, 0},{1, 0, 1, 0},{0, 1, 0, 1},{0, 0, 1, 0}};

 //{{0, 1, 0, 0},{1, 0, 1, 1},{0, 1, 0, 0},{0, 1, 0, 0}};   // Graph: 0 <-> 1 <-> 2
															//				  <-> 3				
 //{{1, 0, 0, 0},{0, 1, 0, 0},{0, 0 , 1, 0},{0, 0, 0, 1}}; // Identity matrix
 //{{0, 1, 0, 0},{1, 0, 1, 0},{0, 1, 0, 1},{0, 0, 1, 0}};  
 
 
 int source = 0;
 int dest = 1;
 
int main(int argc, char **argv)
{
	
	
	int rank;
	int np;
	int len=1;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	
	// temp_row to store calulated temporary row.
	int temp_row[n];
	
	// run n times
	for(int size=0;size<n;size++)
	{
		for(int k=0;k<n;k++)
		{
			if(rank==k)
			{	
				for(int i=0;i<n;i++)
				{
					temp_row[i]=0;
					for(int j=0;j<n;j++)
					{
						temp_row[i] +=  temp[rank][j]*mat[j][i];
					}
				}
			}		
		}
		
		// rank=0 will receive all the calculated data from all other ranks
		if(rank==0)
		{
			for(int i=1;i<np;i++)
			{
				MPI_Recv(temp[i], n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			for(int i=0;i<n;i++)
			{
				temp[0][i]=temp_row[i];
			}
		}
		else{
			
			// all other ranks will send the data to rank=0
			MPI_Send(temp_row, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}

		// rank 0 will broadcast the data to all ranks so that all the other ranks will have updated data.
		MPI_Bcast(temp, n*n, MPI_INT, 0, MPI_COMM_WORLD);
			
		
		if(rank==0)
		{
			printf("A^%d:\n",size+2);
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
				{
					printf("%d " ,temp[i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}
		 if(temp[source][dest]!=0)
		 	len++;
		 else
		 	break;
	}	
	
	if(rank==0)
	{
		if(len==n)
		{
			printf("No path between the source and destination");
		}
		else
			printf("Length of the shortest path between %d and %d is: %d\n", source, dest,len);
	}
	
	MPI_Finalize();
	
	return 0;
}
