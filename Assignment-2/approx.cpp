#include <cstdlib>
#include <cmath>

int main (int argc, char* argv[]) {
  float a = atof(argv[1]); //atof is an unsafe function
  float b = atof(argv[2]); //atof is an unsafe function

  return (fabs(a-b) > .1);
}
