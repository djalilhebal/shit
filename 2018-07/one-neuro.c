/**
 * one-neuro.c -- 2018-07-01
 * inspired by ericelliott/minimal-neuron.js
 * Given a list of inputs, decides if a neuron should fire or not.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct list {
  double value;
  double weight;
  struct list* next;
} Synapse;

typedef struct {
  double threshold;
  Synapse* synapses;
} Neuron;

void addSynapse(Synapse** root, double value, double weight) {
  Synapse* s = malloc(sizeof(Synapse));
  s->value = value;
  s->weight = weight;
  s->next = *root;
  *root = s;
}

Neuron* newNeuron(double threshold, Synapse* synapses) {
  Neuron* n = malloc(sizeof(Neuron));
  n->threshold = threshold;
  n->synapses = synapses;
  return n;
}

void freeNeuron(Neuron* n) {
  // free its synapses and then actually free it
  Synapse* s = n->synapses;
  Synapse* tmp;
  while (s != NULL) {
    tmp = s->next;
    free(s);
    s = tmp;
  }
  
  free(n);
}

double sumSynapses(Synapse* synapses) {
  double result = 0.0;
  Synapse* p = synapses;
  while (p != NULL) {
    result += p->value * p->weight;
    p = p->next;
  }
  return result;
}

int shouldFire(Neuron* n) {
  double sum = sumSynapses(n->synapses);
  return sum >= n->threshold;
}

int main() {
  // for testing
  
  Synapse* s1 = NULL;
  addSynapse(&s1, 1.5, 2.0);
  addSynapse(&s1, 2.5, 2.0);  
  addSynapse(&s1, 1.0, -1.0);
  
  Neuron* n1 = newNeuron(5.0, s1);
  printf("Neuron#1 should fire? %i\n", shouldFire(n1));
  freeNeuron(n1);

  Synapse* s2 = NULL;
  addSynapse(&s2, 66.6, 0.0);
  addSynapse(&s2, 42.0, -13.0);
  addSynapse(&s2, 1.0, 1.0);
  
  Neuron* n2 = newNeuron(6.9, s2);
  printf("Neuron#2 should fire? %i\n", shouldFire(n2));
  freeNeuron(n2);
  
  return 0;
}
