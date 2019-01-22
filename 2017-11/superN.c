#include <stdio.h>
#include <stdlib.h>// system()

char map[24][77+1] = {
"#############################################################################",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                ##########                                 #",
"#                 ##             ##                                         #",
"#                 #################                                         #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                                                           #",
"#                                       ###########          ################",
"#                                       ##        #                         #",
"#              ###                  ######        #      ######             #",
"#     n     #########            k                #                         #",
"#############################################################################",
"#############################################################################"};

int x, y; // the coordinates of 'n'

// this function is used to keep the "main" function simple
void moveN(int _x, int _y){
  if(map[x + _x][y + _y] != '#'){
    map[x][y] = ' ';
    x = x + _x;
    y = y + _y;
    map[x][y] = 'n';
  }
}

int main(){
  char key;
  int i, j; // for working with loops
  int kx, ky, kStep = 1; // 'k' cordinates (x,y) and step

  for(i=0; i<24; i++){// for each line in the map...
    printf("%s\n", map[i]);// draw that line.
    for(j=0; j<77; j++){// for each character in that line...
      if(map[i][j] == 'n') {x=i; y=j;} // save the cordinates of 'n'
      if(map[i][j] == 'k') {kx=i; ky=j;} // save the cordinates of 'k'
    }
  }

  while(1 == 1){ // THE GAME LOOP \(._.)/
    
    // MOVE NUTELLA
    scanf(" %c", &key);
    if(key == 'D' || key == 'd') moveN(0, 1); // right
    if(key == 'Q' || key == 'q') moveN(0, -1); // left
    // if the "Z" key is pressed and we are on the ground, jump
    if((key == 'Z' || key == 'z') && map[x+1][y] == '#') moveN(-4, 0);
    moveN(1, 0); // gravity *~*
    // Did 'n' meet 'k'?
    while(y == ky && x == kx) printf("\nn'lovi'k...");


    // MOVE KAITO
    // if 'k' is going to hit a block or fall, alter its direction
    if(map[kx][ky+kStep] == '#' || map[kx+1][ky+kStep] != '#') kStep = -kStep;
    map[kx][ky] = ' ';
    ky = ky + kStep;
    map[kx][ky] = 'k';
    // Did 'k' meet 'n'?
    while(y == ky && x == kx) printf("\nn'lovi'k...");


    // DRAWING
    // pause; clear the screen; and then re-draw the map, line by line
    for(i=0; i/3<=666; i++) for(j=0; j/3 <= 666; j++);
    system("cls");
    for(i=0; i<24; i++) printf("%s\n", map[i]);
    
  }
  
  return 0;
}
