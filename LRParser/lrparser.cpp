#include "parser.h"
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