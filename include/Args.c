#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdarg.h>

#include "Utils.h"
#include "Any.h"
#include "Args.h"

int argcmp(const char *arg, const char *oarg, int short_value)
{
	if (short_value == 0)
		return strcmp(arg, oarg);

	int arg_len = strlen(arg);
	int oarg_len = strlen(oarg);

	if (arg_len == oarg_len)
		return strcmp(arg, oarg);

	if (arg_len < oarg_len)
		return 1;

	char *tmp = strdup(arg);
	tmp[oarg_len] = '\0';
	int res = strcmp(tmp, oarg);
	free(tmp);

	return res;
}

void parse_args_type(char *str, option_t *opt, argr_t *ret)
{
	if (opt->vdt == UINT_TYPE)
	{
		int err_code;
		unsigned int val = ustrtoint(str, 10, &err_code, NULL);

		if (err_code == 0)
			ret->value = any_alloc(UINT_TYPE, &val);
		else
			ret->shrt = '/';
	}
	else if (opt->vdt == STR_TYPE)
		ret->value = any_alloc(STR_TYPE, str);
}

int parse_args(int argc, char *argv[], option_t *opts, int opts_number, argr_t *ret)
{
	static int i = 0;

	if (ret->value != NULL)
		free(ret->value);

	if (++i >= argc)
		return -1;

	ret->value = NULL;
	ret->arg = NULL;
	ret->carg = NULL;

	if (argv[i][0] != '-')
	{
		ret->arg = argv[i];
		ret->shrt = '?';
		return 1;
	}

	for (int j = 0; j <= opts_number; ++j)
	{
		if (j == opts_number)
		{
			ret->arg = argv[i];
			ret->shrt = '?';
			return 2;
		}

		if (argcmp(&argv[i][1], opts[j].arg, opts[j].vpt & SHORT_VALUE) == 0)
		{
			ret->arg = opts[j].arg;
			ret->shrt = opts[j].shrt;

			for (int k = 1; k < i; ++k)
			{
				if (argcmp(&argv[k][1], opts[j].arg, opts[j].vpt & SHORT_VALUE) == 0)
				{
					ret->shrt = '#';
					return 3;
				}
			}

			if (opts[j].conflict_number != 0)
			{
				for (int k = 1; k < i; ++k)
				{
					for (int l = 0; l < opts[j].conflict_number; ++l) 
					{
						int ci = opts[j].conflicts[l];

						if (argcmp(&argv[k][1], opts[ci].arg, opts[l].vpt & SHORT_VALUE) == 0)
						{
							ret->shrt = '@';
							ret->carg = opts[ci].arg;
							return 3;
						}
					}
				}
			}

			if (opts[j].vpt == NO_VALUE)
				break;

			if ((opts[j].vpt & SHORT_VALUE) != 0)
			{
				int oarg_len = strlen(opts[j].arg);

				if (argv[i][oarg_len + 1] != '\0')
				{
					parse_args_type(&argv[i][oarg_len + 1], &opts[j], ret);
					break;
				}
			}

			if ((opts[j].vpt & LONG_VALUE) != 0)
			{
				if (i + 1 < argc && (argv[i + 1][0] != '-' || isdigit(argv[i + 1][1])))
				{
					parse_args_type(argv[i + 1], &opts[j], ret);
					i++;
					break;
				}
			}

			if ((opts[j].vpt & (SHORT_VALUE | LONG_VALUE)) != 0 && ret->value == NULL)
			{
				ret->shrt = '!';
				return 4;
			}
		}
	}

	return 0;
}
