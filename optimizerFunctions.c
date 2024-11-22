#include "optimizerFunctions.h"
#include "optimizerFunctions2.h"

/* Get the datatype of the variable being declared */
char* getDataType(const char* line)
{
     size_t buffer_size = 10;
     char* ptr = strstr(line, "(");
     char* datatype = (char*)malloc(buffer_size * sizeof(char));
     ptr++;
     size_t length = 0;
     while(isalpha(ptr[0]))
     {
          datatype[length++] = ptr[0];
          ptr++;
     }
     return datatype;
}

/* Gets the value that the variable in the for loop is being initialized to */
int getStart(char *line)
{
     int start = 0;
     char *ptr = strstr(line, ";"); //go to var declaration
     start = (--ptr)[0] - '0'; //starting value of var
     start = 0;
     int count2 = 0;
     while(isdigit(ptr[0])){ //iterates from least to most significant digit in declaration
          start += (((ptr)--[0] - '0') * pow(10, count2++)); //get limiting variable
     }
     return start;
}

/* Gets the name of the variable being declared in the for loop */
char* getVar(const char *line)
{    
     size_t buffer_size = 10;  // Initial size of the buffer
     char* var = (char*)malloc(buffer_size * sizeof(char));
     if (var == NULL)
     {
        printf("Memory allocation failed.\n");
        return NULL;
     }

     size_t length = 0;
     char *ptr = strstr(line, "("); //go to beginning of declaration
     while(ptr[0] == ' '|| ptr[0] == '(') ptr++; //increment until variable datatype is found
     while(isalpha(ptr[0])) ptr++; //increment beyond datatype
     while(ptr[0] == ' ') ptr++; //increment until var name is found
     while(ptr[0] != ' ' && ptr[0] != '=') //increment through varable name
     {
          if (length + 1 >= buffer_size) //reallocate memory if more is needed
          {
               buffer_size *= 2;  // Double the buffer size
               var = (char*)realloc(var, buffer_size * sizeof(char));
               if (var == NULL)
               {
                   printf("Memory reallocation failed.\n");
                   return NULL;
               }
          }
          var[length++] = ptr[0];
          ptr++;
     }
     var[length] = '\0';
     return var;
}

/* Get the comparing operator in the condition of the for loop */
char* getComparator(const char* line)
{
     char *ptr = strstr(line, ";"); //move pointer back to end of var declaration
     ptr++; //increment beyond semicolon
     while(ptr[0] == ' ' || isalpha(ptr[0])) ptr++; //increment until comparator is reached

     size_t buffer_size = 10;
     char* var = (char*)malloc(buffer_size * sizeof(char));
     if (var == NULL)
     {
        printf("Memory allocation failed.\n");
        return NULL;
     }

     size_t length = 0;
     while(ptr[0] != ' ')
     {
          var[length++] = ptr[0];
          ptr++;
     }
     var[length] = '\0';
     return var;
}

/* Get the ending value in the condition of the for loop */
int getEnd(char* line)
{
     char *ptr = strstr(line, ";"); //move pointer to end of variable initialization
     ptr++;
     ptr = strstr(ptr, ";");
     while(!isdigit(ptr[0])) ptr--; //decrement until value is reached
     int end = 0;
     int count = 0;
     while(isdigit(ptr[0])){
          end += (((ptr)--[0] - '0') * pow(10, count++)); //get limiting variable
     }
     return end;
}

char* getIncDec(const char* line)
{
     char* ptr = strstr(line, ")"); //move pointer to end of loop declaration
     while(ptr[0] == ' ') ptr--;
     size_t buffer_size = 10;
     char* alteration = (char*)malloc(buffer_size * sizeof(char));
     if(alteration == NULL)
     {
          printf("Memory allocation failed.\n");
          return NULL;
     }
     while(!isalpha(ptr[0])) ptr--; //go to front of alteration
     size_t length = 0;
     ptr++;
     while(ptr[0] != ' ' && ptr[0] != ')')
     {
          alteration[length++] = ptr[0];
          ptr++;
     }
     alteration[length] = '\0';
     return alteration;
}

