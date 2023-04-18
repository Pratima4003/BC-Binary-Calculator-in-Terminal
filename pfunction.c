#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include"pfunction.h"

//UTILITY FUNCTIONS

//function to equuilise the numbers in terms of digits
void equiliseNumber(number **n1, number **n2){
  int i = (*n1)->size;
  int j = (*n2)->size;
  if(i > j){
    while(i > j){
      pushNumber(*n2, 0);
      j++;
    }
  }
  else{
    while(j > i){
      pushNumber(*n1, 0);
      i++;
    }
  }
  return;
}

//function to check if the number is zero
int zeroNumber(number *n){
  node*p = n->head;
  while(p){
    if(p->data != 0)
      return 0;
    p = p->next;
  }
  return 1;
}

//function to remove trailing/leading zeros
void removeTrailingZeros(number *n){
  if(n->head == NULL)
    return;
  if(zeroNumber(n)){
    destroyNumber(n);
    addToNumber(n, 0);
    return;
  }
  node*p = n->head;
  node*q = NULL;
  while(p->data == 0){
    q = p->next;
    free(p);
    p = q;
    n->size--;
  }
  n->head = p;
  n->head->prev = NULL;
  return;
}

int compareNumbers(number *n1, number *n2){
  node*p = n1->head;
  node*q = n2->head;
  if(n1->size > n2->size)
    return 1;
  else if(n1->size < n2->size)
    return -1;
  else{
    while(p && q){
      if(p->data > q->data)
        return 1;
      else if(q->data > p->data)
        return -1;
      p = p->next;
      q = q->next;
    }
  }
  return 0;
}

/************************************************************/

//required functions
//NOTE:In all the following functions, n1 is the second number and n2 is first number
//E.g.: for 12+3;
//n1 is 3 and n2 is 12


//addition function
number *addNumbers(number *n1, number *n2){
  removeTrailingZeros(n1);
  removeTrailingZeros(n2);
  
  if(zeroNumber(n1)){
    destroyNumber(n1);
    free(n1);
    return n2;
  }
  else if(zeroNumber(n2)){
    destroyNumber(n2);
    free(n2);
    return n1;
  }
  
  equiliseNumber(&n1, &n2);
  node*p = n1->tail;
  node*q = n2->tail;
  int carry = 0;
  number *sumNumber;
  sumNumber = (number*)malloc(sizeof(number));
  initNumber(sumNumber);
  
  //if both the numbers have negative sign, then the ans must also have negative sign
  if(n1->sign == MINUS && n2->sign == MINUS){
    sumNumber->sign = MINUS;
  }
  
  //if the sign of both the numbers is different then subtract them
  if(n1->sign != n2->sign){
    n1->sign = n2->sign;
    sumNumber = subNumbers(n1, n2, 1);
    return sumNumber;
  }
  
  //iterate the numbers from back and perform addition operation
  while(p && q){
    int sum = p->data + q->data + carry;
    carry = sum / 10;
    pushNumber(sumNumber, sum % 10);
    if(p)
      p = p->prev;
    if(q)
      q = q->prev;
  }
  
  //if any carry is remaining, push it to the end of the final ans number
  if(carry != 0)
    pushNumber(sumNumber, carry);
  
  return sumNumber;
}

