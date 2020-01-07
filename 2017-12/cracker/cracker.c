#include <stdio.h> // printf() sprintf() fgets() _popen() FILE
#include <string.h> // strcmp()
int main(){
  /**
   * My first attempt at making a "brute-force cracker"
   * BUG: my.. thing.. fails if the "password" is longer than 2 characters
   */
   
  const char letters[] = ".abcdefghijklmnopqrstuvwxyz!";
  const int BASE = 28;
  char cmd[100], word[10], out[4];
  int tmp, x, i;
  FILE *pipe;
  
  x = 0;
  
  printf("Guessing...\n");
  while(1){
  
    // generate a word, as if counting in base BASE
    tmp = x++;
    i = 0;
    do {
      word[i] = letters[tmp%BASE];
      tmp /= BASE;
      ++i;
    } while(tmp != 0);
    word[i] = '\0';

    printf("%s\n", word);
    
    // cmd = "crackme.exe " + word
    sprintf(cmd, "crackme.exe %s", word);
    
    // out = the output of executing cmd ("err" or "hey")
    pipe = _popen(cmd, "rt");
    fgets(out, 4, pipe);

    // if(out == "hey")
    if(strcmp(out, "hey") == 0){
      printf("FOUND IT!!\n\n<< %s >>", word);
      break;
    }

  } // WHILE

  scanf("%c", out); // "pause"
  return 0;
}
