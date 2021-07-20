#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Utils.h"

int strtoint(const char *str, int base, int *err_code, char **err_str)
{
	if ((base != 0 && base < 2) || base > 62)
	{
		if (err_str != NULL)
			*err_str = strdup_printf("Base can only be 0 or in range [2, 62]");

		if (err_code != NULL)
			*err_code = 1;

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
			{
				if (err_str != NULL)
					*err_str = strdup_printf("Not a number given");

				if (err_code != NULL)
					*err_code = 2;

				return 0;
			}

			if (new_base > base)
				base = new_base;

			tmp++;
		}
	}

	if (base == 0)
	{
		if (err_str != NULL)
			*err_str = strdup_printf("Not a number given");

		if (err_code != NULL)
			*err_code = 2;

		return 0;
	}

	int digit = 0;

	while (*p != '\0') 
	{
		if (*p >= '0' && *p <= '9')
			digit = *p - '0';
		else if (*p >= 'A' && *p <= 'Z')
			digit = *p - 'A' + 10;
		else if (*p >= 'a' && *p <= 'z')
			digit = *p - 'a' + 36;
		else
		{
			if (err_str != NULL)
				*err_str = strdup_printf("Not a number given");

			if (err_code != NULL)
				*err_code = 2;

			return 0;
		}

		if (digit >= base)
		{
			if (err_str != NULL)
				*err_str = strdup_printf("Not a number given");

			if (err_code != NULL)
				*err_code = 2;

			return 0;
		}

		if (negative)
		{
			if ((result < INT_MIN / base) || (result * base < INT_MIN + digit))
			{
				if (err_str != NULL)
					*err_str = strdup_printf("Integer underflow");

				if (err_code != NULL)
					*err_code = 3;

				return 0;
			}
		}
		else
		{
			if ((result > INT_MAX / base) || (result * base > INT_MAX - digit))
			{
				if (err_str != NULL)
					*err_str = strdup_printf("Integer overflow");

				if (err_code != NULL)
					*err_code = 3;

				return 0;
			}
		}

		result *= base;

		if (negative)
			result -= digit;
		else
			result += digit;

		p++;
	}

	if (err_str != NULL)
		*err_str = NULL;

	if (err_code != NULL)
		*err_code = 0;

	return result;
}

unsigned int ustrtoint(const char *str, int base, int *err_code, char **err_str)
{
	if ((base != 0 && base < 2) || base > 62)
	{
		if (err_str != NULL)
			*err_str = strdup_printf("Base can only be 0 or in range [2, 62]");

		if (err_code != NULL)
			*err_code = 1;

		return 0;
	}

	const char *p = str;

	unsigned int result = 0;

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
			{
				if (err_str != NULL)
					*err_str = strdup_printf("Not a positive number given");

				if (err_code != NULL)
					*err_code = 2;

				return 0;
			}

			if (new_base > base)
				base = new_base;

			tmp++;
		}
	}

	if (base == 0)
	{
		if (err_str != NULL)
			*err_str = strdup_printf("Not a positive number given");

		if (err_code != NULL)
			*err_code = 2;

		return 0;
	}

	int digit = 0;

	while (*p != '\0') 
	{
		if (*p >= '0' && *p <= '9')
			digit = *p - '0';
		else if (*p >= 'A' && *p <= 'Z')
			digit = *p - 'A' + 10;
		else if (*p >= 'a' && *p <= 'z')
			digit = *p - 'a' + 36;
		else
		{
			if (err_str != NULL)
				*err_str = strdup_printf("Not a positive number given");

			if (err_code != NULL)
				*err_code = 2;

			return 0;
		}

		if (digit >= base)
		{
			if (err_str != NULL)
				*err_str = strdup_printf("Not a positive number given");

			if (err_code != NULL)
				*err_code = 2;

			return 0;
		}

		if ((result > UINT_MAX / base) || (result * base > UINT_MAX - digit))
		{
			if (err_str != NULL)
				*err_str = strdup_printf("Unsigned integer overflow");

			if (err_code != NULL)
				*err_code = 3;

			return 0;
		}

		result *= base;
		result += digit;

		p++;
	}

	if (err_str != NULL)
		*err_str = NULL;

	if (err_code != NULL)
		*err_code = 0;

	return result;
}

char* inttostr(int number, int base, int *err_code, char **err_str)
{
	if (base < 2 || base > 62)
	{
		if (err_str != NULL)
			*err_str = strdup_printf("Base can only be in range [2, 62]");

		if (err_code != NULL)
			*err_code = 1;

		return 0;
	}

	int negative = 0;
	int symbols_numb = 0;

	if (number < 0)
	{
		negative = 1;
		symbols_numb = 1;
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
		if (err_str != NULL)
			*err_str = "String allocation error";

		if (err_code != NULL)
			*err_code = 2;

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

		if (negative)
			digit *= -1;

		if (digit < 10)
			result[i] = digit + '0';
		else if (digit < 36)
			result[i] = digit + 'A' - 10;
		else
			result[i] = digit + 'a' - 36;

		i--;
	}

	if (err_str != NULL)
		*err_str = NULL;

	if (err_code != NULL)
		*err_code = 0;

	return result;
}

char* strdup_printf(const char *fmt, ...)
{
	va_list ap, ap_copy;
	va_start(ap, fmt);
	va_copy(ap_copy, ap);

	int len = vsnprintf(NULL, 0, fmt, ap_copy);
	char *result = (char*)calloc(sizeof(char), len + 1);
	vsnprintf(result, len + 1, fmt, ap);

	va_end(ap_copy);
	va_end(ap);

	return result;
}

int has_char(const char *str, char ch)
{
	const char *p = str;

	while (*p != '\0') 
	{
		if (*p == ch)
			return 1;

		p++;
	}
	
	return 0;
}
