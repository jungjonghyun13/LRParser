#include <stdio.h>
#include <stdlib.h>
#include "miniC.tbl"
#define PS_SIZE 200

void semantic(int);
void printToken(struct tokenType token);
void dumpStack();
void errorRecovery();

int errcnt = 0;
int sp;
int stateStack[PS_SIZE];
int symbolStack[PS_SIZE];
struct tokenType{//Scanner 추가하면서 검토할 부분
	int number;
};
tokenType scanner() {//Scanner 추가하면서 검토할 부분
	tokenType A;
	return A;
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
				if (errcnt == 0)printf(" *** valid source ***\n");
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