#include <stdio.h>
#include <string.h>

int read_block(FILE *file, char *block, int len)
{
	int c;
	int i = 0;

	while (1) 
	{
		c = fgetc(file);

		if (c == EOF)
		{
			if (i == 0)
				return -1;

			break;
		}

		block[i++] = c;

		if (i == len - 1)
		{
			block[i] = '\0';
			break;
		}
	}

	return i;
}

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
	FILE *in, *out;

	in = fopen("text2.txt", "r");
	out = fopen("text3.txt", "w");

	const char *find = "Lorem ipsum";
	const char *replace = "Fish";
	const char *f = find;

	char block[BLOCK_SIZE] = {0};

	while (1) 
	{
		int len = read_block(in, block, BLOCK_SIZE);

		if (len == -1)
			break;

		char *b = block;

		while (1) 
		{
			if (*b == '\0')
				break;

			if (*b != *f)
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

				fputc(*b, out);
			}
			else if (*++f == '\0')
			{
				fputs(replace, out);
				f = find;
			}

			if (*b == '\0')
				break;

			b++;
		}

		memset(block, 0, len);
	}

	fclose(in);
	fclose(out);

	return 0;
}
