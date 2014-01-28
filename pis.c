#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DEF_N 200

int main(int argc, char **argv) {
  double N = DEF_N;
  
  if (argc > 1)
    N = atoi(argv[1]);
  
  int i, count = 0;
  for (i = 0; i < N; i++) {
    double x = rand();
    x = x / RAND_MAX;
    
    double y = rand();
    y = y / RAND_MAX;
    
    double dx = x - 0.5;
    double dy = y - 0.5;
    
    double delta = dx * dx + dy * dy;
    double r = sqrt(delta);
    
    if (r <= 0.5)
      count = count + 1;
  }
  
  double PI = 4 * count / N;
  
  printf("PI: %.10f\n", PI);
}