#include <stdio.h>

int main() {
  int sum = 0;
  int tmp;

  /** Loop, read from stdin, and sum all the numbers we get. */
  while (scanf("%d", &tmp) == 1) sum += tmp;
  return sum;
}
