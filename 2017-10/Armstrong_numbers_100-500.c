#include <stdio.h>
/*
** Sachant qu'il n'existe que 4 nombres compris entre 100 et 500 tels que la somme des cubes
** des chiffres les composant est égale au nombre lui-même.
** Construire la solution qui permet de retrouver ces 4 nombres.
** exemple: 153 = 1^3+ 5^3 + 3^3 // x=3, y=5, z=1
*/

void main(){
  int i, iii, x, y, z;
  for(i = 100; i < 500; i++) {
    x = i%10;
    y = ((i-i%10)/10)%10;
    z =  ((i-i%100)/100)%10;
    iii = x*x*x + y*y*y + z*z*z;
    if(i == iii) printf("%i\n", i);
    x = (x - x%10) / 10;
  }
     
}
