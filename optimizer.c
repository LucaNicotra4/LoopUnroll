/* 
Assignment 4 Computer Architecture 
Written by Luca Nicotra
18 November 2024
*/

/*
argc - factor to unroll for loops by
     0 - unroll loops by a factor of 2
     1 - unroll loops by a factor of 4
     2 - fully unroll loops
argv - C code source file
*/

#include "optimizerFunctions.h"
#include <stdio.h>

/* Function Signitures */
int getStart(char*);
char* getVar(const char*);
char* getComparator(const char*);
int getEnd(char*);
char* getIncDec(const char*);

/* Define Comparators */
#define LESS_THAN 0
#define GREATER_THAN 1
#define LESS_OR_EQ 2
#define GREATER_OR_EQ 4
#define EQUAL 5

int main(int argc, char *argv[])
{    
     /* Check for proper arguments */
     int factor = atoi(argv[1]);
     if(factor < 0 || factor > 2 || argc != 3)
     {
          printf("Invalid input for argc: %d\n", argc);
          printf("0: %s / 1: %s / 2: %s", argv[0], argv[1], argv[2]);
          return 0;
     }

     /* Initialize write file */
     FILE *optimized = NULL;
     char fileName[80];
     strcpy(fileName, argv[2]);
     strcat(fileName, "_modified");
     optimized = fopen(fileName, "w");
     if(optimized == NULL)
     {
          printf("Error opening write file\n");
          return 0;
     }

     /* Open read file and begin unrolling */
     FILE *fp = NULL;
     if((fp = fopen(argv[2], "r")) != NULL)
     {
          char *line = NULL;
          int lineNum = 0;
          size_t len = 0;
          ssize_t charsRead;

          /* Read input file line by line */
          while(!feof(fp))
          {    
               /* Eventually need to make this its own function for recursive purposes (for nested loops) */
               if((charsRead = getline(&line, &len, fp)) != -1)
               {
                    // printf("Retrieved line of length %zu:\n", charsRead);
                    // printf("%d) %s\n", ++lineNum, line);
                    ++lineNum;
                    char *forPtr = NULL;

                    /* If a for loop is found */
                    if((forPtr = strstr(line, "for")) != NULL)
                    {
                         printf("Found for loop on line %d here: %s\n", lineNum, forPtr);
                         /* Piece up loop */
                         /* Chunk 1 (Declaration) */
                         char *datatype = getDataType(line);
                         int start = getStart(line);
                         char *var = getVar(line);
                         //printf("%s = %d\n", var, start);

                         /* Chunk 2 (Condition) */
                         char *comparator = getComparator(line);
                         int end = getEnd(line);
                         //printf("%s %s %d\n", var, comparator, end);

                         /* Chunk 3 (Inc/Dec) */
                         char *alteration = getIncDec(line);
                         //printf("%s%s\n", var, alteration);
                         
                         /* Convert alteration to digit value (positive for increment, negative for decrement) */
                         int increment = getAlterationValue(alteration);
                         char* incrementOp;
                         if(increment >= 0)
                         {
                              incrementOp = "+=";
                         }else{
                              incrementOp = "-=";
                         }

                         if(factor == 0){ //factor of 2
                              unrollBy2(lineNum, fp, optimized, datatype, var, start, comparator, end, incrementOp, increment);
                              //printf("Start: %d", start);
                         }else if(factor == 1){ //factor of 4

                         }else if(factor == 2){ //full unroll

                         }
                    }else
                    {
                         //if not a for loop, just copy line
                         fprintf(optimized, "%s", line);
                    }
               }
               if(line)
               {
                    free(line), line = NULL; //garbage collector
               }
          }
          fclose(fp), fp = NULL;
     } else {
          printf("Error opening file %s\n", argv[2]);
     }
} //end of main


