#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"pstack.h"
#define C_ERROR '0'

//REQUIRED FUNCTIONS FOR NUMBER STACK

void ninitStack(n_stack *S){
  *S = NULL;
  return;
}

void npush(n_stack *S, number *n){
  n_node *nn;
  nn = (n_node*)malloc(sizeof(n_node));
  if(!nn)
    return;
  nn->data = n;
  nn->next = NULL;
  if(*S == NULL){
    *S = nn;
    return;
  }
  else{
    nn->next = *S;
    *S = nn;
    return;
  }
}

int nisEmpty(n_stack S){
  if(S == NULL)
    return 1;
  return 0;
}

number *npop(n_stack *S){
  if(nisEmpty(*S))
    return NULL;
  n_node *p;
  p = *S;
  if(p->next == NULL){
    number *n = p->data;
    *S = NULL;
    free(p);
    return n;
  }
  number *n = p->data;
  *S = p->next;
  free(p);
  return n;
}

number *n_peek(n_stack S){
  if(nisEmpty(S))
    return NULL;
  return S->data;
}

void display(n_stack S){
  n_node *p;
  p = S;
  while(p){
    printNumber(*(p->data));
    p = p->next;
  }
  printf("\n");
}

//REQUIRED FUNCTIONS FOR CHARACTER STACK

void cinitStack(c_stack *S){
  *S = NULL;
  return;
}

void cpush(c_stack *S, char data){
  c_node *nn;
  nn = (c_node*)malloc(sizeof(c_node));
  if(!nn)
    return;
  nn->data = data;
  nn->next = NULL;
  if(!*S){
    *S = nn;
    return;
  }
  nn->next = *S;
  *S = nn;
  return;
}

int cisEmpty(c_stack S){
  if(S == NULL)
    return 1;
  return 0;
}

char cpop(c_stack *S){
  char c = C_ERROR;
  if(cisEmpty(*S))
    return c;
  c_node *p;
  p = *S;
  if(p->next == NULL){
    *S = NULL;
    c = p->data;
    free(p);
    return c;
  }
  c = p->data;
  *S = p->next;
  free(p);
  return c;
}

char c_peek(c_stack S){
  char c = C_ERROR;
  if(cisEmpty(S))
    return c;
  c = S->data;
  return c;
}

