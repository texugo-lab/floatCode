#include <essentials.h>
#include "src/lexer.c"
#include "src/parser.c"
#include "src/executer.c"
#include "src/terminal.c"

#define NOT_FLOATCODE_FILE_EXIT_CODE 1
#define NOT_FLOATCODE_FILE_EXIT_MESSAGE "Is not Float Code file"
#define FILE_NOT_FOUND_EXIT_CODE 2
#define FILE_NOT_FOUND_EXIT_MESSAGE "Error opening file"

#define FLOAT_CODE_TERMINAL "-term"
#define FLOAT_CODE_HELP "-h"

void printHelp()
{
   printf("usage: floatCode [-h | --help]\n");
   printf("                 [-term | Float Code Terminal]\n");
   printf("                 [fileName] [-asm | Generate Assembly] [outputFileName]\n");
}

int main(int argc, strng argv[])
{
   {
      if(strcmp(argv[1], FLOAT_CODE_HELP) == 0)
      {
         printHelp();
         return 0;
      }
      else if(strcmp(argv[1], FLOAT_CODE_TERMINAL) == 0)
      {
         initTerminal();
         return 0;
      }

      bool assembly = false;
      bool compile = true;

      strng fileName = argv[1];

      if(fileName[strlen(fileName) - 4] != '.' &&
         fileName[strlen(fileName) - 3] != 'f' &&
         fileName[strlen(fileName) - 2] != 'l' &&
         fileName[strlen(fileName) - 1] != 't')
      {
         fprintf(stderr, NOT_FLOATCODE_FILE_EXIT_MESSAGE "\n");
         exit(NOT_FLOATCODE_FILE_EXIT_CODE);
      }
      strng outputFileName = argv[argc - 1];

      if(argc <= 2 || strcmp(argv[2], "-asm") == 0){
         outputFileName = getFileNameNoExt(fileName);
      }

      if(!getFileExtension(outputFileName)){
         outputFileName = textFormat("%s.exe", outputFileName);
      }

      printf("%s\n", fileName);
      for(int i = 2; i < argc; ++i){
         if(strcmp(argv[i], "-asm") == 0 || strcmp(argv[i], "-asm\n") == 0)
         {
            assembly = true;
            compile = false;
         }
         printf("%s\n", argv[i]);
      }
      printf("%s\n", outputFileName);

      FILE *pFile = fopen(fileName, READ);

      if(!pFile){
         fprintf(stderr, FILE_NOT_FOUND_EXIT_MESSAGE "\n");
         exit(FILE_NOT_FOUND_EXIT_CODE);
      }

      if(assembly)     printf("Generating assembly...\n");
      else if(compile) printf("Compiling...\n");

      strng fileText = LoadFileText(fileName);

      int count = 0;

      token* tokens = tokenize(fileText);
      parse(tokens, assembly, compile);
   }
   
   return 0;
}
