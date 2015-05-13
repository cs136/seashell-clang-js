int main( ) {
  return 5;
}

void _exit(int);
void _start( ) {
  _exit(main());
}
