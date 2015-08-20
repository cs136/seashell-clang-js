#include <stdio.h>

int my_abs(const int n) {
  if (n < 0) {
    return -n;
  } else {
    return n;
  }
}

int main(void){
  printf("%d\n", my_abs(-10));
  return 0;
}
