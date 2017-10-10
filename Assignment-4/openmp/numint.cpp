#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

using namespace std;

float parallel_integrate (int argc, char* argv[]);

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
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }
 
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
  float integrate = parallel_integrate(argc, argv);
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cout<<integrate<<std::endl;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}


float parallel_integrate (int argc, char* argv[]){

  int function = atoi(argv[1]);
  float a = atof(argv[2]); 
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  long intensity = atol(argv[5]); 
  int granularity = atoi(argv[8]);
  int thread_count = atoi(argv[6]);
  string sync = argv[7];
  float sum=0.0;

  float temp,func_param;
  temp = (b-a)/n;


  omp_set_num_threads(thread_count);

  if(granularity==-1){
      if(sync == "static"){
        #pragma omp parallel for schedule(static) reduction(+:sum)
        for(int i=0;i<n;i++){
          func_param = a+((i+0.5)*temp); 
          switch(function){
          case 1:    
            sum += (f1(func_param,intensity)*temp);   
            break; 
          case 2:
            sum += (f2(func_param,intensity)*temp);
            break;  
          case 3:
            sum += (f3(func_param,intensity)*temp);  
            break;   
          default:
            sum += (f4(func_param,intensity)*temp);
          }  
        }
      }else if(sync == "dynamic"){
        #pragma omp parallel for schedule(dynamic) reduction(+:sum)
        for(int i=0;i<n;i++){
          func_param = a+((i+0.5)*temp); 
          switch(function){
          case 1:    
            sum += (f1(func_param,intensity)*temp);   
            break; 
          case 2:
            sum += (f2(func_param,intensity)*temp);
            break;  
          case 3:
            sum += (f3(func_param,intensity)*temp);  
            break;   
          default:
            sum += (f4(func_param,intensity)*temp);
          }  
        }
      }else if(sync == "guided"){
        #pragma omp parallel for schedule(dynamic) reduction(+:sum)
        for(int i=0;i<n;i++){
          func_param = a+((i+0.5)*temp); 
          switch(function){
          case 1:    
            sum += (f1(func_param,intensity)*temp);   
            break; 
          case 2:
            sum += (f2(func_param,intensity)*temp);
            break;  
          case 3:
            sum += (f3(func_param,intensity)*temp);  
            break;   
          default:
            sum += (f4(func_param,intensity)*temp);
          }  
        }
      }else{
        cout<<"incorrect scheduling input"<<endl;
      }
    }else{
      if(sync == "static"){
        #pragma omp parallel for schedule(static, granularity) reduction(+:sum)
        for(int i=0;i<n;i++){
          func_param = a+((i+0.5)*temp); 
          switch(function){
          case 1:    
            sum += (f1(func_param,intensity)*temp);   
            break; 
          case 2:
            sum += (f2(func_param,intensity)*temp);
            break;  
          case 3:
            sum += (f3(func_param,intensity)*temp);  
            break;   
          default:
            sum += (f4(func_param,intensity)*temp);
          }  
        }
      }else if(sync == "dynamic"){
        #pragma omp parallel for schedule(dynamic, granularity) reduction(+:sum)
        for(int i=0;i<n;i++){
          func_param = a+((i+0.5)*temp); 
          switch(function){
          case 1:    
            sum += (f1(func_param,intensity)*temp);   
            break; 
          case 2:
            sum += (f2(func_param,intensity)*temp);
            break;  
          case 3:
            sum += (f3(func_param,intensity)*temp);  
            break;   
          default:
            sum += (f4(func_param,intensity)*temp);
          }  
        }
      }else if(sync == "guided"){
        #pragma omp parallel for schedule(guided,granularity) reduction(+:sum)
        for(int i=0;i<n;i++){
          func_param = a+((i+0.5)*temp); 
          switch(function){
          case 1:    
            sum += (f1(func_param,intensity)*temp);   
            break; 
          case 2:
            sum += (f2(func_param,intensity)*temp);
            break;  
          case 3:
            sum += (f3(func_param,intensity)*temp);  
            break;   
          default:
            sum += (f4(func_param,intensity)*temp);
          }  
        }
      }else{
        cout<<"incorrect scheduling input"<<endl;
      }
    }
    
    return sum;
}