#include <stdio.h>
#include <stdlib.h>

int main(void){
  const int len = 5;
  int *arr = malloc(sizeof(int) * len);
  for(int i = 0; i < len; i++){
    arr[i] = i + 1;
  }
  const int newlen = 10;
  arr = realloc(arr, sizeof(int) * newlen);
  for(int i = len - 1; i < newlen; i++){
    arr[i] = newlen - 1;
  }
  for(int i = 0; i < newlen; i++){
    printf("%d", arr[i]);
  }
  printf("\n");
  free(arr);
  return 0;
}
