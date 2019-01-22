/**
 * Leo: To Lie Or Not To Lie? 2018-07-xx 
 * "inspired" by this masterpiece github.com/serendipious/nodejs-decision-tree-id3
 * Mine is a total piece of unreadable shit :p (gotta edit/rewrite it someday)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*____________________/ DATA/LIES \____________________*/
/*
lie {
  they_know, // do they already know (or suspect) the truth?
  exposable, // can somebody else tell them and expose me?
  provable, // can the lie/truth be proved? Is there evidence?
  truth_hurts, // will I (or someone else) get hurt if I say the truth?
  lie_hurts, // will I/someone get hurt (or damage reputation) if found lying?
  lie; // Should I lie or tell the truth?
};
*/

#define DATASIZE 15
#define ATTRIBUTES_LEN 6
#define TARGET 5 // index of the target: lie

const char* ATTR_VALS[2] = {"no", "yes"};
const char* ATTR_NAMES[6] =
{ "they know", "exposable", "provable", "truth hurts", "lie hurts", "lie" };

int DATA[DATASIZE][ATTRIBUTES_LEN] =
{
  {0, 0, 0, 0, 1, 0}, // Can you stop using 'formal' Arabic words? (Bien sur!!)
  {1, 0, 0, 1, 0, 1}, // Can you say something in Japanese? (No!)
  {0, 1, 0, 0, 1, 0}, // Where is your phone?
  {0, 0, 0, 1, 0, 1}, // Where is the last slice of pizza?
  {0, 0, 0, 0, 0, 0}, // Why do you like Nicki Minaj?
  {0, 1, 0, 1, 0, 1}, // Why do you want to move to Canada?
  {0, 0, 1, 0, 1, 0}, // Have you read the Quran?
  {0, 1, 0, 1, 0, 1}, // Have you read the Bible?
  {0, 0, 1, 0, 1, 0}, // Do you know <a person that should be known>? (e.g. imam)
  {0, 0, 0, 1, 0, 1}, // Do you know <a person that must not be known>?
  {1, 0, 0, 0, 0, 0}, // Are you an otaku?
  {0, 1, 0, 1, 0, 1}, // Are you not a good boy?
  {1, 1, 1, 0, 1, 0}, // What do you study?
  {0, 1, 1, 1, 0, 1}, // what's your school average?
  {0, 1, 1, 1, 0, 1}, // You love me? No -_-
};

/*____________________/ TYPES \____________________*/

typedef struct list {
  union {
    int _int; // attribute's number
    int* _array; // an array of attributes
    double _double; // probas/entropies
  } as;
  struct list* next;
} List;

typedef struct tree {
  int label;
  int attribute;
  //int vi;
  struct tree** branches; // we'll be using just branches[0] and branches[1]
} Tree;

/*____________________/ PROTOTYPES \____________________*/

Tree* ID3(List* examples, int target, List* attributes);
int traverseTree(Tree* tree, int* x); // to decide
Tree* newTree(); // allocate an empty tree
Tree** allocEmptyBranches(int);
void addSubtree(Tree*, Tree*);
int isLeaf(Tree*);

void printTree(Tree* tree, int depth);
void printExamples(List* list);
void printAttrs(List* list);

List* init_attributes_list(int);
List* init_examples_list(int arr[DATASIZE][ATTRIBUTES_LEN], int len);

List* newList();
List* newNode(); // just an 'alias'
void freeList(List* list);
List* omitElement(List* list, int el);
List* filterList(List* list, int attr, int val);
void addNode(List** list, List* node); // procedure ajoutD;
int areAllAttr(List*, int, int);
int isEmptyList(List*);
int mostCommonValue(List*, int);
int bestAttribute(List*, int, List*);

/*____________________/ ALGORITHM \____________________*/
/* (Wikipedia 'ID3 Algorithm' :p)
ID3 (Examples, Target_Attribute, Attributes)
    Create a root node for the tree
    If all examples are positive, Return the single-node tree Root, with label = +.
    If all examples are negative, Return the single-node tree Root, with label = -.
    If number of predicting attributes is empty, then Return the single node tree Root,
    with label = most common value of the target attribute in the examples.
    Otherwise Begin
        A := The Attribute that best classifies examples.
        Decision Tree attribute for Root = A.
        For each possible value, vi, of A,
            Add a new tree branch below Root, corresponding to the test A = vi.
            Let Examples(vi) be the subset of examples that have the value vi for A
            If Examples(vi) is empty
                Then below this new branch add a leaf node with label = most common target value in the examples
            Else below this new branch add the subtree ID3 (Examples(vi), Target_Attribute, Attributes - {A})
    End
    Return Root
*/