//function to subtract two numbers
number *subNumbers(number *n1, number *n2, int freeNum){
  removeTrailingZeros(n1);
  removeTrailingZeros(n2);
  
  //if either of the numbers is zero
  if(zeroNumber(n1)){
    destroyNumber(n1);
    free(n1);
    return n2;
  }
  else if(zeroNumber(n2)){
    n1->sign = MINUS;
    destroyNumber(n2);
    free(n2);
    return n1;
  }
  
  number *diffNumber;
  diffNumber = (number*)malloc(sizeof(number));
  initNumber(diffNumber);
  node *p, *q;
  
  //if the sign both the numbers is not equal, the add them and put the sign as negative
  if(n1->sign != n2->sign){
    n1->sign = n2->sign;
    diffNumber = addNumbers(n1, n2);
    return diffNumber;
  }
  
  //compare and according set p and q pointers
  if(compareNumbers(n2, n1) == 1){
    p = n1->tail;
    q = n2->tail;
  }
  else if(compareNumbers(n1, n2) == 1){
    p = n2->tail;
    q = n1->tail;
    diffNumber->sign = MINUS;
  }
  //special case: if both the numbers are zero
  else if(compareNumbers(n1, n2) == 0){
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    addToNumber(diffNumber, 0);
    return diffNumber;
  }
  equiliseNumber(&n1, &n2);
  
  if(n1->sign == MINUS && n2->sign == MINUS){
    if(compareNumbers(n1, n2) == 1)
      diffNumber->sign = PLUS;
    else
      diffNumber->sign = MINUS;
  }
  
  int borrow = 0;
  
  //Iterate over p and q and subtract each digit also keep note of borrow
  while(p && q){
    int sub = q->data - p->data - borrow;
    if(sub < 0){
      sub = sub + 10;
      borrow = 1;
    }
    else
      borrow = 0;
    pushNumber(diffNumber, sub);
    p = p->prev;
    q = q->prev;
  }
  
  //If freeNum flag is set as true then only free both the numbers, else dont
  if(freeNum){
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
  }
  else{
    removeTrailingZeros(n1);
    removeTrailingZeros(n2);
  }
  
  removeTrailingZeros(diffNumber);
  return diffNumber;  
}

//program to multiply two numbers
//this is based on the multiplication technique we do on paper
number *mulNumbers(number *n1, number *n2, int freeNum){
  number *mulNumber;
  mulNumber = (number*)malloc(sizeof(number));
  initNumber(mulNumber);
  int mul_sign = PLUS;
  
  //sign check
  if(n1->sign != n2->sign)
    mul_sign = MINUS;
  
  //if any of the number is zero then return zero
  if(zeroNumber(n1) || zeroNumber(n2)){
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    addToNumber(mulNumber, 0);
    return mulNumber;
  }
  pushNumber(mulNumber, 0);
  node*p = n2->tail;
  int i = 0;
  while(p){
    node*q = n1->tail;
    number*temp = (number*)malloc(sizeof(number));
    initNumber(temp);
    int carry = 0;
    
    while(q){
      int mul = 0;
      mul = q->data * p->data + carry;
      int num = mul % 10;
      carry = mul / 10;
      pushNumber(temp, num);
      q = q->prev;
    }
    
    if(carry != 0)
      pushNumber(temp, carry);
    
    for(int j = 0; j < i; j++){
      addToNumber(temp, 0);
    }
    
    //Here we need not call free temp as addNumbers() natively frees and destroys the two numbers
    mulNumber = addNumbers(mulNumber, temp);
    p = p->prev;
    i++;
  }
  
  if(freeNum){
    destroyNumber(n2);
    free(n2);
  }
  
  destroyNumber(n1);
  free(n1);
  
  mulNumber->sign = mul_sign;
  return mulNumber;
}

