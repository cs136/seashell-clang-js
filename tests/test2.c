int main( ) {
  return 5;
}

void _seashell_RT_exit(int);
void _start( ) {
  _seashell_RT_exit(main());
}
