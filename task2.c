#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
	FILE *in = fopen("text2.txt", "r");
	FILE *out = fopen("text3.txt", "w");

	const char *find = "ad";
	const char *replace = "12345";
	const char *f = find;

	int c;

	while (1) 
	{
		c = fgetc(in);

		if (c != EOF && c != *f)
		{
			if (f != find)
			{
				const char *p = find;
				while (p != f) 
				{
					fputc(*p, out);
					p++;
				}

				f = find;
			}

			fputc(c, out);
		}
		else if (*++f == '\0')
		{
			fputs(replace, out);
			f = find;
		}

		if (c == EOF || c == '\0')
			break;
	}

	fclose(in);
	fclose(out);

	return 0;
}
