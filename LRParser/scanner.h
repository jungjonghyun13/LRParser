#pragma once
#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define FNAMESIZE 100
#define NO_KEYWORD 7
#define ID_LENGTH 12
extern FILE *sourceFile;

enum tsymbol {	//심볼의 의미
	tnull = -1,
	tnot, tnotequ, tremainder, tremAssign, tident, tnumber,
	/* 0          1            2         3            4          5     */
	tand, tlparen, trparen, tmul, tmulAssign, tplus,
	/* 6          7            8         9           10         11     */
	tinc, taddAssign, tcomma, tminus, tdec, tsubAssign,
	/* 12         13          14        15           16         17     */
	tdiv, tdivAssign, tsemicolon, tless, tlesse, tassign,
	/* 18         19          20        21           22         23     */
	tequal, tgreat, tgreate, tlbracket, trbracket, teof,
	/* 24         25          26        27           28         29     */
	//   ...........    word symbols ................................. //

	/* 30     31    32    33      34    35     */
	tconst, telse, tif, tint, treturn, tvoid,

	/* 36      37     38    39                             */
	twhile, tlbrace, tor, trbrace,

	///* 40      41     42    43      44    45       46     47      48  */
	//tchar, tdouble, tfor, tdo, tgoto, tswitch, tcase, tbreak, tdefault,
	///*49     50         51		  52		53	*/
	//tcolon, tampersand, tschar, tstring, trealnumber
};

char *tokenName[] = {
	"!",        "!=",      "%",       "%=",     "%ident",   "%number",
	/* 0          1           2         3          4          5        */
	"&&",       "(",       ")",       "*",      "*=",       "+",
	/* 6          7           8         9         10         11        */
	"++",       "+=",      ",",       "-",      "--",	    "-=",
	/* 12         13         14        15         16         17        */
	"/",        "/=",      ";",       "<",      "<=",       "=",
	/* 18         19         20        21         22         23        */
	"==",       ">",       ">=",      "[",      "]",        "eof",
	/* 24         25         26        27         28         29        */
	//   ...........    word symbols ................................. //
	/* 30         31         32        33         34         35        */
	"const",    "else",     "if",      "int",     "return",  "void",
	/* 36         37         38        39                              */
	"while",    "{",        "||",       "}"
	///* 40		  41		 42		   43         44		 45			46			47			48  */
	//"char",		"double",	"for",		"do",	  "goto",	 "switch",	"case",		"break",	"default",
	///*49		  50		 51				52					53			*/
	//":",		"&",		"%char",	"%string",		"%real_num"
};

char *keyword[NO_KEYWORD] = {
	"const",  "else",    "if",    "int",    "return",  "void",    "while"
	//"char",		"double",	"for",		"do",	  "goto",	 "switch",	"case",		"break",	"default"
};

enum tsymbol tnum[NO_KEYWORD] = {
	tconst,    telse,     tif,     tint,     treturn,   tvoid,     twhile
	//tchar, tdouble, tfor, tdo, tgoto, tswitch, tcase, tbreak, tdefault
};


struct tokenType {
	int number;			//토큰 번호
	union {
		char id[ID_LENGTH];
		int num;
	} value;
};


int superLetter(char ch)	//isalpha 알파벳인지 검사 
{
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}

int superLetterOrDigit(char ch)	//( al num이면 알파벳이거나 숫자) 
{
	if (isalnum(ch) || ch == '_') return 1;
	else return 0;
}

int getNumber(char firstCharacter)
{
	int num = 0;
	int value;
	char ch;
	// decimal
	ch = firstCharacter;
	do {
		num = 10 * num + (int)(ch - '0');
		ch = fgetc(sourceFile);
	} while (isdigit(ch));
	ungetc(ch, sourceFile); /*  retract  */
	return num;
}