/*____________________/ FUNCTIONS \____________________*/
Tree*
ID3 (List* examples, int target, List* attributes) {
  printExamples(examples);
  printAttrs(attributes);
  Tree* root = newTree();
  if (areAllAttr(examples, target, 0)) {
    root->label = 0;
  } else if (areAllAttr(examples, target, 1)) {
    root->label = 1;
  } else if (isEmptyList(attributes)) {
    root->label = mostCommonValue(examples, target);
  } else {
    int A = bestAttribute(examples, target, attributes);
    printf("\nA = %i(%s)\n\n", A, ATTR_NAMES[A]);
    List* possibleValuesForA = init_attributes_list(2); // we only use {1, 0}
    int numberOfPossibleValuesForA = listLength(possibleValuesForA);
    root->attribute = A;
    root->branches = allocEmptyBranches(numberOfPossibleValuesForA);
    for (; possibleValuesForA != NULL; possibleValuesForA = possibleValuesForA->next) {
    int vi = !possibleValuesForA->as._int;
      List* examples_subset = filterList(examples, A, vi);
      if (isEmptyList(examples_subset)) {
        Tree* leaf = newTree();
        leaf->label = mostCommonValue(examples, target);
        addSubtree(root, leaf);
      } else {
        Tree* subtree = ID3(examples_subset, target, omitElement(attributes, A));
        addSubtree(root, subtree);
      }
    }
  }
  return root;
}

int traverseTree(Tree* branch, int* x) {
  int v;
  while (!isLeaf(branch)) {
    v = x[ branch->attribute ];
    branch = branch->branches[ v ];
  }
  return branch->label;
}

// http://dsw.users.sonic.net/entropy.html
// https://math.stackexchange.com/questions/331103/intuitive-explanation-of-entropy
double entropy(List* examples, int target) {
  double sumOfLogVals = 0.0; // the result
  int total_length = listLength(examples);
  List* uniqueVals = init_attributes_list(2); // {0, 1}

  // for each unique value `v` of target
  for (; uniqueVals != NULL; uniqueVals = uniqueVals->next) {
    int v = uniqueVals->as._int;
    int count = countAttr(examples, target, v);
    double p = (double) count / (double) total_length;
    double logVal = -p * log2(p);
    sumOfLogVals += logVal;
  }

  return sumOfLogVals;
}

// calc information gain for this `attr`
double calcGain(List* examples, int target, int attr) {
  List* attrVals = init_attributes_list(2); // {0, 1}
  double setEntropy = entropy(examples, target);
  int setSize = listLength(examples);
  double sumOfEntropies = 0.0;

  for (; attrVals != NULL; attrVals = attrVals->next) {
    int v = attrVals->as._int;
    List* subset = filterList(examples, attr, v);
    double proba = (double)listLength(subset) / (double)setSize;
    double entro = entropy(subset, target);
    sumOfEntropies += proba * entro;
  }

  return setEntropy - sumOfEntropies;
}

// bestAttribute = the attr which has the max info gain
int bestAttribute(List* examples, int target, List* attribs) {
  int bestAttr = attribs->as._int;
  double bestGain = calcGain(examples, target, attribs->as._int);

  for (; attribs != NULL; attribs = attribs->next) {
    int attrib = attribs->as._int;
    double gain = calcGain(examples, target, attrib);
    if (gain > bestGain) {
      bestGain = gain;
      bestAttr = attrib;
    }
  }

  return bestAttr;
}

// ======== LIST FUNCTIONS ===========

List* newList() {
  return (List*)malloc(sizeof(List));
}

List* newNode() {
  return newList();
}

int isEmptyList(List* list) {
  return list == NULL;
}

int listLength(List* list) {
  int len = 0;
  for (; list != NULL; list = list->next) {
    len++;
  }
  return len;
}

// how many times the attribute `attr` has appeared with value `val`
int countAttr(List* list, int attr, int val) {
  int result = 0;

  for (; list != NULL; list = list->next) {
    if (list->as._array[attr] == val) {
      result++;
    }
  }

  return result;
}

int areAllAttr(List* list, int attr, int val) {
  int total = listLength(list);
  int count = countAttr(list, attr, val);
  return count == total;
}

