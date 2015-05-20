#include <stdio.h>
#include <stdint.h>
extern int32_t _seashell_RT_suspend(void);
int main(int argc, char* argv[]) {
  int32_t result = _seashell_RT_suspend();
  printf("Hello World %d!\n", result);
  return result;
}
