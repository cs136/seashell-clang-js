#include <stdio.h>
#include <stdlib.h>

int main(void){
  int n = 0;
  printf("the size of an int is: %zd\n", sizeof(int));
  printf("the size of n is: %zd\n", sizeof(n));
  char c = 'c';
  printf("the size of a char is: %zd\n", sizeof(char));
  printf("the size of c is: %zd\n", sizeof(c));
  int *pn = &n;
  printf("the size of an int pointer is: %zd\n", sizeof(int *));
  printf("the size of pn is: %zd\n", sizeof(pn));
  char *pc = &c;
  printf("the size of a char pointer is: %zd\n", sizeof(char *));
  printf("the size of pc is: %zd\n", sizeof(pc));
  return 0;
}
