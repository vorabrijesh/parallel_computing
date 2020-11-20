/*
 * To compile: g++ -std=c++11 -pthread 5.cc
 */


#include <iostream>
#include <thread>
#include <vector>
#define MAX 10000
int nthreads;
int ndata;

int sum[MAX] = { 0 };

void compute_sum(int tid, std::vector<int>& d)
{
	int st = (ndata / nthreads) * tid;
	int en = (ndata / nthreads) * (tid + 1);

	for (int i = st; i < en; i++) {
	sum[tid] += d[i];
	}
}

int main(int argc, char ** argv)
{

	// int ndata, nthreads;
	scanf("%d %d",&ndata,&nthreads); 
	std::vector<std::thread> threads;

	std::vector<int> data;
	for (int i = 0; i < ndata; i++) {
		data.push_back(i);
	}

	for (int i = 0; i < nthreads; i++) {
		threads.push_back(std::thread(compute_sum, i, std::ref(data)));
	}

	for (auto &th : threads) {
		th.join();
	}

	int tsum = 0;
	for (int i = 0; i < nthreads; i++) {
		tsum += sum[i];
	}

	std::cout << "Array sum is: " << tsum << std::endl;

	return 0;
}