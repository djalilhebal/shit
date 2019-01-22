#include <stdio.h>
void main() { /*
write an algorithm that counts the number of increasing monotonic sequences
in a negative-number-terminated series of positive numbers. EXAMPLE:
[1,3,3],[0,15,44],[2],[1,9],-1 (there are 4 inc-mono-seqs shown in brackets) */

  int n, x, y;
  n = 0;                 // the number of monotonic sequences
  scanf("%i",&x);       // get the first numbo
  
  while( x >= 0 ){
    n = n + 1;
    do {
      y = x; // save the current number
      scanf("%i",&x); // get the next num
    } while(y <= x); // jusqu'à (x > y)
  }
  printf("n = %i", n); // Done \(^o^)/ 
}
