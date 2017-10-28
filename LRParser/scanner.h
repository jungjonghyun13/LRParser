#pragma once
#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define FNAMESIZE 100
#define NO_KEYWORD 11
#define ID_LENGTH 12
extern FILE *fp;
enum tsymbol {
	tnull = -1,
	tdot, tsemi, tcomma, tcolon, tassign, tequal, tless, tgreat,
	/*0    1       2       3       4       5       6       7*/
	tplus, tminus, tdiv, tmul, tlparen, trparen, tident, tnumber, t$,
	/*8     9       10    11    2       13       14        15    16 */
	/*............word symbols.......................*/
	tbegin, tend, tlabel, tinteger, tgoto, tif, tthen, tfi, telse,
	/*17     18     19        20       21   22   23    24    25*/
	toutput, tinput
	/*26      27*/
};

char *tokenName[] = {
	".",	";",	",",	":",	"=>",	"=",	"<",	">",
	/*0    1       2       3       4       5       6       7*/
	"+",	"-",	"/",	"*",	"(",	")",	"%id",	"%number",	"EOF",
	/*8      9       10      11      2       13       14        15        16 */
	/*............word symbols.......................*/
	"begin",	"end",	"label",	"integer",	"goto",	"if",	"then",	"fi",	"else",
	/*17          18      19             20        21    22        23    24       25*/
	"output",	"input"
	/*26      27*/
};

//�� ����� �ش��ϴ� ��ū�� ��ȣ�� ���� �迭
enum tsymbol tnum[NO_KEYWORD] = {
	tbegin, tend, tlabel, tinteger, tgoto, tif, tthen, tfi, telse, toutput, tinput
};
struct tokenType {
	int number;			//��ū ��ȣ
	union {
		char id[ID_LENGTH];
		int num;
	} value;
};
struct tokenType token;

char *keyword[NO_KEYWORD] = {
	"begin", "end","label","integer","goto","if","then","fi","else","output","input"
};

int superLetter(char ch)	//isalpha ���ĺ����� �˻� 
{
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}

int superLetterOrDigit(char ch)	//( al num�̸� ���ĺ��̰ų� ����) 
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
		ch = fgetc(fp);
	} while (isdigit(ch));
	ungetc(ch, fp); /*  retract  */
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
	if (t.number <= 13) {
		printf("\tToken -----> %9s  (%9d)\n", tokenName[t.number], t.number);
	}
	else if (t.number == 14) {
		printf("\tToken -----> %9s  (%9d)\n", tokenName[t.number], t.number);
	}
	else if (t.number == 15) {
		printf("\tToken -----> %9s  (%9d)\n", tokenName[t.number], t.number);
	}
	else if (t.number == 16) {//EOF
		printf("\tToken -----> %9s  (%9d)\n", tokenName[t.number], t.number);
		//exit(0);
	}
	else if (27 >= t.number && t.number >=17 ) {
		printf("\tToken -----> %9s  (%9d)\n", tokenName[t.number], t.number);
	}
}
struct tokenType scanner()
{
	int i, index;
	char ch, id[100];//�ӽ÷� 100���� ����.
	token.number = tnull;

	do {
		while (isspace(ch = getc(fp)));
		if (superLetter(ch))
		{
			i = 0;
			do {
				if (i < ID_LENGTH)
					id[i++] = ch;
				else
					break;
				ch = getc(fp);
			} while (superLetterOrDigit(ch));
			if (i >= ID_LENGTH) lexicalError(1);	//Id�� ���̰� �ִ� ���̸� �ʰ��� 
			id[i] = '\0';
			ungetc(ch, fp); //  retract

			// find the identifier in the keyword table
			for (index = 0; index < NO_KEYWORD; index++) //id�� �ν��ϴ� �κ�
				if (!strcmp(id, keyword[index]))
					break;
			if (index < NO_KEYWORD)
			{
				token.number = tnum[index];		//Ű������ ��ū �ѹ��� tnum[index]�� ��´�.
			}
			else token.number = tident; //Ű���尡 �ƴ϶�� �ĺ��ڶ�� ǥ���Ѵ�.
		} // end of identifier or keyword
		else if (isdigit(ch)) { // number
			token.value.num = getNumber(ch);
			token.number = tnumber;
		}
		else switch (ch)
		{
		case '.': token.number = tdot;	break;
		case ';': token.number = tsemi;	break;
		case ',': token.number = tcomma; break;
		case ':': token.number = tcolon; break;
		case '<': token.number = tless;	 break;
		case '>': token.number = tgreat; break;
		case '+': token.number = tplus;	 break;
		case '-': token.number = tminus; break;
		case '*': token.number = tmul;	 break;
		case '/': token.number = tdiv;	 break;
		case '(': token.number = tlparen;  break;
		case ')': token.number = trparen;  break;
		case EOF: token.number = t$;	break;
		case '=': 
			ch = getc(fp);
			if(ch == '>')
				token.number = tassign;
			else {
				token.number = tequal;	break;
				ungetc(ch, fp);
			}
		}
		PrintTokenState(token);
		return token;
	} while (token.number == tnull);
}