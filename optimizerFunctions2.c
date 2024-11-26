#include "optimizerFunctions2.h"

/* Returns a line with the looping variable being incremented / decremented by the given increment */
char* incrementLine(char* line, char* var, int increment, int varIndex, int factor)
{
     printf("Increment in inc: %d\n", increment);
     char* ptr = line;
     size_t buffer_size = strlen(line)+10;
     size_t length = 0;
     char* incrementedLine = (char*)malloc(buffer_size * sizeof(char));
     if(incrementedLine == NULL)
     {
          printf("Memory allocation failed.\n");
          return NULL;
     }

     /* Loop begins */
     int count = 0;
     varIndex++;
     while(count < varIndex) //copy line up until var is reached
     {
          incrementedLine[length++] = ptr[count++]; 
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
     }
     //copy in var
     for(int i = 0; i < strlen(var); i++)
     {
          incrementedLine[length++] = var[i];
     }
     incrementedLine[length++] = ' ';
     ptr += varIndex + strlen(var); // move pointer past var

     if(factor == 0 || factor == 1)
     {
          if(increment >= 0)
          {
               incrementedLine[length++] = '-';
          }else{
               incrementedLine[length++] = '+';
          }
     }else{
          if(increment >= 0)
          {
               incrementedLine[length++] = '+';
          }else{
               incrementedLine[length++] = '-';
          }
     }
     incrementedLine[length++] = ' ';
     //convert increment to string
     //char buffer[32];
     char *buffer = (char*)malloc(1 * sizeof(int));
     sprintf(buffer, "%d", abs(increment));
     //concatenating increment onto main string
     strcat(incrementedLine, buffer);
     length = strlen(incrementedLine);
     /* End Loop */
     
     char* testPtr;
     while((testPtr = strstr(ptr, var)) != NULL){
          int indices = strlen(ptr) - strlen(testPtr); //number of places to iterate to to reach next var
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
          while(indices > 0)
          {
               incrementedLine[length++] = ptr[0];
               ptr++;
               indices--;
          }
          //once var is reached
          for(int i = 0; i < strlen(var); i++)
          {
               incrementedLine[length++] = var[i];
          }
          incrementedLine[length++] = ' ';
          ptr += varIndex + strlen(var); // move pointer past var
          if(increment >= 0)
          {
               incrementedLine[length++] = '-';
          }else{
               incrementedLine[length++] = '+';
          }
          incrementedLine[length++] = ' ';

          char *buffer = (char*)malloc(1 * sizeof(int));
          sprintf(buffer, "%d", abs(increment));
          strcat(incrementedLine, buffer);
          length = strlen(incrementedLine);
     }

     while(ptr[0] != '\0' && ptr[0] != ';')
     {
          incrementedLine[length++] = ptr[0];
          ptr++;
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
     }
     incrementedLine[length++] = ';';
     incrementedLine[length] = '\n';
     return incrementedLine;
}
