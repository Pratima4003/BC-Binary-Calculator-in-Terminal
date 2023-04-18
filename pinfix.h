#define SIZE 2048

#include"pfunction.h"
#include"plist.h"
#include"pstack.h"
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


number *infixEval(char *expr);
int readLine(char *line, int len);
void getArgs(int argc, char **argv);
