#include <stdio.h>
#include <math.h>
int main(){
	/** isKaprekarNumber?
	 * 
	 * x = 45
	 * square = 2025
	 * numberOfDigits = 4
	 * divisor = 100
	 * y = 20 + 25 = 45 = x
	 * 
	 * x = 297
	 * square = 88209
	 * numberOfDigits = 5
	 * divisor = 1000
	 * y = 88 + 209 = 297 = x
	 */
	int x, y;
	int square, numberOfDigits, divisor;
	printf("Enter a number: "); scanf("%d", &x);
	// counting the number of digits in the square
	numberOfDigits = 0;
	square = x*x;
	while (square != 0) {
		numberOfDigits++;
		square = square/10;
    }
    
	square = x*x; // recalculate it
	divisor = pow(10, (numberOfDigits/2 + numberOfDigits%2) );
	y = square/divisor + square%divisor;
    printf("isKaprekarNumber(%d) ? %c", x, x == y ? 'y' : 'n');
	return 0;
}
