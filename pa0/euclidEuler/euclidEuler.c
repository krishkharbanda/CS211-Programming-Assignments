#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int isPerfectNumber(long int x) {
  long int sum = 1;
  for (int i = 2; i <= sqrt(x); i++) {
    if (x % i == 0) {
      sum += i;
      sum += x / i;
    }
  }
  return (sum == x);
}

int main(int argc, char* argv[]) {

  // Open the filename given as the first command line argument for reading
  FILE* fp = fopen(argv[1], "r");
  if (!fp) {
    perror("fopen failed");
    return EXIT_FAILURE;
  }

  char buf[256];

  char* string = fgets(buf, 256, fp);
  char* ptr;
  long int x = strtol(string, &ptr, 10);
  /* ... */

  if (isPerfectNumber(x)) {
    long double val = (1+sqrt(1+8*x))/2;
    long double num = log(val);
    long double p = num/log(2);
    long int m_p = pow(2, p) - 1;
    // Printing in C.
    // %d is the format specifier for integer numbers.
    // \n is the newline character
    printf( "%ld \n", m_p );
  } else {
    printf("-1\n");
  }
}