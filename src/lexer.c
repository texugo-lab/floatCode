#include <essentials.h>
#include <ctype.h>

#pragma once

#define COMMENT_COLOR "\e[3m\e[38;2;0;140;125m"

typedef enum tokenType{
   VARIABLE = 1,
   IDENTIFIER = 2,
   NUMBER = 3,
   STRING = 4,
   L_SEPARATOR = 5,
   R_SEPARATOR = 6,
   KEYWORD = 7,
   COMMENT = 8,
   INFIX_OPERATOR = 9,
   SUFFIX_OPERATOR = 10,
   K_BOOL = 11,
   CONDITIONAL = 12,

   NEWLINE = 20,
}tokenType;

typedef struct token{
   strng token;
   tokenType type;
}token;

bool needleInHaystack(char needle, strng haystack){
   for(int i = 0; i <= strlen(haystack); i++){
      if(haystack[i] == '\0') return false;
      else if(tolower(haystack[i]) == tolower(needle)) return true;
   }

   return false;
}

#define MAX_TOKENS 0x7fff

void appendToken(token* tokens, tokenType type, const strng token){
   strng copy = strdup(token);
   if(!copy) return;

   for(int i = 0; i < MAX_TOKENS; i++){
      if(tokens[i].type || tokens[i].token) continue;

      tokens[i].type = type;
      tokens[i].token = copy;
      break;
   }
}

