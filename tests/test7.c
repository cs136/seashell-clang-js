extern void _seashell_RT_exit(int);
extern int _seashell_RT_read(int fd, void* buf, int size);
extern int _seashell_RT_write(int fd, const void* buf, int size);

int main() {
  char buffer [80] = "Hello World!\n";
  _seashell_RT_write(1, buffer, 13);
  return 0;
}

void _start() {
  _seashell_RT_exit(main());
}
