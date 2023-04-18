#include"pinfix.h"
#include<stdio.h>
#include<string.h>

//Function to calculate precedence of the operators
int precedence(char op){
  if(op == '+' || op == '-')
    return 1;
  if(op == '*' || op == '/' || op == '%')
    return 2;
  if(op == '^')
    return 3;
  return 0;
}

//Function for applying the operators to the numbers
number *applyOp(number *a, number *b, char op){
  switch(op){
    case '+':
      return addNumbers(a, b);
    case '-':
      return subNumbers(a, b, 1);
    case '*':
      return mulNumbers(a, b, 1);
    case '/':
      return divNumbers(a, b, 0);
    case '%':
      return divNumbers(a, b, 1);
    case '^':
      return powNumbers(a, b);
  }
  return NULL;
}

//Different states of the infix evaluator. Will be used for error handling.
typedef enum states{OPERATOR, NUMBER, START, END } states;

//The infix evaluator
number *infixEval(char *exp){
  
  //This block of code initialises all the variables, stacks and numbers
  states s1 = START;
  int isRightAsso = 0;
  number *n1, *n2;
  n1 = (number*)malloc(sizeof(number));
  n2 = (number*)malloc(sizeof(number));
  initNumber(n1);
  initNumber(n2);
  c_stack c;
  n_stack n;
  cinitStack(&c);
  ninitStack(&n);
  int signFlag = 0;
  int len = strlen(exp);
  int i = 0;
  
  //If user types quit then exit the program
  if(strcmp(exp, "quit") == 0){
    printf("THANK YOU!!\n");
    exit(0);
  }
  
  //Iterating through all the possible cases of characters
  for(i = 0; i < len; i++){
    if(exp[i] == ' '){
      //if the character is a space
      continue;
    }
    else if(exp[i] == '('){
      //If the character is an opening bracket (
      cpush(&c, exp[i]);
    }
    else if(isdigit(exp[i])){
      //If the character is a number
      
      //following condition will come true only if there is a number after number.
      if(s1 == NUMBER){
        printf("SYNTAX ERROR\n");
        return NULL;
      }
      
      //Reading the entire number inside following while loop and pushing it to the number ADT
      while(i < len && isdigit(exp[i])){
        addToNumber(n1, (exp[i] - '0'));
        i++;
      }
      
      //If there was a minus sign before the number then this flag will be true, hence making the sign if the number MINUS
      if(signFlag){
        n1->sign = MINUS;
        signFlag = 0;
      }
      
      //Pushing the number to the stack
      npush(&n, n1);
      n1 = (number*)malloc(sizeof(number));
      initNumber(n1);
      i--;
      s1 = NUMBER;
    }
    
    else if(exp[i] == ')'){
      //If the character is the closing bracket )
      //keep poping the number stack twice and operator stack once and evaluate till the opening brace ( is found
      while(!cisEmpty(c) && c_peek(c) != '('){
        n1 = npop(&n);
        n2 = npop(&n);
        char op = cpop(&c);
        npush(&n, applyOp(n1, n2, op));
        n1 = (number*)malloc(sizeof(number));
        n2 = (number*)malloc(sizeof(number));
        initNumber(n1);
        initNumber(n2);
      }
      //pop the opening brace (
      if(!cisEmpty(c))
        cpop(&c);
    }
    else if(exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '%' || exp[i] == '^'){
      //If the character is an operator
      //If a minus sign occurs after the evaluation has started or after a operator, then set the signFlag to 1.
      //for expressions like: -2 + 3 or 2 + - 3
      if(exp[i] == '-' && (s1 == OPERATOR || s1 == START)){
        signFlag = 1;
        continue;
      }
      //If an operator occurs immediately after an operator then throw error, like: 2 + + 3 or 13 * / 2
      if(s1 == OPERATOR){
        printf("SYNTAX ERROR AT %d(%c)\n", i, exp[i]);
        return NULL;
      }
    
      //If an exponent character is found then set the expression to follow right associativity
      if(exp[i] == '^'){
        isRightAsso = 1;
      }
     else{
        isRightAsso = 0;
      }
    
      //If the expression is right associative then we dont evaluate
      if(!isRightAsso){
        //If the operator is of less precedence than the top operator in the operator stack then evaluate the top operator
        while(cisEmpty(c) != 1 && precedence(c_peek(c)) >= precedence(exp[i])){
          n1 = npop(&n);
          n2 = npop(&n);
          char op = cpop(&c);
          npush(&n, applyOp(n1, n2, op));
          n1 = (number*)malloc(sizeof(number));
          n2 = (number*)malloc(sizeof(number));
          initNumber(n1);
          initNumber(n2);
        }
      } 
      //Push operator into operator stack and set state to operator
      cpush(&c, exp[i]);
      s1 = OPERATOR;
    }
    else{
      //If any other character than the ones specified are found, then throw an error, like: 27 & 3
      printf("INVALID SYMBOL: %c\n", exp[i]);
      return NULL;
    }
  }
  
  //Expression should always end with a number else throw an error, like: 2+3*
  if(s1 != NUMBER){
    printf("SYNTAX ERROR AT: %d(%c)\n", i, exp[i]);
    return NULL;
  }
  
  //Till the operator stack is not empty:
  //1. Pop number stack twice
  //2. Pop character stack once
  //3. Evaluate and push the result back onto the number stack
  while(cisEmpty(c) != 1){
    n1 = npop(&n);
    n2 = npop(&n);
    char op = cpop(&c);
    npush(&n, applyOp(n1, n2, op));
    n1 = (number*)malloc(sizeof(number));
    n2 = (number*)malloc(sizeof(number));
    initNumber(n1);
    initNumber(n2);
  }
  
  //Following functions ensure that every malloc() memory is accompanied by free() of memory
  destroyNumber(n1);
  destroyNumber(n2);
  free(n1);
  free(n2);
  
  s1 = END;
  
  //Return the number present in the number stack, which is indeed our answer
  return npop(&n);  
}

