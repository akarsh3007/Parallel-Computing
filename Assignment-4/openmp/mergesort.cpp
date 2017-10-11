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

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);


#ifdef __cplusplus
}
#endif

using namespace std;
void merge(int *arr, int n, int *temp);
void mergesort(int *arr, int n, int *temp);

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

  int size = atoi(argv[1]);
  int * arr = new int [size];
  int *temp = new int [size];
  int nbthreads = atoi(argv[2]);

  generateMergeSortData (arr, size);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  omp_set_num_threads(nbthreads);

  #pragma omp parallel
    {
        #pragma omp single
        mergesort(arr, atoi(argv[1]), temp);
    }

  checkMergeSortResult (arr, size);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  delete[] arr;

  return 0;
}
void merge(int *arr, int n, int *temp)
{
    int i = 0, j = n/2, k = 0;
    while (i<n/2 && j<n)
    {
        if (arr[i] < arr[j])
        {
           temp[k++] = arr[i++];
        }
        else
        {
           temp[k++] = arr[j++];
        }
    }
    while (i<n/2)
    {
        temp[k++] = arr[i++];
    }

    while (j<n)
    {
        temp[k++] = arr[j++];
    }
    memcpy(arr, temp, n*sizeof(int));
    return;
}
void mergesort(int *arr, int n, int *temp)
{
  if (n < 2)
  return;

   #pragma omp task
   mergesort(arr, n/2, temp);
   #pragma omp taskwait

   #pragma omp task
   mergesort(arr+(n/2), n-(n/2), temp);

   #pragma omp taskwait

   merge(arr, n, temp);
}
