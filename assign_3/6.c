#include <stdio.h>
#include <omp.h>
int main()
{
	int i = 0; // a shared variable
	int n;
	scanf("%d",&n);
	int arr[n];

	for(int i=0;i<n;i++)
	{
		arr[i]=i;
	}
	int sum=0;
	{
	#pragma omp for
		for (i = 0; i < n; i++) {
	      sum+=arr[i];
	    }
	}
	printf("sum %d\n", sum);
}
