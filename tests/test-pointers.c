#include <stdio.h>

int main(void){
  int i = 20;
  int *q = &i;
  int **m = &q;
  int *l = q;
  //Going to try and print out pointers
  //printf("qp: %p\n", q);
  printf("qv: %d\n", *q);
  printf("mv: %d\n", **m);
  printf("%d\n", (&i == q) && (q == l));
  return 0;
}
