#include <stdio.h>
#include <assert.h>

int main(void){
  int i = 2;
  assert(i == 2);
  printf("Assert Passed\n");
  assert(i == 3);
  return 0;
}
