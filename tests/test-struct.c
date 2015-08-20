#include <stdio.h>

struct posn {
  int x;
  int y;
};

int main(void){
  struct posn hi = {5,6};
  printf("%d%d\n", hi.x, hi.y);
  return 0;
}

