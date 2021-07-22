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

#define BLOCK_SIZE 2

int main(int argc, char *argv[])
{
	FILE *in, *out;

	in = fopen("text2.txt", "r");
	out = fopen("text3.txt", "w");

	const char *find = "aab";
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
					const char *s = find;   // Substring of "find", where first character of substring is printed
					const char *p = find;   // Pointer to "find"
					const char *sw = s + 1; // "s" without first character

					fputc(*s, out);

					while (1) 
					{
						if (sw == f)
						{
							f--;

							if (*f != *b)
							{
								fputc(*b, out);
								f = find;
							}

							break;
						}

						if (*p != *sw)
						{
							fputc(*++s, out);

							p = find;
							sw = s + 1;
							
							continue;
						}

						p++;
						sw++;
					}

				}
				else 
					fputc(*b, out);
			}
			
			if (*b == *f && *++f == '\0')
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
