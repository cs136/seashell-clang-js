#include <stdio.h>

int main(void){
  for(int i = 0; i < 10; i++){
    if(i == 9){
      printf("%d ", i);
    }
  }
  int q = 0;
  int l = 0;
  while(1){
    q++;
    if(q <= 20){
      continue;
    }
    else {
       break; 
    }
    l++;
  }
  printf("%d %d ", q, l);
  
  do{
     q--; 
  }while (q > 0);
  
  printf("%d\n", q);
  return 0;
}
