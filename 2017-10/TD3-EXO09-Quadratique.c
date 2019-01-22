#include <stdio.h>
#include <math.h> // sqrt

void main(){ /* TD3-EXO09 Équation Quadratique

Résoudre une équation du second degré
(avec tous les cas possibles réels et complexes)
*/

  float a,b,c,d,x,y;
  
  printf("Entrer a b c\n");
  scanf("%f",&a);
  scanf("%f",&b);
  scanf("%f",&c);
  
  if(a == 0){
    printf("ERROR: a = 0 !");
  } else {
    
    d = b*b - 4*a*c;
    x = -b / (2*a);
    y = sqrt( sqrt(d*d) ) / (2*a); // sqrt(d*d) = abs(d) thou (d<0?-d:d) is better
    
    if(d == 0){ printf("Seul solution R: %f", x); }
  
    if(d > 0){  printf("Deux solutions R: %f et %f", x+y, x-y); }
  
    if(d < 0){  printf("Deux solutions C: %f + %fi et %f - %fi", x, y, x, y); }
  
  }
}// FinMain :p
