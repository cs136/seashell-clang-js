// sum(k) produces the sum of 0...k
// requires: k >= 0
int sum(const int k) {
if (k == 0) {
return 0;
} else {
return k + sum(k-1);
}
}

int main(void){
  printf("%d\n", sum(3));
  return 0;
} 
