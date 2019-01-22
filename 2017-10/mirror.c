#include <stdio.h>
void main(){// Mirror "12345" -> "54321"
  int x;
  scanf("%i",&x);
  while(x!=0) {
    printf("%i", x%10);
    x = (x - x%10) / 10;
  }

}
