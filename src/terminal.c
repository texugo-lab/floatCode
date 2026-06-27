#include <essentials.h>
#include "lexer.c"
#include "parser.c"
#include <conio.h>

#pragma once

void printPrompt(strng promptCharacter, strng text)
{
		printf("\e[0;32m%s\e[0m", promptCharacter);
		printf(text);
}

void setCursor(int x, int y)
{
	printf("\033[%d;%dH", y,x);
}

void clearLine()
{
	printf("\x1B[2K");
}

void clearScreen()
{
	for(int i = 0; i <= 6; i++)
	{
		setCursor(0, i);
		clearLine();
	}
}

int getChar()
{
	int Char = _getch();
	if(Char == 0 || Char == 0xE0)
	{
		Char += 255;
	}
	return Char;
}

strng removeLastChar(strng str)
{
	strng newStr = str;

	int len = strlen(str);
	for(int i = 0; i < len - 4; i++)
	{
		newStr[i] = str[i];
	}

	return newStr;
}

void initTerminal()
{
	clearScreen();

	int seconds = 0; 

	strng text = "\0";

	strng promptSymbol = ">";
	strng result;

	int character = 0;

	while (true)
	{
		setCursor(0, 1);
		clearLine();
		printf("Float Code Terminal");

		setCursor(0, 3);
		clearLine();
		result = terminalParse(tokenize(text));

		setCursor(0, 2);
		clearLine();
		printPrompt(promptSymbol, textFormat("%s---%d", text, character, strlen(text)));

		character = getChar();

		if(character == 27)
		{
			clearScreen();
			setCursor(0, 0);
			return;
		}
		else if(character == 8 || character == 77 || character == 75 || character == 72 || character == 80)
		{
			text = textFormat(text);
			text[strlen(text) - 1] = '\0';
		}
		else if(character == 13);
		else text = textFormat("%s%c", text, character);
	}
}
