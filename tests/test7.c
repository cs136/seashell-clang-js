extern void _exit(int);
extern int _read(int fd, void* buf, int size);
extern int _write(int fd, const void* buf, int size);

int main() {
  char buffer [80] = "Hello World!\n";
  _write(1, buffer, 13);
  return 0;
}

void _start() {
  _exit(main());
}
