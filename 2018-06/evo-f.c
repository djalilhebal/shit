// 2018-06-05 -- an evolutionary/genetic shit that learns a mathy function (?)

#include <stdio.h>
#include <stdlib.h> // malloc, free, rand, srand
#include <time.h> // time(NULL) to seed to the `rand` function

// =============== CONSTANTS ===============

#define LETTERS "0123456789x+-*/"
#define LETTERSLEN 15
#define GENELEN 8
#define POPUSIZE 16
#define MAXGENERATIONS 66666 // max attempts

// LEARNING DATA
#define DATASIZE 7
int DATA[DATASIZE][2] = {
  // f(x) = -x + 1 = 0-x +1 -> "+1-0x" = "-1x"
  // {x, f(x)}
  {0, 1},
  {1, 0},
  {2, -1},
  {3, -2},
  {-1, 2},  
  {-2, 3},
  {-3, 4}
};

// =============== TYPES ===============

typedef enum {false, true} bool;

typedef struct {
  char* seq; // "sequence": prefix notation expression: x+y -> +xy, 2*x -> *2x
  int score; // score errors, the less the better
} Gene;

typedef struct {
  bool found; // have we found a sequence that works on all the training data?
  int generation;
  Gene genes[POPUSIZE];
} Population;

// =============== PROTOTYPES ===============

int absDiff(int a, int b);
char* newSeq();
char randomLetter();
char* randomSeq();
int scoreSeq(char* seq);
int eval(char* seq, int x);
char* mateGenes(char* a, char* b);

Population* randomPopulation();
void testPopulation(Population* p);
void sortPopulation(Population* p);
void matePopulation(Population* p);
void printPopulation(Population* p);

// =============== FUNCTIONS ===============

int main() {
  srand(time(NULL));

  Population* p = randomPopulation();

  printf("Working...\n");
  do {
    testPopulation(p);
    sortPopulation(p);
    if (p->found) break;
    matePopulation(p); // nextGeneration kichghl
    
  } while (true); //(p->generation < MAXGENERATIONS);
  
  printPopulation(p);

  return 0;
}

// POPULATION FUNCTIONS

Population* randomPopulation() {
  Population* p = (Population*) malloc(sizeof(Population));
  p->found = false;
  p->generation = 0;
  int i;
  for (i = 0; i < POPUSIZE; i++) {
    p->genes[i].seq = randomSeq();
    //p->genes[i].score = -1; // it will be set by the test function anyway
  }
  return p;
}

void testPopulation(Population* p) {
  int i, score;
  p->found = false;
  for (i = 0; i < POPUSIZE; i++) {
    char* seq = p->genes[i].seq;
    int score = scoreSeq(seq);
    if (score == 0) {
      p->found = true;
    }
    p->genes[i].score = score;
  }
  
}

void sortPopulation(Population* p) { // selection sort
  int i, j, k;
  Gene tmp;
  for (i = 0; i < POPUSIZE - 1; i++) {
    k = i;
    for (j = i; j < POPUSIZE; j++) {
      if (p->genes[j].score > p->genes[k].score)
        k = j;
    }
    tmp = p->genes[k];
    p->genes[k] = p->genes[i];
    p->genes[i] = tmp;
  }
}

void matePopulation(Population* p) {
  int i, j;
  char *parent1, *parent2, *child1, *child2;
  for (i = 0; i < POPUSIZE/2 - 1; i++) {
    j = POPUSIZE/2 + i + 1; // "kill"/replace the second half of the population
    parent1 = p->genes[i].seq;
    parent2 = p->genes[i+1].seq;
    child1 = mateGenes(parent1, parent2);
    child2 = mateGenes(parent2, parent1);
    free(p->genes[i].seq);
    free(p->genes[j].seq);
    p->genes[i].seq = child1;
    p->genes[j].seq = child2;
  }
  p->generation++;
}

void printPopulation(Population* p) {
  int i;
  printf("--------{ Gen. %i }--------\n", p->generation);
  for (i = 0; i < POPUSIZE; i++)
    printf("Gene#%i (%i) %s \n", i, p->genes[i].score, p->genes[i].seq);
  printf("------------------------------\n", p->generation);
  
}

// GENE FUNCTIONS

int absDiff(int a, int b) {
  // "absolute difference"
  //
  // EXAMPLES:
  // absDiff(3, 3) == 0;
  // absDiff(1, 3) == 2;
  // absDiff(1, -3) == 2;
  
  int result;
  
  if ((a >= 0 && b >= 0) || (a <= 0 && b <= 0)) // same sign
    result = a - b;
  else
    result = a + b;
  
  if (result >= 0)
    return result;
  else
    return -result;
}

char randomLetter() {
  return LETTERS[rand()%LETTERSLEN];
}

char* newSeq() {
  char* seq = (char*)malloc((GENELEN+1)*sizeof(char));
  seq[GENELEN] = '\0';
  return seq;
}

char* randomSeq() {
  char* seq = newSeq();
  int i;
  for (i = 0; i < GENELEN; i++) {
    seq[i] = randomLetter();
  }
  return seq;
}

char* mateGenes(char* a, char* b) { // half a + half b & maybe mutate randomly
  char* seq = newSeq();
  int i;
  for (i = 0; i < GENELEN/2; i++)
    seq[i] = a[i];
  for (i = GENELEN/2; i < GENELEN; i++)
    seq[i] = b[i];
  
  if (rand()%100 < 3) { // 3% chance for a mutation to happen
    // put a random `letter` in a random position
    seq[rand()%GENELEN] = randomLetter();
  }
  
  return seq;
}

int scoreSeq(char* seq) {
  int i, x, y, z;
  int score = 0;
  
  for (i = 0; i < DATASIZE; i++) {
    // f(x) = y
    // seq(x) = z
    x = DATA[i][0];
    y = DATA[i][1];
    z = eval(seq, x);
    score -= absDiff(y, z);
  }
  
  return score;
}

// evaluate prefix expressions using a static stack
struct {
  int arr[256];
  int top;
} stack; // stack is a global *variable*

void init() {
  stack.top = -1;
}

void push(int n) {
  stack.top++;
  stack.arr[stack.top] = n;
}

int pop() {
  if (stack.top < 0) return 0;
  return stack.arr[stack.top--];
}

int eval(char* seq, int x) {
  int a, b, i;
  char c;

  init(); // resent the stack
  
  for (i = GENELEN - 1; i >= 0; i--) {
    c = seq[i];
    if (c == 'x') push(x);
    if (c >= '0' && c <= '9') push(c - '0');
    switch(c) {
      case '+':
        push(pop() + pop());
        break;
      case '-':
        push(pop() - pop());
        break;
      case '*':
        push(pop() * pop());
        break;
      case '/':
        a = pop();
        b = pop();
        if (b == 0)
          push(0);
        else
          push(a / b);
    
        break;
    }
  }
  
  return pop();
}
// end eval

// END FUNCTIONS
