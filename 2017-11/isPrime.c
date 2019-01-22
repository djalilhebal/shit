#include <stdio.h>
void main() { // isPrime?
  
  int max, x, y, p;
  
  scanf("%i", &x);
  if(x < 0) x = -x; // get |x|
  
  if(x != 2 && x % 2 == 0){ // all prime numbers are odd expect 2
    p = 0; // it is NOT prime
  } else {
    p = 1; // MAYBE it's prime
  }

  max = x/2;
  y = 3;// our starting point
  while(p != 0 && y <= max){
    if(x % y == 0) p = 0;
    y = y + 2; // the next ODD number to try
  }

  if(p == 1) printf("prime"); else printf("not prime"); // Done \(^o^)/

}
