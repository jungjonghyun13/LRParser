#include "parser.h"
FILE *sourceFile;

int main(int argc, char *argv[]) {
	if (argc < 2)
	{
		printf("\n Endter filename : ");
		scanf("%s", filename);
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
}