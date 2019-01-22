// Carroll's Alphabet -- 2018-05-04 (Friday)
// By Wanis (@blackreapr) and Djalil (@dreamski21)
// This code is "ungood" (memory management and shit) but it's easy to read

#include <stdio.h>
#include <string.h> // strlen()
#include <ctype.h> // isalpha(), toupper()

#define MAX 140 // an arbitrary max length of KEY and MSG
#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // "ABC"[0] == 'A'

// keep only letters (and uppercase them)
void preprocess(char* str, char* out) {
  int i, j = 0, len = strlen(str);
  
  for (i = 0; i < len; i++) {
    if (isalpha(str[i])) {
      out[j++] = toupper(str[i]);
    }
  }
  out[j] = '\0';
}

void encipher(char* key, char* msg, char* out) {
  int key_len = strlen(key);
  int msg_len = strlen(msg);
  int i;
  int a; // alphabet's index
  int k = 0; // key's index
  
  for (i = 0; i < msg_len; i++) {
    a = (key[k] - 'A' + msg[i] - 'A') % 26;
    out[i] = ALPHA[ a ];
    k = (k+1) % key_len;
  }
  
  out[i] = '\0';
}

void decipher(char* key, char* msg, char* out) {
  int key_len = strlen(key);
  int msg_len = strlen(msg);
  int a; // alphabet's index 
  int k = 0; // key's index
  int i;
  
  for (i = 0; i < msg_len; i++) {
	  
    a = msg[i] - key[k];
    if (a < 0) a = 26 + a;
    
    out[i] = ALPHA[ a ];
    k = (k+1) % key_len;
  }
  
  out[i] = '\0';

}

int main() {

  char* HELP =
  "Carroll's Alphabet (Vigenere cipher)\n\n"
  
  "Example:\n"
  "KEY: vigilance\n"
  "INP: Meet me on Tuesday evening at seven.\n"
  "MSG: MEETMEONTUESDAYEVENINGATSEVEN\n"
  "OUT: HMKBXEBPXPMYLLYRXIIQTOLTFGZZV\n\n";

  char raw_key[MAX+1], key[MAX+1];
  char raw_msg[MAX+1], msg[MAX+1];
  char out[MAX+1];
  char c;
  
  printf(HELP);
  do {
    puts("What do you want to do? [E]ncipher or [D]ecipher?");
    scanf(" %c", &c);
    fflush(stdin); // to clear stdin. IMPORTANT for gets()
    c = toupper(c); // to accept 'e' and 'd'
  } while (c != 'E' && c != 'D');
  
  printf("Enter key: "); gets(raw_key);
  printf("Enter msg: "); gets(raw_msg);
  
  preprocess(raw_key, key);
  preprocess(raw_msg, msg);

  if (c == 'E') {
    encipher(key, msg, out);
  } else {
    decipher(key, msg, out);
  }
  
  puts("\n=== OUTPUT ===\n");  
  puts(out);
  
  return 0;
}
