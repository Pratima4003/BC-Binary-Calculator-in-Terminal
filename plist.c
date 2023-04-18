#include"plist.h"
#include<limits.h>
#include<stdio.h>
#include<stdlib.h>


//initialise the number
void initNumber(number *n){
  n->head = NULL;
  n->tail = NULL;
  n->size = 0;
  n->sign = PLUS;
}

//add the integer to the end of the the list
void addToNumber(number *n, int d){
  node* nn;
  nn = (node*)malloc(sizeof(node));
  if(!nn)
    return;
  nn->data = d;
  nn->prev = NULL;
  nn->next = NULL;
  if(!n)
    return;
  if(n->head == NULL){
    n->size++;
    n->head = nn;
    n->tail = nn;
    return;
  }
  n->tail->next = nn;
  nn->prev = n->tail;
  n->tail = n->tail->next;
  n->size++;
  return;
}

//print the number
void printNumber(number n){
  if(n.head == NULL){
    printf("No Number To Print\n");
    return;
  }
  if(n.sign == -1){
    printf("-");
  }
  node *p = n.head;
  int i = 0;
  while(p){
    printf("%d", p->data);
    p = p->next;
    i++;
    if(i > 50){
      printf("\\");
      printf("\n");
      i = 0;
    }
  }
  printf("\n");
  return;
}

//free all the nodes and destroy the number
void destroyNumber(number *n){
  node *p;
  p = n->head;
  if(!p)
    return;
  node*q = NULL;
  while(p){
     q = p->next;
     free(p);
     p = q;
  }
  n->head = NULL;
  n->tail = NULL;
  n->size = 0;
  n->sign = PLUS;
  return;
}

//add the node at the start of the number
void pushNumber(number *n, int d){
  node *nn;
  nn = (node*)malloc(sizeof(node));
  if(!nn)
    return;
  nn->data = d;
  nn->next = NULL;
  nn->prev = NULL;
  if(n->head == NULL){
    n->size++;
    n->head = nn;
    n->tail = nn;
    return;
  }
  nn->next = n->head;
  n->head->prev = nn;
  n->head = nn;
  n->size++;
  return;
}
