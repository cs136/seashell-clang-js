#include <stdio.h>

int stuff(int a);

int main(void){
  printf("%d\n", stuff(10));
  return 0;
}

int stuff(int a){
  return a;
}