// most common value of `attr` (either 0 or 1)
int mostCommonValue(List* examples, int attr) {
  if (countAttr(examples, attr, 0) > countAttr(examples, attr, 1)) {
    return 0;
  } else {
    return 1;
  }
}

// clone and keep only elms that have array[attr] == val
List* filterList(List* list, int attr, int val) {
  List* result = NULL;

  for (; list != NULL; list = list->next) {
    if (list->as._array[attr] == val) {
    List* clone = newNode();
    clone->as._array = list->as._array;
    addNode(&result, clone);
    }
  }

  return result;
}

// CLONE 'list' and keep only the elements that have val != el
List* omitElement(List* list, int el) {
  List* result = NULL;

  for (; list != NULL; list = list->next) {
    if (list->as._int != el) {
    List* clone = newNode();
    clone->as._int = list->as._int;
    addNode(&result, clone);
    }
  }

  return result;
}

void freeList(List* list) {
  List* tmp;
  while (list != NULL) {
    tmp = list->next;
    free(list);
    list = tmp;
  }
}

void addNode(List** list, List* node) {
  node->next = *list;
  *list = node;
}

// init_attributes_list(3);
// result: 2 -> 1 -> 0
List* init_attributes_list(int n) {
  List* list = NULL;
  int i;
  for (i = 0; i < n; i++) {
    List* node = newNode();
    node->as._int = i;
    addNode(&list, node);
  }
  return list;
}

List* init_examples_list(int arr[DATASIZE][ATTRIBUTES_LEN], int len) {
  // Given an array of examples, convert them to a list
  List* list = NULL;
  int i;
  for (i = 0; i < len; i++) {
    List* node = newNode();
    node->as._array = arr[i];
    addNode(&list, node);
  }
  return list;
}

// ============== TREE FUNCTIONS ===========

Tree* newTree() {
  Tree* tree = malloc(sizeof(Tree));
  tree->branches = NULL;
  return tree;
}

Tree** allocEmptyBranches(int n) {
  Tree** branches = malloc(n * sizeof(Tree**));
  for (; n --> 0 ;) {
    branches[n] = NULL;
  }
  return branches;
}

void addSubtree(Tree* root, Tree* branch) {
  int i = 0;
  while (root->branches[i] != NULL) ++i;
  root->branches[i] = branch;
}

int isLeaf(Tree* branch) {
  return branch->branches == NULL;
}

// ========= OUTPUT FUNCTIONS ========

void indent(int n) {
  for (; n --> 0 ;) { // for n downto 0
    printf("\t");
  }
}

void printTree(Tree* tree, int depth) {
  if (depth == 0) {
    printf("\n\n-----{ DECISION TREE }-----\n\n");
  }

  indent(depth);

  if (isLeaf(tree)) {
    printf("Lie? %s!\n", ATTR_VALS[tree->label]);
  } else {
    printf("%s ?\n", ATTR_NAMES[tree->attribute]);

    indent(depth); printf("No\n");
    printTree(tree->branches[0], depth+1);

    indent(depth); printf("Yes\n");
    printTree(tree->branches[1], depth+1);
  }

  printf("\n");
}

void printExamples(List* list) {
  printf("=== examples (%i) ===", listLength(list));
  for (; list != NULL; list = list->next) {
    printf("\n- ");
    int i;
    for (i = 0; i < 6; i++) {
      printf("%i ", list->as._array[i]);
    }
  }
  printf("\n");
}

void printAttrs(List* list) {
  printf("Attributes (%i): ", listLength(list));
  for (; list != NULL; list = list->next) {
    printf("%i(%s) ", list->as._int, ATTR_NAMES[list->as._int]);
  }
  printf("\n\n");
}

int main() {
  puts("initializing examples list...");
  List* examples = init_examples_list(DATA, DATASIZE);
  puts("initializing attributes list...");
  List* attributes = omitElement(init_attributes_list(ATTRIBUTES_LEN), TARGET);
  puts("Building tree...");
  Tree* tree = ID3(examples, TARGET, attributes);
  printTree(tree, 0);

  puts("TESTING");
  printf("Situation X: Some angry mobs are yelling: Are you <insert something>?!! >:(\n");
  printf("They know. Exposable. Not provable. Truth fucking hurts. Lies don't hurt.\n");
  int x[] = {1, 1, 0, 1, 0, -1}; // `-1` can be omitted
  printf("Lie? %s!!", ATTR_VALS[traverseTree(tree, x)]);

  return 0;
}
