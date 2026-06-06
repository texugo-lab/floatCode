#include <essentials.h>
#include <ctype.h>

typedef enum tokenType{
   VARIABLE = 0,
   NAME = 1,
   NUMBER = 2,
   STRING = 3,
   PARENTHESES = 4,
   OPERATOR = 5,
   KEYWORD = 6
}tokenType;

typedef struct token{
   char* token;
   tokenType type;
}token;

bool needleInHaystack(char needle, char* haystack){
   for(int i = 0; i <= strlen(haystack); i++){
      if(haystack[i] == '\0') return false;
      else if(tolower(haystack[i]) == tolower(needle)) return true;
   }

   return false;
}

#define MAX_TOKENS 0x7ff

void appendToken(token* tokens, tokenType type, const char* token){
   char* copy = strdup(token);
   if(!copy) return;

   for(int i = 0; i < MAX_TOKENS; i++){
      if(tokens[i].type || tokens[i].token) continue;

      tokens[i].type = type;
      tokens[i].token = copy;
      break;
   }
}

token* tokenize(char* string){
   token* tokens = calloc(MAX_TOKENS, sizeof(token));
   if(!tokens) return NULL;

   for(int i = 0; i <= strlen(string); i++){

      bool nmbr = string[i] == 'n' && string[i + 1] == 'm' && string[i + 2] == 'b' && string[i + 3] == 'r' && string[i + 4] == ' ';
      bool boolean = string[i] == 'b' && string[i + 1] == 'o' && string[i + 2] == 'o' && string[i + 3] == 'l' && string[i + 4] == ' ';
      bool vec2 = string[i] == 'v' && string[i + 1] == 'e' && string[i + 2] == 'c' && string[i + 3] == '2' && string[i + 4] == ' ';
      bool strg = string[i] == 's' && string[i + 1] == 't' && string[i + 2] == 'r' && string[i + 3] == 'n' && string[i + 4] == 'g' && string[i + 5] == ' ';
      bool fn = string[i] == 'f' && string[i + 1] == 'n' && string[i + 2] == ' ';
      bool isBoolean = string[i] == 't' && string[i + 1] == 'r' && string[i + 2] == 'u' && string[i + 3] == 'e' ||
                       string[i] == 'f' && string[i + 1] == 'a' && string[i + 2] == 'l' && string[i + 3] == 's' && string[i + 4] == 'e';

      if(string[i] == ' '){
         continue;;
      }
      else if(needleInHaystack(string[i], "(){}[]")){
         // printf("PARENTHESES: %c\n", string[i]);
         char token_str[2] = {string[i], '\0'};
         appendToken(tokens, PARENTHESES, token_str);
      }
      else if(needleInHaystack(string[i], "+-/*=!^")){
         // printf("OPERATOR: %c\n", string[i]);
         char token_str[2] = {string[i], '\0'};
         appendToken(tokens, OPERATOR, token_str);
      }
      else if(needleInHaystack(string[i], "0123456789.,")){
         // printf("NUMBER: %c\n", string[i]);
         for(int j = i + 1; j < strlen(string); j++){
            printf("%c\n", string[j]);
            if(!needleInHaystack(string[j], "0123456789.,")){
               j++;
               char numberLiberal[j - i];

               for(int k = 0;k <= j - i; k++){
                  numberLiberal[k] = string[k + i];
               }
               appendToken(tokens, NUMBER, numberLiberal);
               i = j; 
               break;
            }
         }
         char token_str[2] = {string[i], '\0'};
         appendToken(tokens, NUMBER, token_str);
      }
      else if(isBoolean){
         char trueKeyword[5] = {string[i], string[i + 1], string[i + 2], string[i + 3]};
         char falseKeyword[6] = {string[i], string[i + 1], string[i + 2], string[i + 3], string[i + 4]};

         if(string[i] == 'f' && string[i + 1] == 'a' && string[i + 2] == 'l' && string[i + 3] == 's' && string[i + 4] == 'e'){
            // printf("KEYWORD: %s\n", falseKeyword);
            appendToken(tokens, KEYWORD, falseKeyword);
            continue;
         }
         // printf("KEYWORD: %s\n", trueKeyword);
         appendToken(tokens, KEYWORD, trueKeyword);
      }
      else if(nmbr || boolean || vec2){
         char variable[5] = {string[i], string[i + 1], string[i + 2], string[i + 3]};
         // printf("VARIABLE: %s\n", variable);
         appendToken(tokens, VARIABLE, variable);
         i += 4;
      }
      else if(strg){
         char variable[6] = {string[i], string[i + 1], string[i + 2], string[i + 3], string[i + 4]};
         // printf("VARIABLE: %s\n", variable);
         appendToken(tokens, VARIABLE, variable);
         i += 5;
      }
      else if(fn){
         char variable[6] = {string[i], string[i + 1], string[i + 2]};
         // printf("VARIABLE: %s\n", variable);
         appendToken(tokens, VARIABLE, variable);
         i += 2;
      }
      else if(string[i] == '\"'){
         for(int j = i + 1; j < strlen(string); j++){
            printf("%c\n", string[j]);
            if(string[j] == '\"'){
               j++;
               char stringLiberal[j - i];

               for(int k = 0;k <= j - i; k++){
                  stringLiberal[k] = string[k + i];
               }
               appendToken(tokens, STRING, stringLiberal);
               i = j;
               break;
            }
         }
      }
      else if(needleInHaystack(string[i], "\n\t")){
         continue;
      }
      else{
         printf("%c\n", string[i]);
      }
   }
   for(int i = 0; i < MAX_TOKENS; i++){
      if(tokens[i].token) printf("%s ", tokens[i].token);
   }
   printf("\n");

   return tokens;
}
