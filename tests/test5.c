extern int _seashell_RT_suspend();
extern void _seashell_RT_exit(int);

int main() {
  int a = 0, b = 2, c = 3;
  a = b + _seashell_RT_suspend() + _seashell_RT_suspend() + c;
  return a;
}

void _start() {
  _seashell_RT_exit(main());
}
