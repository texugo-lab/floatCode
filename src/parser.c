#include <essentials.h>
#include "lexer.c"

#pragma once

int getBindPower(strng operator)
{
	if(!operator)
		return 0;
	else if(strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0)
		return 1;
	else if(strcmp(operator, "*") == 0 || strcmp(operator, "/") == 0)
		return 2;
	else if(strcmp(operator, "^") == 0 || strcmp(operator, "//") == 0)
		return 3;
	else
		return 0;
}

token* parse(token* tokens, bool assembly, bool compile)
{
   for(int i = 0; i < MAX_TOKENS; i++)
	{

		if(!tokens[i].type) continue;
		if(tokens[i].type == COMMENT) printf("(" COMMENT_COLOR "%s\e[0m)->COMMENT", tokens[i].token);
		else if(tokens[i].type == NEWLINE)
		{
			printf("\n");
			continue;
		}

		else printf("(%s)->", tokens[i].token);
	
		switch (tokens[i].type)
		{
			case VARIABLE:
				printf("VARIABLE");
				break;
			case IDENTIFIER:
				printf("IDENTIFIER");
				break;
			case NUMBER:
				printf("NUMBER");
				break;
			case STRING:
				printf("STRING");
				break;
			case L_SEPARATOR:
				printf("L_SEPARATOR");
				break;
			case R_SEPARATOR:
				printf("R_SEPARATOR");
				break;
			case KEYWORD:
				printf("KEYWORD");
				break;
			case K_BOOL:
				printf("BOOL");
				break;
			case CONDITIONAL:
				printf("CONDITIONAL");
				break;
			case INFIX_OPERATOR:
				printf("INFIX_OPERATOR");
				break;
			case SUFFIX_OPERATOR:
				printf("SUFFIX_OPERATOR");
				break;
			default:
				break;
		}
	}
	printf("\n");
}

char* terminalParse(token* tokens)
{
	strng finalMessage = "";

	for(int i = 0; i < MAX_TOKENS; i++)
	{
		if(!tokens[i].token || !tokens[i].type) continue;

		if(tokens[i].type == NUMBER)
		{
			strng tokenBefore = tokens[i - 1].token;
			strng tokenAfter = tokens[i + 1].token;

			int bindPowerBefore = getBindPower(tokenBefore);
			int bindPowerAfter = getBindPower(tokenAfter);

			if(!tokenAfter)
				tokenAfter = "\0";
			if(!tokenBefore)
				tokenBefore = "\0";

			finalMessage = textFormat("%s   %s", finalMessage, (bindPowerBefore >= bindPowerAfter) ? tokenBefore : tokenAfter);
		}
	}

	return finalMessage;
}
