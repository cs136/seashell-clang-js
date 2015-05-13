extern int _suspend();
extern void _exit(int);

int main() {
  int a = 0, b = 2, c = 3;
  a = b + _suspend() + _suspend() + c;
  return a;
}

void _start() {
  _exit(main());
}
