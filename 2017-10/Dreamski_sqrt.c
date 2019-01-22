#include <stdio.h>

float Dreamski_sqrt(float X){
  int i=0;
  float x=1, y, z, a=10;

  if (X < 0) return -1; // Like, an error

  while(x*x != X && a > 0.00000001){
    y = x*x > X ? x-a : x+a;
    z = y*y > X ? y-a : y+a;
    if( x == z ) a = a/10;
    x = x*x > X ? x-a : x+a;
    i=i+1;
  }
  printf("#%i " ,i);

  return x;
}

int main() {
  float num;
  printf("Find the square root of: \n"); scanf("%f", &num);  
  printf("%G is the sqrt of %G\n", Dreamski_sqrt(num), num);
  return 0;
}
