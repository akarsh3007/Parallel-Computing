#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <string>
#include <pthread.h>

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

// global sum
float sum = 0.0 ;
// initializer mutex
pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;

// data structure to pass to thread
struct NumIntegrate
{
  int function_id ;
  float a ;
  float temp;
  unsigned long  n ;
  float intensity ;
  int start;
  int length;

  NumIntegrate(int function_id, float a, float temp, unsigned long n,
                float intensity, int start, int length)
                {
                    this->function_id = function_id;
                    this->a = a;
                    this->temp = temp;
                    this->n = n;
                    this->intensity = intensity;
                    this->start = start;
                    this->length = length;
                }

};


void* num_integrate_static_iterationlevel (void *arg);
void* num_integrate_static_threadlevel(void *arg);

int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
    return -1;
  }
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  int function_id = stoi(argv[1]);
  float a = stof(argv[2]);
  float b = stof(argv[3]);
  unsigned long n = stol(argv[4]);
  float intensity = stof(argv[5]);
  int nbthreads = stoi(argv[6]);
  string sync = argv[7];

  float temp = (b-a)/n;
  int length = n/nbthreads;
  pthread_t th[nbthreads];
  int th_start = 0;

    for (int i = 0; i < nbthreads; ++i)
    {
      NumIntegrate *nums = new NumIntegrate(function_id,a,temp,n,intensity,th_start,length);
      if (sync == "iteration")
      {
        pthread_create(&th[i], NULL, num_integrate_static_iterationlevel, nums);
        th_start = th_start + length;
      }
      if (sync == "thread")
      {
        pthread_create(&th[i], NULL, num_integrate_static_threadlevel, nums);
        th_start = th_start + length;
      }
    }

  for (int i = 0; i < nbthreads; ++i)
  {
    pthread_join(th[i], NULL);
  }

  std::cout << sum << "\n";
  pthread_mutex_destroy(&sum_lock);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}

  void* num_integrate_static_iterationlevel (void *arg)
  {

   NumIntegrate* args = (NumIntegrate*) arg;
   int i;
   float x = 0.0;

   int end = args->start +  args->length;

   for (i=args->start; i<end; i++)
   {
     x = args->a + ((float)i + 0.5) * args->temp;

     pthread_mutex_lock(&sum_lock);
     switch (args->function_id)
     {
       case 1:
          sum = sum + f1(x, args->intensity)*args->temp;
         break;
       case 2:
          sum = sum + f2(x, args->intensity)*args->temp;
        break;
      case 3:
          sum = sum + f3(x, args->intensity)*args->temp;
        break;
      case 4:
          sum = sum + f4(x, args->intensity)*args->temp;
        break;
       default:
           std::cerr << "Invalid functionid" << "\n";
         break;
     }
    pthread_mutex_unlock(&sum_lock);
   }
  }

  void* num_integrate_static_threadlevel (void *arg)
  {

   NumIntegrate* args = (NumIntegrate*) arg;
   int i;
   float x = 0.0;
   float local_sum = 0.0;
   int end = args->start +  args->length;

   for (i=args->start; i<end; i++)
   {
     x = args->a + ((float)i + 0.5) * args->temp;

     switch (args->function_id)
     {
         case 1:
            local_sum = local_sum + f1(x, args->intensity)*args->temp;
           break;
         case 2:
            local_sum = local_sum + f2(x, args->intensity)*args->temp;
          break;
        case 3:
            local_sum = local_sum + f3(x, args->intensity)*args->temp;
          break;
        case 4:
            local_sum = local_sum + f4(x, args->intensity)*args->temp;
          break;
         default:
             std::cerr << "Invalid functionid" << "\n";
           break;
      }
   }

   pthread_mutex_lock(&sum_lock);
   sum = sum + local_sum;
   pthread_mutex_unlock(&sum_lock);
  }
