/**
 * Any.c, kinda dynamic types in C, inspired by JavaScript (?)
 *
 * I couldn't sleep one night so I started this crappy thingy 
 * The next morning I simply lost interest.
 * I'll just upload this non-working code and I'm done with it...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* GENERAL FUNCTIONS */
char* new_strcat(char* str1, char* str2) {
  int size = strlen(str1) + strlen(str2) + 1;
  char* out = (char*)malloc(size * sizeof(char));
  sprintf(out, "%s%s", str1, str2);
  return out;
}

/* Maybe simplify it by doing as JS did with types? */
/* only `number` (double, float, int) and `string` (char*, char) */

typedef struct {
  char type; /* as in printf: 'i', 'f', 'c', or 's' */
  void *data;
} Any;

Any newObj(char type, void *data) {
  Any *obj = (Any*)malloc(sizeof(Any));

  if (type == 's') {
    /* so that we can free() it later, ig */
    data = (void*)strdup((char*)data);
  }
  
  obj->type = type;
  obj->data = data;
  return *obj; /* I'll need to edit everything to use pointers */
}

int isValidOperation(char o, char t1, char t2) {
  /* works on all types: either an addition or a concatination will be performed */
  if (o == '+') {
    return 1;
  }
  
  /* other operations are only valid on numbers */
  if ((t1 == 'i' || t1 == 'f') && (t2 == 'i' || t2 == 'f')) {
    return 1;
  }
  
  return 0;
}

int isValidConversion(char from, char to) {
#define ROWS 5
  char ALLOWED[ROWS][2] = {
    /* {from, to}  */
    {'i', 'f'},
    {'f', 'i'},  
    {'i', 's'},
    {'f', 's'},
    {'c', 's'}
  };

  
  if (from == to) {
    return 1;    
  }
  
  int i;
  for (i = 0; i < ROWS; i++) {
    if (ALLOWED[i][0] == from && ALLOWED[i][1] == to) {
      return 1;
    }
  }

  return 0;
}

Any convObj(Any obj, char toType) {
  if (obj.type == toType) {
    return obj;
  }
  
  if (!isValidConversion(obj.type, toType)) {
    printf("ERROR: can't convert from '%c' to '%c'\n", obj.type, toType);
    exit(1);
  }
  
  Any result;
  result.type = toType;
  result.data = /* convert */ obj.data;
  return result;
}

char* objToString(Any obj) {
  char* str;

  switch (obj.type) {
    case 's':
      str = strdup( (char*)obj.data );
      break;
      
    case 'c':
      str = (char*)malloc(2 * sizeof(char));
      *(str+0) = *(char*)obj.data;
      *(str+1) = '\0';
      break;
      
    case 'i':
      str = (char*)malloc(50 * sizeof(char));
      sprintf(str, "%i", obj.data);    
      break;
      
    case 'f':
      str = (char*)malloc(50 * sizeof(char));
      sprintf(str, "%g", obj.data);
      break;
  }
  
  return str; 
}

Any operate(Any x, char o, Any y) {
  char type;
  void *data;
  
  switch (o) {
    case '+':
      if (x.type == 's' || x.type == 'c' || x.type == 's' || y.type == 'c' ) {
        type = 's';
        char* str = new_strcat(objToString(x), objToString(y));
        data = (void*)str;
        break;
      }

      if (x.type == 'i' && y.type == 'i') {
        type = 'i';
        data = (void*)(*(int*)x.data + *(int*)y.data);
        break;
      } else {
        type = 'f';
        data = (void*)
          (
            ( x.type == 'i' ? *(int*)x.data : *(float*)x.data )
          +
            ( y.type == 'i' ? *(int*)y.data : *(float*)y.data )
          );
      }
  }

  return newObj(type, data);
}

/* for testing */
int main() {

  Any i1 = newObj('i', (void*)600);
  Any i2 = newObj('i', (void*)66);
  Any i3 = operate(i1, '+', i2);
  
  printf("i1 = %s\n", objToString(i1));
  printf("i2 = %s\n", objToString(i2));
  printf("i3 = %s\n", objToString(i3));
  
  Any c1 = newObj('c', (void*)'X');
  Any s1 = newObj('s', "HEYYO!");
  Any s2 = operate(s1, '+', i3);
  Any s3 = operate(c1, '+', i3);
  
  printf("s3 = %s", objToString(s3));
  
  return 0;
}