int getAlterationValue(char *alteration)
{
     int increment = 0;
     int length = strlen(alteration);
     if(isdigit(alteration[length - 1])){
          int count = 0;
          while(isdigit(alteration[length - 1]))
          {
               increment += ((alteration[length - 1] - '0') * pow(10, count++));
               length--;
          }
          length--; //iterate beyond = sign in += or -=
          if(alteration[length - 1] == '-')
          {
               increment *= -1;
          }
     }else if(alteration[length-1] == '+'){
          increment = 1;
     }else if(alteration[length-1] == '-'){
          increment = -1;
     }else{
          printf("Unrecognized increment.\n");
          return 0;
     }
     return increment;
}

void unrollBy2(int lineNum, FILE* fp, FILE* optimized, char* datatype, char* var, int start, char* comparator, int end, char* incrementOp, int increment)
{
     if(increment < 0) start -= 2; //if increment is negative, ensure that initialized value is dec instead of inc

     fprintf(optimized, "%s %s;\n", datatype, var);
     fprintf(optimized, "for(%s = %d; %s %s %d; %s %s %d)\n", 
          var, ++start, var, comparator, end, var, incrementOp, abs((increment*2)));
     fprintf(optimized, "{\n"); //Beginning of loop
     printf("Now optimizing: for(%s %s = %d; %s %s %d; %s %s %d)\n",
          datatype, var, ++start, var, comparator, end, var, incrementOp, abs((increment*2)));

     long startingPoint;
     int checkStart = 1;
     char *line = NULL;
     size_t len = 0;
     ssize_t charsRead;
     int bracketCount = 0; //increment for every new loop/statement that is found
     int lengthRead;

     /* Iterate through loop body */
     while(!feof(fp))
     {
          if((charsRead = getline(&line, &len, fp)) != -1)
          {
               ++lineNum;
               char *linePtr = NULL;
               if(checkStart == 1)
               {
                    startingPoint = ftell(fp);
                    checkStart = 0;
               }
               
               if((linePtr = strstr(line, "for")) != NULL) //if another for loop is found
               {
                    //recursive step??
               }else if((linePtr = strstr(line, "}")) != NULL) //if end of current for loop is reached
               {
                    if((linePtr = strstr(line, "{")) == NULL)
                    {
                         bracketCount--;
                         if(bracketCount == 0)
                         {
                              lineNum++;
                              break;
                         }
                    }
               }else if((linePtr = strstr(line, "{")) != NULL) //if another set of brackets is introduced
               {
                    if((linePtr = strstr(line, "}")) == NULL)
                    {
                         bracketCount++;
                    }
               }else //if it's just another line
               {
                    if((linePtr = strstr(line, var)) != NULL) //if var in loop is found in line
                    {
                         linePtr--;
                         /* Ensure that the var was correctly identified */
                         if(!isalpha(linePtr[0]) && !isdigit(linePtr[0]))
                         {
                              int varIndex = strlen(line) - strlen(linePtr);
                              linePtr += strlen(var) + 1;
                              if(!isalpha(linePtr[0]) && !isdigit(linePtr[0]))
                              {
                                   /* Print line along with inc/decremented line */
                                   fprintf(optimized, "%s", line);
                                   char* incrementedLine = incrementLine(line, var, increment, varIndex);
                                   fprintf(optimized, "%s", incrementedLine);
                              }
                         }
                    }else{ //duplicate if just another line
                         fprintf(optimized, "%s", line);
                         fprintf(optimized, "%s", line);
                    }
               }
          }
     }
     fprintf(optimized, "}\n");

     /* Iterate once more if loop has odd number of iterations */
     if(abs((end - start))%2 == 0) return;
     long setBack = ftell(fp) - startingPoint;
     int newStart = end - ((end - start) % 2);
     char newIncrementOp;
     fseek(fp, -(ftell(fp) - startingPoint), SEEK_CUR); //pushes pointer back to beginning of for loop for reiteration
     if(increment >= 0)
     {
          newIncrementOp = '+';
     }else
     {
          newIncrementOp = '-';
     }

     /* Create for loop with remaining number of iterations and reiterate through loop */
     fprintf(optimized, "/* Remainding Iterations */\n");
     fprintf(optimized, "for(%s = %d; %s %s %d; %s %c= 1)\n", var, newStart, var, comparator, end, var, newIncrementOp);
     fprintf(optimized, "{\n");
     while((charsRead = getline(&line, &len, fp)) != -1)
     {
          if(line[0] == '{') continue;
          if(line[0] == '}') break;
          fprintf(optimized, "%s", line);
     }
     fprintf(optimized, "}\n");
     return;
}