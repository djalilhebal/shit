/**
 * Carroll's Breaker - to break Lewis Carroll's Alphabet cipher
 * Gotta rewrite it later. Maybe use probability vectors? Cosine similarity?
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//=================<< TEXT/CORPUS >>====================
/**
 * These are some verses of my fav songs
 * I should've used a text that includes most of English's character combinations.
 */
char* TEXT = {
"I FEEL FREE - NICKI MINAJ"
"My man you're bad but Nicki is badder"
"Stack your cookies up, go get you a ladder"

"THE SOUND OF MADNESS - SHINEDOWN"
"Somehow I'm still here to explain"
"That the darkest hour never comes in the night"

"ALONE IN A ROOM - ASKING ALEXANDRIA"
"All I needed was the last thing I wanted"
"To sit alone in a room and say it all out loud"

"SILENCE IN THE SNOW - TRIVIUM"
"Here we all stand on this canvas of white"
"Our palette holds but only one shade tonight"
"Silence snows in, in her wintery chill"
"Let's paint the ground red with the blood of our kill"

"DEAD TO THE WORLD - BULLET FOR MY VALENTINE"
"I have died a hundred times before"
"Your words have ripped out my insides"
"I find revenge within my soul"
"I've tried a thousand times before"
"To leave my demons behind me for good"
"I'm trapped with no escape to find"

// from Wikipedia - Pangram:
"The quick brown fox jumps over the lazy dog"
"Jived fox nymph grabs quick waltz"
"Glib jocks quiz nymph to vex dwarf"
"Sphinx of black quartz, judge my vow"
"How vexingly quick daft zebras jump!"
"The five boxing wizards jump quickly"
"Jackdaws love my big sphinx of quartz"
"Pack my box with five dozen liquor jugs" 
};

//=================<< DECIPHER >>====================
/**
 * This is "BAD practice", I shouldn't've copied code from `Carroll's Alphabet.c`
 * Instead, I should've turned it into a header/lib and `include`d it
 */

#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // "ABC"[0] == 'A'

char* decipher(char* msg, char* key) {
  int key_len = strlen(key);
  int msg_len = strlen(msg);
  char* out = malloc(sizeof(char) * (msg_len+1));
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
 
  return out;
}

/* keep only letters (and uppercase them) */
char* preprocess(char* str) {
  int source_len = strlen(str);
  int result_len = 0;

  int i;
  for (i = 0; i < source_len; i++) {
    // result_len += isalpha(str[i]); // We could do this. Cool, huh? *-*
    if (isalpha(str[i])) {
      result_len++;
    }
  }
 
  char* result = malloc(sizeof(char) * (result_len+1));
  int j = 0;
  for (i = 0; i < source_len; i++) {
    if (isalpha(str[i])) {
      result[j++] = toupper(str[i]);
    }
  }
  result[j] = '\0';
 
  return result;
}

//=================<< DICTIONARY >>====================
/**
 * We'll be testing passwords that exist in this dictionary
 * otherwise I would end up complicating stuff or re-doing my `brute-forcer.c`
 */
char* DICT[] = {
  "ALICE",
  "BLACK",
  "CALM",
  "DARKNESS",
  "GRAY",
  "MALICE",
  "SILENCE",
  "TWILIGHT",
  "VIGILANCE",
  NULL
};

int dict_i = 0; /* dictionary index */

char* dict_nextWord() {
  return DICT[dict_i++];
}

//=================<< LANGUAGE MODEL >>====================
/**
 * Our language model is a character-based n-gram model, where n = 2
 * I chose n = 2 for simplicity. For better results n = 3 would be far better
 */
typedef struct model {
  int len; // length of training data
  int map[26][26]; // bigram LM => 2 dimensional vector
} LM;

void printModel(LM* m) {
  int i, j;
  printf("MODEL {len = %i}\n", m->len);
  for (i = 0; i < 26; i++) {
    for (j = 0; j < 26; j++) {
      if (!m->map[i][j]) continue;
      printf("%c%c - %i\n", ALPHA[i], ALPHA[j], m->map[i][j]);
    }
  }
  printf("\n");
}

int distance(LM* a, LM* b) {
  /*
  Think of each language model as a point that has 676 (26*26) coordinates.
  We need to calculate the distance between them.
  From Wikipedia - Euclidean distance:
 
  The standard Euclidean distance can be squared in order to place progressively
  greater weight on objects that are farther apart.
  ...
  Squared Euclidean Distance is not a metric, as it does not satisfy the triangle inequality;
  however, it is frequently used in optimization problems in which distances only have to be
  compared.
  */
 
  int d = 0; // Squared Euclidean Distance
  int i, j;
  for (i = 0; i < 26; i++) {
    for (j = 0; j < 26; j++) {
      int tmp = a->map[i][j] - b->map[i][j];
      d += tmp * tmp;
    }
  }
  return d; 
}

LM* newModel() {
  LM* m = malloc(sizeof(LM));
  m->len = 0;
  int i, j;
  for (i = 0; i < 26; i++) {
    for (j = 0; j < 26; j++) {
      m->map[i][j] = 0;
    }
  }

  return m;
}

int inRange(char c) {
  return (c >= 'A' && c <= 'Z');
}

int toIndex(char c) {
  return c - 'A';
}

void updateModel(LM* m, char* str) {
  const int len = strlen(str);
  if (len < 2) {
    puts("[updateModel] strlen(input) must be >= 2 !");
    return;
  }
  m->len += len - 1;
 
  int i;
  char a, b;
  for (i = 0; i < len -1; i++) {
    a = str[i];
    b = str[i+1];
    if (!inRange(a) || !inRange(b)) {
      continue;
    }
    m->map[ toIndex(a) ][ toIndex(b) ]++;
  }
}

LM* init_main_model() {
  LM* m = newModel();
  char* preprocessedText = preprocess(TEXT);
  updateModel(m, preprocessedText);
  free(preprocessedText);
  return m;
}

int main() {
  puts("INITIALIZING...");
  LM* m = init_main_model(); //printModel(m);
  
  // scanf it or idk
  char* encrypted = "UHLBGXTIDTKJQDZVVAMNEBQWFAJIPCYOCM";
 
  LM* _m; // temp model
  char* decoded;
  char* guess;
  int diff;
  char* bestGuess;
  int minDiff = 666*666;
 
  puts("GUESSING...");
  while (guess = dict_nextWord()) { // while there are more words
    decoded = decipher(encrypted, guess);
    _m = newModel();
    updateModel(_m, decoded);
    diff = distance(m, _m);
    if (diff < minDiff) {
      printf("%s seems better (-%i)\n", guess, diff);
      printf("[ %s ]\n\n", decoded);
      bestGuess = guess;
      minDiff = diff;
    }
   
    free(decoded);
    free(_m);
  }
 
  puts("DONE");
 
  return 0;
}
