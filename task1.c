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

	char *err_str;
	int err_code;

	int res = strtoint(str, base, &err_code, &err_str);
	if (err_code == 0)
		printf("Result: %d\n", res);
	else if (err_code != 0 && err_str != NULL)
	{
		printf("%s!\n", err_str);
		free(err_str);
	}
	else
		printf("An error occured!\n");

	free(str);
}

void task1_2(void)
{
	char *str;

	int number;
	int base;
	
	int err_code_in;
	char *err_str_in;

	printf("Enter number:\n");
	while (1)
	{
		int serr = scanf("%m[^\n]s", &str);

		if (serr == 1)
		{
			number = strtoint(str, 10, &err_code_in, &err_str_in);

			if (err_code_in == 0)
			{
				free(str);
				break;
			}
		}

		if (err_str_in != NULL)
		{
			printf("%s!\n", err_str_in);
			free(err_str_in);
		}

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

	int err_code_out;
	char *err_str_out;

	char *res = inttostr(number, base, &err_code_out, &err_str_out);
	if (err_code_out == 0)
		printf("Result: %s\n", res);
	else if (err_code_out != 0 && err_str_out != NULL)
	{
		printf("%s!\n", err_str_out);
		free(err_str_out);
	}
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
