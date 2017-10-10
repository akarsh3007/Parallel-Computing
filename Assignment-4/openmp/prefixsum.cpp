#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif

  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
  
  
#ifdef __cplusplus
}
#endif

using namespace std;

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

  int * arr = new int [atoi(argv[1])];

  generatePrefixSumData (arr, atoi(argv[1]));
  
  int n = stoi(argv[1]);
  int num_proc = stoi(argv[2]);
  int policy_n = 10;

	int* pr =new int[n+1];
  omp_set_num_threads(num_proc);

  for(int i=0;i<n;i++){
    cout<<arr[i]<<" ";
  }
  cout<<endl;

  #pragma omp for schedule(dynamic, policy_n)
  for (int i = 1;i < n; i++)
  {
    arr[i] += arr[i-1];
  }                

  for (int i = policy_n; i < n; i=i+policy_n)
  {
    #pragma omp for schedule(static)
    for (int j = i;j < i+policy_n; j++)
    {
      arr[j] += arr[i-1];
    }
  }

  pr[0]=0;

  for(int i=0;i<n;i++){
    pr[i+1] = arr[i];
  }

  for(int i=0;i<=n;i++){
    cout<<pr[i]<<" ";
  }
  cout<<endl;


  checkPrefixSumResult(pr, atoi(argv[1]));
  
  delete[] arr;
  delete[] pr;

  return 0;
}
