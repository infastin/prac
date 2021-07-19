#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

void task1_1(void)
{
	char *str;
	int base;

	printf("Enter a string containing the number:\n");
	while (scanf("%m[^\n]s", &str) != 1)
	{
		printf("Input error, re-enter!\n");

		int c;
		while((c = getchar()) != '\n' && c != EOF);
	}

	printf("Enter the base of the number system:\n");
	while (scanf("%d", &base) != 1)
	{
		printf("Input error, re-enter!\n");

		int c;
		while((c = getchar()) != '\n' && c != EOF);
	}

	int err;
	int res = strtoint(str, base, &err);
	if (err == 0)
		printf("Result: %d\n", res);
	else
		printf("An error occured!\n");
}

void task1_2(void)
{
	int number;
	int base;

	printf("Enter number:\n");
	while (scanf("%d", &number) != 1)
	{
		printf("Input error, re-enter!\n");

		int c;
		while((c = getchar()) != '\n' && c != EOF);
	}

	printf("Enter the base of the number system:\n");
	while (scanf("%d", &base) != 1)
	{
		printf("Input error, re-enter!\n");

		int c;
		while((c = getchar()) != '\n' && c != EOF);
	}

	int err;
	char *res = inttostr(number, base, &err);
	if (err == 0)
		printf("Result: %s\n", res);
	else
		printf("An error occured!\n");
}

int main(int argc, char *argv[])
{
	int choice;
	int check;

	printf("1. Convert string with number of some base to integer.\n");
	printf("2. Convert interger to string with number of some base.\n");
	printf("0. Exit\n");
	printf("Choose number:\n");
	while ((check = scanf("%d", &choice)) != 1 || choice < 0 || choice > 2) 
	{
		if (check != 1)
			printf("Input error, re-enter!\n");
		else
			printf("Wrong choice!\n");
	}

	int c;
	while((c = getchar()) != '\n' && c != EOF);

	switch (choice) 
	{
		case 1:
			task1_1();
			break;
		case 2:
			task1_2();
			break;
		default:
			break;
	}

	return 0;
}
