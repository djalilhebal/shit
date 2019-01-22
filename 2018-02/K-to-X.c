// saw this on YouTube (forgot the title/link but still have the screenshot), I decided to try it
#include <stdio.h>
char map[10][25] = {
  "#########################",
  "# K          #          #",
  "#      #     #          #",
  "# ######     #          #",
  "#      #                #",
  "#      #########      ###",
  "#      #            X   #",
  "#      #     ############",
  "#                       #",
  "#########################"
};

int from(int x, int y) {
  if (!isValidMove(x, y)) return 0;
  if (isOnTarget(x, y)) return 1;
//  map[x][y] = '*'; printMap(); // for debugging ._.
  return from(x+1, y) + from(x, y+1);
}

void printMap(){
  int i, j;
  for (i = 0; i < 10; i++){
    for (j = 0; j < 25; j++){
      printf("%c", map[i][j]);
    }
    printf("\n");
  }
  printf("\n\n\n");
}

int isValidMove(int x, int y){
  return map[x][y] != '#';
}

int isOnTarget(int x, int y){
  return map[x][y] == 'X';
}

int main(){
  // get K(x, y)
  int i, j;
  int x, y;
  for (i = 0; i < 10; i++)
    for (j = 0; j < 25; j++){
      if (map[i][j] == 'K'){
        x = i;
        y = j;
      } 
    }
  
  printMap();
    printf("There exist %d possible moves (RIGHT OR DOWN) from K(%d, %d) to X", from(x, y), x, y);
  return 0;
}
