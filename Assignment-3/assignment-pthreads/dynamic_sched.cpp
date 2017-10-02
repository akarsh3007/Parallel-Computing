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


int function_id, counter = 0, granularity, nbthreads;
float a, b, intensity;
unsigned long n;
string sync;
double multiplier_temp;
double sum = 0;

pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t getnext_lock = PTHREAD_MUTEX_INITIALIZER;

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

   if (sync == "iteration")
   {
      for (int i = 0; i < nbthreads; ++i)
      {
        pthread_create(&th[i], NULL, num_integrate_dynamic_iterationlevel, &sum);
      }
   }
   else if (sync == "chunk")
   {
     for (int i = 0; i < nbthreads; ++i)
     {
       pthread_create(&th[i], NULL, num_integrate_dynamic_chunklevel, &sum);
     }
   }
   else if (sync == "thread")
   {
     for (int i = 0; i < nbthreads; ++i)
     {
       pthread_create(&th[i], NULL, num_integrate_dynamic_threadlevel, &sum);
     }
   }
   else
   {
      std::cerr << "Sync method provided is not correct. Choose from iteration, chunk or thread " << "\n";
   }

    for (int i = 0; i < nbthreads; ++i)
    {
        pthread_join(th[i], NULL);
    }

  std::cout << sum << "\n";
  pthread_mutex_destroy(&getnext_lock);
  pthread_mutex_destroy(&sum_lock);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}

bool done()
{
	if(counter == -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void getnext(int *start, int *end)
{
	pthread_mutex_lock (&getnext_lock);
	*start = counter;
	if(counter + granularity < n)
	{
		*end = counter + granularity-1;
		counter += granularity;
	}
	else
	{
		*end = n - 1;
		counter = -1;
	}
	pthread_mutex_unlock (&getnext_lock);
	return;
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
      if (function_id == 1)
       {
        *sum = *sum + f1(x, intensity)* multiplier_temp;
       }
       else if (function_id == 2)
       {
        *sum = *sum + f2(x, intensity)* multiplier_temp;
       }
       else if (function_id == 3)
       {
         *sum = *sum + f3(x,intensity)* multiplier_temp;
       }
       else if (function_id == 4)
       {
         *sum = *sum + f4(x, intensity)* multiplier_temp;
       }
       else
       {
         std::cerr << "Invalid functionid" << "\n";
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
		for(int i = start; i<= end; i++)
		{
      x = a + ((float)i + 0.5) * multiplier_temp;
      if (function_id == 1)
       {
         pthread_mutex_lock (&sum_lock);
        *sum = *sum + f1(x, intensity)* multiplier_temp;
         pthread_mutex_unlock (&sum_lock);
       }
       else if (function_id == 2)
       {
         pthread_mutex_lock (&sum_lock);
        *sum = *sum + f2(x, intensity)* multiplier_temp;
         pthread_mutex_unlock (&sum_lock);
       }
       else if (function_id == 3)
       {
         pthread_mutex_lock (&sum_lock);
        *sum = *sum + f3(x, intensity)* multiplier_temp;
         pthread_mutex_unlock (&sum_lock);
       }
       else if (function_id == 4)
       {
         pthread_mutex_lock (&sum_lock);
        *sum = *sum + f4(x, intensity)* multiplier_temp;
         pthread_mutex_unlock (&sum_lock);
       }
       else
       {
         std::cerr << "Invalid functionid" << "\n";
       }
		}
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
			pthread_mutex_lock (&sum_lock);
      if (function_id == 1)
       {
        *sum = *sum + f1(x, intensity)* multiplier_temp;
       }
       else if (function_id == 2)
       {
        *sum = *sum + f2(x, intensity)* multiplier_temp;
       }
       else if (function_id == 3)
       {
         *sum = *sum + f3(x,intensity)* multiplier_temp;
       }
       else if (function_id == 4)
       {
         *sum = *sum + f4(x, intensity)* multiplier_temp;
       }
       else
       {
         std::cerr << "Invalid functionid" << "\n";
       }
			pthread_mutex_unlock (&sum_lock);
		}
	pthread_mutex_lock (&sum_lock);
	*sum += local_sum;
	pthread_mutex_unlock (&sum_lock);
}
}
