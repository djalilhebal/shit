#include <stdio.h> // printf()

// no need for <string.h> and its strcmp()
int sameString(char *a, char *b){
  while(*a == *b && *a != '\0'){
    a++;
    b++;
  }
  return *a == '\0' && *b == '\0';
}

void main(int argc, char* argv[]){
  char key[] = "yo";
  if( argc > 1 && sameString(key, argv[1]) ) printf("hey"); else printf("err");
}
