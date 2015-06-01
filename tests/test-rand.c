#include <stdio.h>
#include <stdlib.h>
int main(void){
  srand(1234);
  printf("%d\n", rand() % 10);
  return 0;
}