token* tokenize(strng string){
   token* tokens = calloc(MAX_TOKENS, sizeof(token));
   if(!tokens) return NULL;

   for(int i = 0; i <= strlen(string); i++){

      bool nmbr = string[i] == 'n' && string[i + 1] == 'm' && string[i + 2] == 'b' && string[i + 3] == 'r' && string[i + 4] == ' ';
      bool isBool = string[i] == 'b' && string[i + 1] == 'o' && string[i + 2] == 'o' && string[i + 3] == 'l' && string[i + 4] == ' ';
      bool vec2 = string[i] == 'v' && string[i + 1] == 'e' && string[i + 2] == 'c' && string[i + 3] == '2' && string[i + 4] == ' ';
      bool vec3 = string[i] == 'v' && string[i + 1] == 'e' && string[i + 2] == 'c' && string[i + 3] == '3' && string[i + 4] == ' ';
      bool isString = string[i] == 's' && string[i + 1] == 't' && string[i + 2] == 'r' && string[i + 3] == 'n' && string[i + 4] == 'g' && string[i + 5] == ' ';
      bool fn = string[i] == 'f' && string[i + 1] == 'n' && string[i + 2] == ' ';
      bool isBoolValue = string[i] == 't' && string[i + 1] == 'r' && string[i + 2] == 'u' && string[i + 3] == 'e' || string[i] == 'f' && string[i + 1] == 'a' && string[i + 2] == 'l' && string[i + 3] == 's' && string[i + 4] == 'e';

      if(string[i] == ' ' || needleInHaystack(string[i], "\t")){
         continue;
      }
      else if(string[i] == '\"'){
         int j = i + 1;

         for(; j <= strlen(string) - 1; j++) if(string[j] == '\"') break;

         char stringLiteral[j - i + 1];
         for(int k = i; k <= j; k++){
            stringLiteral[k - i] = string[k];
         }

         stringLiteral[j - i + 1] = '\0';
         appendToken(tokens, STRING, stringLiteral);
         i = j;
      }
      else if(string[i] == '\n' || string[i] == ';'){
         appendToken(tokens, NEWLINE, "\n");
      }
      else if(string[i] == '#' && string[i + 1] != '-'){
         int j = i;
         for(j; string[j] != '\n' && string[j] != '\0'; j++);
         char comment[j];
         for(int k = i; k <= j; k++) comment[k - i] = string[k];
         comment[j - i] = '\0';
         appendToken(tokens, COMMENT, comment);
         appendToken(tokens, NEWLINE, "\n");
         i = j;
         continue;
      }
      
      else if(string[i] == '#' && string[i + 1] == '-'){
         int j = i + 1;
         for(j; string[j - 1] != '#' || string[j - 2] != '-'; j++);
         char comment[j - i];
         for(int k = i; k <= j; k++) comment[k - i] = string[k];
         comment[j - i] = '\0';
         appendToken(tokens, COMMENT, comment);
         appendToken(tokens, NEWLINE, "\n");
         i = j;
         continue;
      }
      else if(nmbr || isBool || vec2 || vec3 || isString || fn){
         if(isString){
            char variable[6] = {string[i], string[i + 1], string[i + 2], string[i + 3], string[i + 4], '\0'};
            appendToken(tokens, VARIABLE, variable);
            i += 5;
         }
         else if(fn){
            char variable[3] = {string[i], string[i + 1], '\0'};
            appendToken(tokens, VARIABLE, variable);
            i += 2;
         }
         else{
            char variable[5] = {string[i], string[i + 1], string[i + 2], string[i + 3], '\0'};
            appendToken(tokens, VARIABLE, variable);
            i += 4;
         }
      }
      else if(needleInHaystack(string[i], "(){}[]<>")){
         char token_str[2] = {string[i], '\0'};
         if(needleInHaystack(string[i], "([<"))  appendToken(tokens, L_SEPARATOR, token_str);
         else                                    appendToken(tokens, R_SEPARATOR, token_str);
         
      }

      else if(string[i] == '=' && string[i + 1] == '=')
      {
         char token_str[3] = {string[i], string[i + 1], '\0'};
         appendToken(tokens, CONDITIONAL, token_str);
      }
      
      else if(needleInHaystack(string[i], "!")){
         char token_str[2] = {string[i], '\0'};
         appendToken(tokens, SUFFIX_OPERATOR, token_str);
      }
      else if(needleInHaystack(string[i], "+") && needleInHaystack(string[i + 1], "+")){
         char token_str[3] = {string[i], string[i + 1], '\0'};
         i+=1;
         appendToken(tokens, SUFFIX_OPERATOR, token_str);
      }
      else if(needleInHaystack(string[i], "-") && needleInHaystack(string[i + 1], "-")){
         char token_str[3] = {string[i], string[i + 1], '\0'};
         i+=1;
         appendToken(tokens, SUFFIX_OPERATOR, token_str);
      }
      else if(needleInHaystack(string[i], "/") && needleInHaystack(string[i + 1], "/")){
         char token_str[3] = {string[i], string[i + 1], '\0'};
         i+=1;
         appendToken(tokens, INFIX_OPERATOR, token_str);
      }
      else if(needleInHaystack(string[i], "+-/*^") && needleInHaystack(string[i + 1], "=")){
         char token_str[3] = {string[i], string[i + 1], '\0'};
         i+=1;
         appendToken(tokens, INFIX_OPERATOR, token_str);
      }
      else if(needleInHaystack(string[i], "+-/*=^") && needleInHaystack(string[i + 1], "=")){
         char token_str[3] = {string[i], string[i + 1], '\0'};
         i+=1;
         appendToken(tokens, INFIX_OPERATOR, token_str);
      }
      else if(needleInHaystack(string[i], "+-/*=^")){
         char token_str[2] = {string[i], '\0'};
         appendToken(tokens, INFIX_OPERATOR, token_str);
      }

      else if(needleInHaystack(string[i], "0123456789.")){
         int j = i;
         bool isFloat = false;
         bool isNegative = false;
         for(; j <= strlen(string) - 1; j++) if(!needleInHaystack(string[j], "0123456789.")){j--; break;}

         char numberLiteral[j - i + 2];
         if(string[i - 1] == '-')
         {
            isNegative = true;
            numberLiteral[0] = '-';
         }
         for(int k = i; k <= j; k++){
            if(isNegative)numberLiteral[k - i + 1] = string[k];
            else numberLiteral[k - i] = string[k];
         }

         if(!isNegative) numberLiteral[j - i + 1] = '\0';
         else numberLiteral[j - i + 2] = '\0';
         appendToken(tokens, NUMBER, numberLiteral);
         i = j;
      }

      else if(isBoolValue){
         char trueKeyword[5] = {string[i], string[i + 1], string[i + 2], string[i + 3], '\0'};
         char falseKeyword[6] = {string[i], string[i + 1], string[i + 2], string[i + 3], string[i + 4], '\0'};

         if(string[i] == 'f' && string[i + 1] == 'a' && string[i + 2] == 'l' && string[i + 3] == 's' && string[i + 4] == 'e'){
            appendToken(tokens, K_BOOL, falseKeyword);
            i += 5;
            continue;
         }
         i += 4;
         appendToken(tokens, K_BOOL, trueKeyword);
      }
      else if(needleInHaystack(string[i], "_abcdefghijklmnopqrstuvwxyz")){
         int j = i;
         for(; j <= strlen(string); j++) if(!needleInHaystack(string[j], "_abcdefghijklmnopqrstuvwxyz")) break;

         char word[j - i];
         for(int k = i; k <= j; k++){
            word[k - i] = string[k];
         }

         word[j - i] = '\0';
         appendToken(tokens, IDENTIFIER, word);
         i = j - 1;
      }
   }

   return tokens;
}