void lexicalError(int n)
{
	printf("\n *** Lexical Error : ");
	switch (n) {
	case 1: printf("an identifier length must be less than 12.\n");
		break;
	}
}
void PrintTokenState(struct tokenType t) {
	if (30 <= t.number && t.number <= 36 || 40 <= t.number && t.number <= 45) {//키워드	
		printf("\tToken -----> %9s  (%9d, %9d)\n", tokenName[t.number], t.number, 0);
	}
	else if (t.number == 4) {	//변수
		printf("\tToken -----> %9s  (%9d, %9s)\n", tokenName[t.number], t.number, t.value.id);
	}
	else if (t.number == 5) {	//상수(정수)
		printf("\tToken -----> %9s  (%9d, %9d)\n", tokenName[t.number], t.number, t.value.num);
	}
	else if (0 <= t.number && t.number < 29) {	//기본 심볼, 연산자
		printf("\tToken -----> %9s  (%9d, %9d)\n", tokenName[t.number], t.number, 0);
	}
	else if (37 <= t.number && t.number <= 39) {//기본 심볼, 연산자
		printf("\tToken -----> %9s  (%9d, %9d)\n", tokenName[t.number], t.number, 0);
	}
	else if (t.number == 29) {//EOF
		printf("\tToken -----> %9s  (%9d, %9d)\n", tokenName[t.number], t.number, 0);
	}
}
struct tokenType scanner()
{
	int i, index;
	char ch, id[100];//임시로 100으로 잡음.
	struct tokenType token;
	token.number = tnull;

	do {
		while (isspace(ch = fgetc(sourceFile)));	// state 1: skip blanks
		if (superLetter(ch)) { // identifier or keyword
			i = 0;
			do {
				if (i < ID_LENGTH) id[i++] = ch;
				//else
				//	break;
				ch = fgetc(sourceFile);
			} while (superLetterOrDigit(ch));
			if (i >= ID_LENGTH) lexicalError(1);
			id[i] = '\0';
			ungetc(ch, sourceFile);  //  retract
									 // find the identifier in the keyword table
			for (index = 0; index < NO_KEYWORD; index++)
				if (!strcmp(id, keyword[index])) 
					break;
			if (index < NO_KEYWORD)    // found, keyword exit
				token.number = tnum[index];
			else {                     // not found, identifier exit
				token.number = tident;
				strcpy(token.value.id, id);
			}
		}  // end of identifier or keyword
		else if (isdigit(ch)) { // number
			token.value.num = getNumber(ch);
			token.number = tnumber;
		}
		else switch (ch){
		case '/':
			ch = fgetc(sourceFile);
			if (ch == '*')			// text comment
				do {
					while (ch != '*') ch = fgetc(sourceFile);
					ch = fgetc(sourceFile);
				} while (ch != '/');
			else if (ch == '/')		// line comment
				while (fgetc(sourceFile) != '\n');
			else if (ch == '=')  token.number = tdivAssign;
			else {
				token.number = tdiv;
				ungetc(ch, sourceFile); // retract
			}
			break;
		case '!':
			ch = fgetc(sourceFile);
			if (ch == '=')  token.number = tnotequ;
			else {
				token.number = tnot;
				ungetc(ch, sourceFile); // retract
			}
			break;
		case '%':
			ch = fgetc(sourceFile);
			if (ch == '=') {
				token.number = tremAssign;
			}
			else {
				token.number = tremainder;
				ungetc(ch, sourceFile);
			}
			break;
		case '&':
			ch = fgetc(sourceFile);
			if (ch == '&')  token.number = tand;
			else {
				lexicalError(2);
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '*':
			ch = fgetc(sourceFile);
			if (ch == '=')  token.number = tmulAssign;
			else {
				token.number = tmul;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '+':
			ch = fgetc(sourceFile);
			if (ch == '+')  token.number = tinc;
			else if (ch == '=') token.number = taddAssign;
			else {
				token.number = tplus;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '-':
			ch = fgetc(sourceFile);
			if (ch == '-')  token.number = tdec;
			else if (ch == '=') token.number = tsubAssign;
			else {
				token.number = tminus;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '<':
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tlesse;
			else {
				token.number = tless;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '=':
			ch = fgetc(sourceFile);
			if (ch == '=')  token.number = tequal;
			else {
				token.number = tassign;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '>':
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tgreate;
			else {
				token.number = tgreat;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '|':
			ch = fgetc(sourceFile);
			if (ch == '|')  token.number = tor;
			else {
				lexicalError(3);
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '(': token.number = tlparen;         break;
		case ')': token.number = trparen;         break;
		case ',': token.number = tcomma;          break;
		case ';': token.number = tsemicolon;      break;
		case '[': token.number = tlbracket;       break;
		case ']': token.number = trbracket;       break;
		case '{': token.number = tlbrace;         break;
		case '}': token.number = trbrace;         break;
		case EOF: token.number = teof;            break;
		default: {
			printf("Current character : %c", ch);
			lexicalError(4);
			break;
		}
		}
	} while (token.number == tnull);

	//PrintTokenState(token);
	return token;
}