#include <stdio.h>

const float penny = 0.01;

float add_pennies(int n) {
  if (n == 0) {
    return 0;
  } else {
    return penny + add_pennies(n-1);
  }
}

int main(void) {
  float dollar = add_pennies(100);
  printf("the value of one dollar is: %f\n", dollar);
  return 0;
}