//This function is used to read a line from the standard input, and break when EOF(End Of File) occur
int readLine(char *line, int len){
  int i;
  char ch;
  i = 0;
  while(i < len - 1){
    ch = getchar();
    if(ch == EOF)
      return 0;
    if(ch == '\n'){
      line[i++] = '\0';
      return i - 1;
    }
    else
      line[i++] = ch;
  }
  line[len - 1] = '\0';
  return len - 1;
}

//Some Information about the program
void printInfo(){
  printf("bc 1.07.1\n");
  printf("Made by Pratima Pore, MIS:112103111\n");
  printf("Copyright 1991-1994, 1997, 1998, 2000, 2004, 2006, 2008, 2012-2017 Free Software Foundation, Inc.\n");
  printf("This is a free software with ABSOLUTE NO WARRANTY\n");
}

//The following function handles the three flags that have been implemented in the program
void getArgs(int argc, char **argv){
  //Only execute when the number of arguments is more than one
  if(argc > 1){
    //Iterate through the argv array
    for(int i = 1; i < argc; i++){
      if(strcmp(argv[i], "-h") == 0){
        // -h is for usage
        // -q is for quiet i.e info will not be printed
        // -v is to print only info and exit
        printf("usage: ./bc [options]\n");
        printf("-h\t\tprint usage and exit\n");
        printf("-q\t\tdon't print initial banner\n");
        printf("-v\t\tprint version information and exit\n");
        exit(0);
      }
      
      else if(strcmp(argv[i], "-q") == 0)
        return;
      
      else if(strcmp(argv[i], "-v") == 0){
        printInfo();
        exit(0);
      }
      
      else{
        //File Handling
        
        /*If the user enters arguments other than the flags from the program,
        we treat them as file names, try to open them and 
        evaluate the text lines from the file*/
        char exp[SIZE];
        FILE *fp;
        fp = fopen(argv[i], "r");
        
        //If the file doesnt exist or the opening fails
        if(!fp){
          printf("FILE DOES NOT EXIST OR DOES NOT HAVE RIGHT PERMISSIONS!!\n");
          exit(0);
        }
        
        //File exist or has permission
        //Read all lines of the file and execute one by one
        while(fscanf(fp, "%[^\n]%*c", exp) != EOF){
          printf("-> ");
          number *n3;
          n3 = (number*)malloc(sizeof(number));
          initNumber(n3);
          n3 = infixEval(exp);
          if(n3)
            printNumber(*n3);
          free(n3);
        }
        
        //Closing the file
        fclose(fp); 
      }
    }
  }
  printInfo();
  return;
}
