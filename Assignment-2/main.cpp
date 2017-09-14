#include <iostream>
#include <string>
#include <cmath>
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

int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  int function_id = stoi(argv[1]);
  float a = stof(argv[2]);
  float b = stof(argv[3]);
  float n = stof(argv[4]);
  float intensity = stof(argv[5]);
  float temp = (b-a)/n;
  float res = 0;
  if (function_id == 1)
  {
    for (int i = 0; i < n; ++i)
    {
      res = res + (f1((a+(i+0.5)*temp),intensity)*temp);
    }

  }
  else if (function_id == 2)
  {
    for (int i = 0; i < n; ++i)
    {
      res = res + (f2((a+(i+0.5)*temp),intensity)*temp);
    }
  }
  else if (function_id == 3)
  {
    for (int i = 0; i < n; ++i)
    {
      res = res + (f3((a+(i+0.5)*temp),intensity)*temp);
    }
  }
  else if (function_id == 4)
  {
    for (int i = 0; i < n; ++i)
    {
      res = res + (f4((a+(i+0.5)*temp),intensity)*temp);
    }
  }
  else
  {
    std::cerr << "Invalid functionid" << "\n";
    return -1;
  }

  std::cout << res <<endl;
  return 0;
}
