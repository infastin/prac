#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		fprintf(stderr, "No filename given!\n");
		return EXIT_FAILURE;
	}

	FILE *in = fopen(argv[1], "r");

	if (in == NULL)
	{
		perror("Couldn't open file");
		return EXIT_FAILURE;
	}

	int c;
	int i = 0;

	char buf[16];

	while (1) 
	{
		c = getc(in);
		buf[i] = c;

		if (c != EOF)
		{
			char *sym = inttostr(c, 16, NULL);

			if (strlen(sym) == 1)
				putchar('0');
			printf("%s ", sym);

			free(sym);
		}

		if (++i == 16 || c == EOF)
		{
			if (c == EOF)
				for (int j = i - 1; j < 16; ++j) 
					printf("   ");

			printf("| ");
			for (int j = 0; j < i - 1; ++j) 
			{
				if (buf[j] > 31 && buf[j] < 127)
					putchar(buf[j]);
				else
					putchar('?');

				buf[j] = '\0';
			}

			putchar('\n');
			i = 0;
		}

		if (c == EOF)
			break;
	}

	fclose(in);

	return 0;
}
