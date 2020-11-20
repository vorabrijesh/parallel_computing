/*
How to run:

To compile: g++ -std=c++11 -pthread 1.cc
To run $./a.out 10 0.01
First argument is number of r,c in matrix
Second argument is threshold

*/
// #include <random>

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>

using namespace std::chrono;


const int nthreads = 1; // number of threads


// double mat1[3][3];
int sum[nthreads] = { 0 };

void compute_sum(int n,int i, int j, std::vector<std::vector<double>> &mat, std::vector<std::vector<double>> &temp)
{
	// temp[i][j] = 2*mat[i][j];
	double avg = mat[i][j];
	int count = 1;
	if(i-1>=0 && i-1<n)
	{
		avg+=mat[i-1][j];
		count++;
	}
	if(i+1>=0 && i+1<n)
	{
		avg+=mat[i+1][j];
		count++;	
	}
	if(j-1>=0 && j-1<n)
	{
		avg+=mat[i][j-1];
		count++;
	}
	if(j+1>=0 && j+1<n)
	{
		avg+=mat[i][j+1];
		count++;	
	}
	avg = avg /count;
	temp[i][j] = avg;
	
}

int main(int argc, char ** argv)
{

    int n = atoi(argv[1]);
    double threshold = atof(argv[2]);
    printf("%d %f\n",n,threshold);
    
    std::vector<std::vector<double>> mat;
    std::vector<std::vector<double>> temp;

    // initialize temp amd mat vectors
    std::vector<double> t;
	for(int j=0;j<n;j++)
	{
		t.push_back(0.0);
	}
    for(int i=0;i<n;i++)
    {
    	mat.push_back(t);
    	temp.push_back(t);
    }

    // assign random values.
    double lbound = 0;
    double ubound = 10;
    std::uniform_real_distribution<double> urd(lbound, ubound);
    std::default_random_engine re;
    
    printf("Initial Matrix:\n");
    for (int i = 0; i < n; i++)
    {
    	
        for(int j = 0; j < n; j++)
        {
            mat[i][j] =  urd(re);
            std::cout<<mat[i][j]<<" ";
        }
        printf("\n");
    }
    printf("\n");

    
    double max_diff = -1e9;
    int iter=0;
    auto start = high_resolution_clock::now();
    do
    {
    	max_diff = -1e9;
	    // vector of n*n threads
	    std::vector<std::thread> threads;

	    // calculate 
	    for(int i=0;i<n;i++)
	    {
	    	for(int j=0;j<n;j++)
	    	{
	    		threads.push_back(std::thread(compute_sum, n, i, j,std::ref(mat),std::ref(temp)));
	    	}
	    }
	    //join threads
	    for (auto &th : threads) 
	    {
	    	th.join();
	    }
	    
	    
	    for(int i=0;i<n;i++)
	    {
	    	for(int j=0;j<n;j++)
	    	{
	    		std::cout<<temp[i][j]<<" ";

	    		if(max_diff <  temp[i][j] - mat[i][j])
	    		{
	    			max_diff =  temp[i][j] - mat[i][j];
	    		}
	    		mat[i][j] = temp[i][j];
	    	}
	    	std::cout<<"\n";
	    }
	    std::cout<<"max_diff : "<<max_diff<<"\n";
	    iter++;
	}
	while(max_diff>threshold);

	std::cout<<"\nIteration : "<<iter<<"\n";

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << "duration : "<< duration.count() <<" milliseconds"<< std::endl;
    return 0;
}