//Function to divide and calculate the modulus of two numbers
//int RemOrQuo is to determine which operation is to be done
number *divNumbers(number *n1, number *n2, int RemOrQuo){
  
  number *temp = (number*)malloc(sizeof(number));
  number*temp2 = NULL;
  number*Q = (number*)malloc(sizeof(number));
  initNumber(temp);
  initNumber(Q);
  int i = 0;
  node*q = n2->head;
  
  //If the divisor is Zero(0), then display zero division error and exit the program
  if(zeroNumber(n1)){
    printf("ERROR: CANNOT DIVIDE BY ZERO\n");
    exit(0);
  }
  
  int Q_sign = PLUS;
  int temp_sign = PLUS;
  
  //Sign Check
  switch(n1->sign){
    case PLUS:
      switch(n2->sign){
        case PLUS:
          temp_sign = PLUS;
          Q_sign = PLUS;
          break;
        case MINUS:
          Q_sign = MINUS;
          temp_sign = PLUS;
          break;
      }
      break;
    case MINUS:
      switch(n2->sign){
        case PLUS:
          Q_sign = MINUS;
          temp_sign = MINUS;
          break;
        case MINUS:
          Q_sign = PLUS;
          temp_sign = MINUS;
          break;
      }
      break;     
  }
  
  n1->sign = PLUS;
  n2->sign = PLUS;
  
  if(compareNumbers(n1, n2) == 0){
    //If both the numbers are equal, return Quotient as 1 or remainder as 0
    if(RemOrQuo == 1){
      pushNumber(Q, 0);
      free(temp);
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      Q->sign = Q_sign;
      return Q;
    }
    pushNumber(Q, 1);
    free(temp);
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    Q->sign = Q_sign;
    return Q;
  }
  else if(compareNumbers(n1, n2) == 1){
    //This condition is when divisor is greater than dividend
    //since float numbers are not included, return 0 as quotient and dividend as remainder
    if(RemOrQuo == 1){
      node*p = n2->head;
      while(p){
        addToNumber(Q, p->data);
        p = p->next;
      }
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      free(temp);
      Q->sign = Q_sign;
      return Q;
    }
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    pushNumber(Q, 0);
    free(temp);
    Q->sign = Q_sign;
    return Q;
  }
  
  pushNumber(temp, q->data);
  
  //If n1(divisor) is more than the temp then append the next digit of n2 to temp
  while(compareNumbers(temp, n1) == -1){
    i++;
    q = q->next;
    addToNumber(temp, q->data);
  }
  
  //Keep iterating till i is less than size of dividend
  while(n2->size > i){
    int j = 0;
    if(!zeroNumber(temp)){
      int k = compareNumbers(temp, n1);
      while(k != -1){
        temp2 = temp;
        temp = subNumbers(n1, temp, 0);
        destroyNumber(temp2);
        free(temp2);
        j++;
        k = compareNumbers(temp, n1);
        if(k == 0){
          j++;
          break;
        }
      }
    }
    ++i;
    j = j % 10;
    addToNumber(Q, j);
    q = q->next;
    if(q)
      addToNumber(temp, q->data);
  }
  
  //If the Quotient is zero then push 0 to Q
  if(Q->size == 0)
    pushNumber(Q, 0);
  //We destroy the divisor as we no longer need it
  destroyNumber(n2);
  free(n2);
  if(RemOrQuo == 0){
    //Return Quotient
    destroyNumber(temp);
    destroyNumber(n1);
    free(temp);
    free(n1);
    Q->sign = Q_sign;
    return Q;
  }
  else{
    //Return Remainder
    if(compareNumbers(temp, n1) == 0){
      destroyNumber(n1);
      free(n1);
      destroyNumber(temp);
      pushNumber(temp, 0);
      return temp;
    }
    destroyNumber(Q);
    destroyNumber(n1);
    free(Q);
    free(n1);
    temp->sign = temp_sign;
    return temp;
  }
}

//code for power of a number
//implemented using repeated multiplication
number *powNumbers(number *n1, number *n2){
  number *pow = (number*)malloc(sizeof(number));
  number *temp = (number*)malloc(sizeof(number));
  initNumber(pow);
  int pow_sign = PLUS;
  
  //sign check
  if(n2->sign == MINUS)
    pow_sign = MINUS;
    
    //zero check
    if(zeroNumber(n1)){
      addToNumber(pow, 1);
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      return pow;
    }
    else if(zeroNumber(n2)){
      addToNumber(pow, 0);
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      return pow;
    }
    
    //If the sign of the exponent is zero then output is 1
    if(n1->sign == MINUS){
      pushNumber(pow, 0);
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      return pow;
    }
    
    number *UnityNumber = (number*)malloc(sizeof(number));
    initNumber(UnityNumber);
    pushNumber(UnityNumber, 1);
    
    if(compareNumbers(n1, UnityNumber) == 0){
      destroyNumber(n1);
      destroyNumber(UnityNumber);
      free(UnityNumber);
      free(temp);
      free(n1);
      return n2;
    }
    
    if(compareNumbers(n2, UnityNumber) == 0){
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      return UnityNumber;
    }
    
    addToNumber(pow, 1);
    
    while(!zeroNumber(n1)){
      pow = mulNumbers(pow, n2, 0);
      n1 = subNumbers(UnityNumber, n1, 0);
    }
    
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    free(temp);
    
    pow->sign = pow_sign;
    return pow;
}

