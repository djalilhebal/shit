// 2018-06-03 -- a simplistic/basic dynamic stack (no error handling and shit)

#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

typedef struct liste {
  int info;
  struct liste* suiv;
} Liste;

typedef struct {
  Liste* sommet;
} Pile;

void init_pile(Pile* p) {
  p->sommet = NULL;
}

bool non(bool cond) {
  if (cond)
    return false;
  else
    return true;
}

bool pile_vide(Pile* p) {
  return p->sommet == NULL;
}

void empiler(Pile* p, int x) {
  //  l: Liste; allouer(l);
  Liste* l = (Liste*)malloc(sizeof(Liste));
  l->info = x;
  l->suiv = p->sommet;
  p->sommet = l;
}

void depiler(Pile* p, int* x) {
  Liste* l = p->sommet;
  p->sommet = p->sommet->suiv;
  *x = l->info;
  free(l);
}

int main() {
  int x;
  Pile* p = (Pile*)malloc(sizeof(Pile));
  init_pile(p);
  
  empiler(p, 1);
  empiler(p, 2);

  while( non(pile_vide(p)) ) {
    depiler(p, &x);
    printf("%i ", x);
  }
  
  return 0;
}
