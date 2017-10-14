#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);


#ifdef __cplusplus
}
#endif

using namespace std;
int compare(int a0, int a1);
void merge(int *arr, int l, int m, int r);
void mergesort(int *arr,int n);

int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }

  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  long size = atoi(argv[1]);
  int * arr = new int [size];
  int *temp = new int [size];
  int nbthreads = atoi(argv[2]);
  generateMergeSortData (arr, size);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  omp_set_num_threads(nbthreads);
  mergesort(arr,size);
  checkMergeSortResult (arr, size);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  delete[] arr;

  return 0;
}
int compare(int a0, int a1)
{
	return a0 < a1 ? a0 : a1;
}

void merge(int *arr, int l, int m, int r)
{
  int n1 = m - l + 1, n2 = r - m;

	// Creating left and right arrays
	int L[n1], R[n2];
	for (int i = 0; i < n1; i++)
	{
		L[i] = arr[l + i]; // from l to l+n1-1
	}
	for (int j = 0; j < n2; j++)
	{
		R[j] = arr[m + 1+ j]; // from m+1 to m+n2
	}


	int i = 0, j = 0, k = l;

	// Merging 2 arrays of length n1 & n2
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k++] = L[i++];
		}
		else
		{
			arr[k++] = R[j++];
		}
	}

	if(j == n2) // in case R finishes first, leftover elements of L will get inserted as it is
	{
		while (i < n1)
		{
			arr[k++] = L[i++];
		}
	}
	else if(i == n1) // in case L finishes first, leftover elements of R will get inserted as it is
	{
		while (j < n2)
		{
			arr[k++] = R[j++];
		}
	}
	return;

}
void mergesort(int *arr,int n)
{
  for (int len=1; len<=n-1; len = 2*len)
	{
        #pragma omp parallel for schedule(dynamic)
        for (int ls=0; ls<n-1; ls += 2*len)
        {
          int mid = compare(ls+len-1, n-1);
          int right = compare(ls+(2*len)-1, n-1);
          merge(arr, ls, mid, right);
        }
    }
	return;
}
