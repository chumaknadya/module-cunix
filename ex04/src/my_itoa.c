#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char* my_itoa(int nmb) {
  char *buffer = malloc(16 * sizeof(char));
  char *ptr = buffer;
  int i = 1;
  int n;

  if (nmb < 0) {
    nmb = -1 * nmb;
    *buffer = '-';
    buffer++;
  }

  n = nmb;

  while (n > 9) {
    n /= 10;
    i *= 10;
  }

  while (i > 0) {
    n = nmb / i;
    *buffer = '0' + n;
    buffer++;
    nmb -= i * n;
    i /= 10;
  }
  *buffer = '\0';

  return ptr;
}
