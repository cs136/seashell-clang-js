extern void _exit(int);
extern int _read(int fd, void* buf, int size);
extern int _write(int fd, const void* buf, int size);

int main() {
  char buffer [80] = "";
  if (_read(0, buffer, 80) != 13) 
    return 1;
  _write(1, buffer, 13);
  return 0;
}

void _start() {
  _exit(main());
}
