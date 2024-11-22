#ifndef OPTIMIZER_FUNCTIONS_H
#define OPTIMIZER_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

char* getDataType(const char*);
int getStart(char*);
char* getVar(const char*);
char* getComparator(const char*);
int getEnd(char*);
char* getIncDec(const char*);
int getAlterationValue(char*);
void forLoopFound(char*, char*, int, int, FILE*, FILE*);
void unroll(int, FILE*, FILE*, char*, char*, int, char*, int, char*, int, int);

#endif