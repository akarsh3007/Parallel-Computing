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

// global variables
int function_id, counter = 0, granularity, nbthreads;
float a, b, intensity;
unsigned long n;
string sync;
double multiplier_temp;
// mutex initializer
pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t getnext_lock = PTHREAD_MUTEX_INITIALIZER;

// methods
bool done();
void getnext(int *start, int *end);
void* num_integrate_dynamic_iterationlevel (void *arg);
void* num_integrate_dynamic_chunklevel(void *arg);
void* num_integrate_dynamic_threadlevel(void *arg);

int main (int argc, char* argv[]) {

  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }

   double sum = 0;
   // start time
   std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   function_id = stoi(argv[1]);
   a = stof(argv[2]);
   b = stof(argv[3]);
   n = stol(argv[4]);
   intensity = stof(argv[5]);
   nbthreads = stoi(argv[6]);
   sync = argv[7];
   granularity = stoi(argv[8]);
   pthread_t th[nbthreads];
   multiplier_temp = (b-a)/n;

      for (int i = 0; i < nbthreads; ++i)
      {
        if (sync == "iteration")
        {
          pthread_create(&th[i], NULL, num_integrate_dynamic_iterationlevel, &sum);
        }
        else  if (sync == "chunk")
          {
            pthread_create(&th[i], NULL, num_integrate_dynamic_chunklevel, &sum);
          }
        else  if (sync == "thread")
          {
              pthread_create(&th[i], NULL, num_integrate_dynamic_threadlevel, &sum);
          }
        else
        {
           std::cerr << "Sync method provided is not correct. Choose from iteration, chunk or thread " << "\n";
        }

    }
    for (int i = 0; i < nbthreads; ++i)
    {
        pthread_join(th[i], NULL);
    }

  std::cout << sum << "\n";
  pthread_mutex_destroy(&getnext_lock);
  pthread_mutex_destroy(&sum_lock);
  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  // logging time
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}

bool done()
{
	if(counter == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void getnext(int *start, int *end)
{
  /*locking the mutex to find begin and end*/
  pthread_mutex_lock (&sum_lock);
  if(!done())
  {
    *start=counter;
    counter+=granularity;
    if(counter<n)
    {
      *end=counter-1;
    }
    else
    {
      *end=n-1;
      counter=-1;
    }
  }
  else{
    *start=0;
    *end=-1;
  }
  pthread_mutex_unlock(&sum_lock);
}

void* num_integrate_dynamic_iterationlevel (void *arg)
{
  while(!done())
	{
		int start, end;
		getnext(&start, &end);
		double *sum = (double *) arg;
        double x = 0.0;
		for(int i = start; i<= end; i++)
		{
      x = a + ((float)i + 0.5) * multiplier_temp;
			pthread_mutex_lock (&sum_lock);
      switch (function_id)
      {
        case 1:
           *sum = *sum + f1(x, intensity)* multiplier_temp;
          break;
        case 2:
           *sum = *sum + f2(x, intensity)* multiplier_temp;
         break;
       case 3:
           *sum = *sum + f3(x, intensity)* multiplier_temp;
         break;
       case 4:
           *sum = *sum + f4(x, intensity)* multiplier_temp;
         break;
        default:
            std::cerr << "Invalid functionid" << "\n";
          break;
      }
			pthread_mutex_unlock (&sum_lock);
		}
	}
}

void* num_integrate_dynamic_chunklevel(void *arg)
{
  while(!done())
	{
		int start, end;
    double x = 0.0;
		getnext(&start, &end);
		double *sum = (double *) arg;
    double local_sum = 0.0;
		for(int i = start; i<= end; i++)
		{
      x = a + ((float)i + 0.5) * multiplier_temp;
      switch (function_id)
      {
        case 1:
           local_sum = local_sum + f1(x, intensity)* multiplier_temp;
          break;
        case 2:
           local_sum = local_sum + f2(x, intensity)* multiplier_temp;
         break;
       case 3:
           local_sum = local_sum + f3(x, intensity)* multiplier_temp;
         break;
       case 4:
           local_sum = local_sum + f4(x, intensity)* multiplier_temp;
         break;
        default:
            std::cerr << "Invalid functionid" << "\n";
          break;
      }
		}
    pthread_mutex_lock (&sum_lock);
    *sum += local_sum;
    pthread_mutex_unlock (&sum_lock);
	}
}

void* num_integrate_dynamic_threadlevel(void *arg)
{
  double local_sum = 0.0;
  double x = 0.0;
  double *sum = (double *) arg;
	while(!done())
	{
		int start, end;
		getnext(&start, &end);
    for(int i = start; i<= end; i++)
		{
      x = a + ((float)i + 0.5) * multiplier_temp;
      switch (function_id)
      {
        case 1:
           local_sum = local_sum + f1(x, intensity)* multiplier_temp;
          break;
        case 2:
           local_sum = local_sum + f2(x, intensity)* multiplier_temp;
         break;
       case 3:
           local_sum = local_sum + f3(x, intensity)* multiplier_temp;
         break;
       case 4:
           local_sum = local_sum + f4(x, intensity)* multiplier_temp;
         break;
        default:
            std::cerr << "Invalid functionid" << "\n";
          break;
      }
		}
  }
  pthread_mutex_lock (&sum_lock);
  *sum = *sum + local_sum;
  pthread_mutex_unlock (&sum_lock);
}
