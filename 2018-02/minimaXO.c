// 2018-02-15, a "buggy" first attempt at implementing a Tic Tac Toe game
// check www.wikipedia.org/wiki/Minimax

#include <stdio.h>
char board[] =
    "012"
    "345"
    "678";

int wins[8][3] = {
  {0, 1, 2},
  {3, 4, 5},
  {6, 7, 8},
  {0, 3, 6},
  {1, 4, 7},
  {2, 5, 8},
  {0, 4, 8},
  {2, 4, 6}
};

int hasWon(char c){
  int i, x, y, z;
  for (i = 0; i < 8; i++){
    x = wins[i][0];
    y = wins[i][1];
    z = wins[i][2];
    if (board[x] == c && board[y] == c && board[z] == c) return 1;
  }
  return 0;
}

int scoreBoard(){
  if (hasWon('X')) return -10;
  if (hasWon('O')) return 10;
  return 0;
}

char nextTurn(char turn){
  return turn == 'X' ? 'O' : 'X';
}

char toChar(int i){
  if (i == 0) return '0';
  if (i == 1) return '1';
  if (i == 2) return '2';
  if (i == 3) return '3';
  if (i == 4) return '4';
  if (i == 5) return '5';
  if (i == 6) return '6';
  if (i == 7) return '7';
  if (i == 8) return '8';
}

void drawBoard(){
  int i;
  printf("-------\n");  
  for (i = 0; i < 9; i++){
    printf(" %c", board[i]);
    if (i == 2 || i == 5 || i == 8) printf("\n\n");
  }
  printf("\n");
}

int countEmptyCells() {
    int i, result = 0;
    for (i = 0; i < 9; i++){
        if (board[i] != 'X' && board[i] != 'O') result++;
    }
    return result;
}

int isValidMove(int m){
  if (m < 0 || m > 8) return 0;
  if (board[m] == 'X' || board[m] == 'O') return 0;
  return 1;
}

int minimax(char turn, int depth) {
  int move, bestMove, score, bestScore;
  
  if (countEmptyCells() == 0 || scoreBoard() != 0) {
    if (scoreBoard() >= 0) {
      return scoreBoard() - depth;
    } else {
      // this shit causes me a headache: "2*depth" kinda solves a problem that I still don't get
      return scoreBoard() + 2*depth;
    }
  }
  
  bestScore = turn == 'O' ? -100 : 100;
  
  for (move = 0; move < 9; move++) {
    if (!isValidMove(move)) continue;
    
    board[move] = turn; // set  
    score = minimax(nextTurn(turn), depth+1);
    
    // maximizing O || minimizing X 
    if ((turn == 'O' && score > bestScore) || (turn =='X' && score < bestScore)){
      bestScore = score;
      bestMove = move;    
    }
    
    board[move] = toChar(move); // unset
  }
  if (depth == 0) return bestMove; else return bestScore;
}

int moveO() {
  int m = minimax('O', 0);
  printf("O: %d\n", m);
  return m;
}

int moveX(){
  int m;
  do {
    printf("X: "); scanf("%d", &m);
  } while (!isValidMove(m));
  return m;
}

int main() {
  char turn = 'X';
  printf("\n\tMINIMAXO - an XO game, USER-COMPUTER (minimax algorithm)\n\n");
  drawBoard();
  
  // while there are empty cells and nobody has won 
  while (scoreBoard() == 0 && countEmptyCells() != 0) {
    if (turn == 'X'){
      board[moveX()] = 'X';
    } else {
      board[moveO()] = 'O';
    }
    turn = nextTurn(turn); // switch player
    drawBoard();
  }
  
  printf("\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
  switch (scoreBoard()) { // from the point of view of 'O'
    case -10: printf("YOU WIN!!"); break;
    case 10: printf("YOU LOSE"); break;
    case 0: printf("DRAW"); break;
  }
  printf("\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
  
  return 0;
}
