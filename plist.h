#ifndef LIST_H
#define LIST_H

#include<stdlib.h>
#define PLUS 1
#define MINUS -1

//node structure for number
typedef struct node{
  int data;
  struct node *prev;
  struct node *next;
}node;

//number structure for large numbers
typedef struct number{
  node *head;
  node *tail;
  int size;
  int sign;
}number;

void initNumber(number *n);
void addToNumber(number *n, int d);
void printNumber(number n);
void destroyNumber(number *n);
void pushNumber(number *n, int d);

#endif
