#include <stdio.h>
void main(){
  int x, y, z, n;
  
  // make sure the input is valid
  do { scanf("%d", &n); } while(n < 2);

  printf("0, 1");
  x = 0;
  y = 1;

  while(n > 2){
    z = y + x;
    printf(", %d", z);
    x = y;
    y = z;
    n = n - 1;
  }
}
