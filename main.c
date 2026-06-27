#include <essentials.h>
#include "src/lexer.c"

void runCode(char* fileName){
   
   if(fileName[strlen(fileName) - 4] != '.' &&
      fileName[strlen(fileName) - 3] != 'f' &&
      fileName[strlen(fileName) - 2] != 'l' && 
      fileName[strlen(fileName) - 1] != 't'
   ){
      error("Is not a floatCode file!");
      return;
   }

   char* buffer;

   FILE *pFile = fopen(fileName, READ);
   if(!pFile){
      error("File not found or not existent!");
      return;
   }

   while (fgets(buffer, 0x7ff, pFile) != NULL){
      token* tokens = tokenize(buffer);
      parse(tokens);
   }


}

int main(int argc, char *argv[])
{
   for(int i = 1; i < argc; i++){
      runCode(argv[i]);
   }
   return 0;
}