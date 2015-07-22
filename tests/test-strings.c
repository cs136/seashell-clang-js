#include <stdio.h>
#include <string.h>

int main(void){
  char* mystr = "Hello";
  char mystr2[6];
  strcpy(mystr2, mystr);
  printf("%d", strcmp(mystr, mystr2));
  char mystr3[] = "World";
  printf("%d", 0 > strcmp(mystr, mystr3));
  printf("%s", mystr);
  int i = strlen(mystr);
  printf("%d\n", i);
  return 0;
}
