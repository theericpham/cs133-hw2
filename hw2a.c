#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

// helper tests the primality of a number p
int is_prime(int p) {
  int i;
  
  if (p < 2)  return 0;
  i = 2;
  while (i*i <= p) {
    if (p % i == 0) return 0;
    i++;
  }
  return 1;
}

int main(int argc, char** argv) {

  int N = 0;
  int T = 1;
  
  if (argc > 1)
    N = atoi(argv[1]);
  
  if (argc > 2) {
     T = atoi(argv[2]);
     omp_set_num_threads(T);
  }
  
  int i = 0;
  int *primes = (int *)malloc(sizeof(int)*N);
  
#pragma omp parallel for
  for (i=0; i<N; i++)
    primes[i] = is_prime(i+1);
  
  if (N>1) printf("2");
  for (i=2; i<N;i++)
    if (primes[i] == 1) printf(", %i", i+1);
  
  printf("\n");
}
