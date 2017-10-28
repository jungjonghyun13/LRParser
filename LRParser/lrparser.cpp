#include <stdio.h>
#include <stdlib.h>
#include "miniC.tbl"
#include "scanner.h"
#define PS_SIZE 200
FILE *sourceFile;
char filename[100];

int errcnt = 0;
int sp;
int stateStack[PS_SIZE];
int symbolStack[PS_SIZE];

void semantic(int n) {
	printf("reduced rule number number = %d\n", n);
}
void printToken(struct tokenType token) {
	if (token.number == tident)
		printf("%s", token.value.id);
	else if (token.number == tnumber)
		printf("%d", token.value.num);
	else
		printf("%s", tokenName[token.number]);
}
void dumpStack() {
	int i, start;
	if (sp > 10) start = sp - 10;
	else start = 0;
	printf("\n *** dump state stack : ");
	for (i = start; i <= sp; ++i)
		printf("%d", stateStack[i]);
	printf("\n *** dump symbol stack :");
	for (i = start; i <= sp; ++i)
		printf("%d", symbolStack[i]);
	printf("\n");
}
void errorRecovery() {
	struct tokenType tok;
	int parenthesisCount, braceCount;
	int i;
	//step 1: skip to the semicolon
	parenthesisCount = braceCount = 0;
	while (true) {
		tok = scanner();
		if (tok.number = teof)exit(1);
		if (tok.number == tlparen) parenthesisCount++;
		else if (tok.number == trparen) parenthesisCount--;
		if (tok.number == tlbrace) braceCount++;
		else if (tok.number == trbrace) braceCount--;
		if ((tok.number == tsemicolon) && (parenthesisCount <= 0) && (braceCount <= 0))
			break;
	}

	// step 2: adjust state stack
	for (i = sp; i >= 0; i--) {
		//statement_list -> statement_list . statement
		if (stateStack[i] == 36)break;

		//statement_list -> . statement
		//statement_list -> . statement_list statement
		if (stateStack[i] == 24)break;

		//declaration_list -> declaration_list . declaration
		if (stateStack[i] == 25) break; // second internal dcl

		//declaration_list -> . declaration
		//declaration_list -> . declaration_list declaration
		if (stateStack[i] == 17) break; // internal declaration

		//external declaration
		//external_dcl -> . declaration
		if (stateStack[i] == 2) break;
		if (stateStack[i] == 0)  break;
	}
}


void parser()
{
	extern int parsingTable[NO_STATES][NO_SYMBOLS + 1];
	extern int leftSymbol[NO_RULES + 1], rightLength[NO_RULES + 1];
	int entry, ruleNumber, lhs;
	int currentState;
	struct tokenType token;

	sp = 0; stateStack[sp] = 0;
	token = scanner();				//Scanner 추가하면서 검토할 부분
	while (true) {
		currentState = stateStack[sp];
		entry = parsingTable[currentState][token.number];		//Scanner 추가하면서 검토할 부분
		if (entry > 0) {
			sp++;
			if (sp > PS_SIZE) {
				printf("critical compiler error: parsing stack overflow\n");
				exit(1);
			}
			symbolStack[sp] = token.number;
			stateStack[sp] = entry;
			token = scanner();
		}
		else if (entry < 0) {
			ruleNumber = -entry;
			if (ruleNumber == GOAL_RULE) {
				if (errcnt == 0)printf(" *** valid source ***\n\n");
				else printf(" *** error in source : %d\n", errcnt);
				return;
			}
			semantic(ruleNumber);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			currentState = parsingTable[stateStack[sp]][lhs];
			sp++;
			symbolStack[sp] = lhs;
			stateStack[sp] = currentState;
		}
		else {
			printf(" === error in source ===\n");
			errcnt++;
			printf("Current Token : ");
			printToken(token);
			dumpStack();
			errorRecovery();
			token = scanner();
		}
	}/* while(true) */
}/* parser */
int main(int argc, char *argv[]) {
	if (argc < 2)
	{
		printf("\n Endter filename : ");
		scanf("%s", filename);
		//strcpy(filename ,"perfect.mc");
		//printf("perfect.mc\n");
	}
	else if (argc == 2)
	{
		strcpy(filename, argv[1]);
	}
	else {
		printf("Arguement error!");
		exit(1);
	}
	if ((sourceFile = fopen(filename, "r")) == NULL)
	{
		printf("File open error!\n");
		exit(2);
	}
	printf(" > Scanner started ...\n\n");

	parser();
	//if (token.number == t$) {
	//	printf("\n\n\t\tAccepted\n\n");
	//}
	//else {
	//	printf("\n\n\t\t Not Accepted\n\n");
	//}

}