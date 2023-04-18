#include"plist.h"

//number stack for number
typedef struct n_node{
  number *data;
  struct n_node *next;
}n_node;

typedef n_node *n_stack;

void ninitStack(n_stack *S);
void npush(n_stack *S, number *n);
number *npop(n_stack *S);
int nisEmpty(n_stack S);
number *n_peek(n_stack S);
void display(n_stack S);

/*******************************************************/

//character stack for operator
typedef struct c_node{
  char data;
  struct c_node *next;
}c_node;

typedef c_node *c_stack;

void cinitStack(c_stack *S);
void cpush(c_stack *S, char data);
char cpop(c_stack *S);
int cisEmpty(c_stack S);
char c_peek(c_stack S);
