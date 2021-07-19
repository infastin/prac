#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int strtoint(const char *str, int base, int *err)
{
	if (base == 1 || base > 62)
	{
		fprintf(stderr, "strtoint: BASE can only be 0 or in range [2, 62]!\n");
		
		if (err != NULL)
			*err = 1;

		return 0;
	}

	const char *p = str;

	int result = 0;
	int negative = 0;

	if (*p == '-')
	{
		negative = 1;
		p++;
	}

	if (base == 0)
	{
		const char *tmp = p;
		int new_base = 0;

		while (*tmp != '\0') 
		{
			if (*tmp >= '0' && *tmp <= '9')
				new_base = *tmp - '0' + 1;
			else if (*tmp >= 'A' && *tmp <= 'Z')
				new_base = *tmp - 'A' + 11;
			else if (*tmp >= 'a' && *tmp <= 'z')
				new_base = *tmp - 'a' + 37;
			else
				break;

			if (new_base > base)
				base = new_base;

			tmp++;
		}
	}

	if (base == 0)
	{
		fprintf(stderr, "strtoint: Not a number given!");
		
		if (err != NULL)
			*err = 1;

		return 0;
	}

	int digit;

	while (*p != '\0') 
	{
		if (*p >= '0' && *p <= '9')
			digit = *p - '0';
		else if (*p >= 'A' && *p <= 'Z')
			digit = *p - 'A' + 10;
		else if (*p >= 'a' && *p <= 'z')
			digit = *p - 'a' + 36;
		else
			break;

		if (digit >= base)
			break;

		if ((result > INT_MAX / base) || (result * base > INT_MAX - digit))
		{
			fprintf(stderr, "strtoint: Integer overflow!\n");
			
			if (err != NULL)
				*err = 1;

			return 0;
		}

		result *= base;
		result += digit;

		p++;
	}

	if (negative)
		result *= -1;

	if (err != NULL)
		*err = 1;

	return result;
}

char* inttostr(int number, int base, int *err)
{
	if (base < 2 || base > 62)
	{
		fprintf(stderr, "strtoint: BASE can only be 0 or in range [2, 62]!\n");
		
		if (err != NULL)
			*err = 1;

		return 0;
	}

	int negative = 0;
	int symbols_numb = 0;

	if (number < 0)
	{
		negative = 1;
		symbols_numb = 1;
		number *= - 1;
	}

	int tmp = number;

	while (tmp != 0) 
	{
		tmp /= base;
		symbols_numb++;
	}

	char *result = (char*)calloc(symbols_numb + 1, 1);

	if (result == NULL)
	{
		fprintf(stderr, "inttostr: String allocation error!");
		
		if (err != NULL)
			*err = 1;

		return NULL;
	}

	if (negative)
		result[0] = '-';

	int digit;
	int i = symbols_numb - 1;

	while (number != 0) 
	{
		digit = number % base;
		number /= base;

		if (digit < 10)
			result[i] = digit + '0';
		else if (digit < 36)
			result[i] = digit + 'A' - 10;
		else
			result[i] = digit + 'a' - 36;

		i--;
	}

	if (err != NULL)
		*err = 1;

	return result;
}
