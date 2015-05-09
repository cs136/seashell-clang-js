extern int _suspend();

int main() {
  int a = 0, b = 2, c = 3;
  a = b + _suspend() + _suspend() + c;
  return a;
}
