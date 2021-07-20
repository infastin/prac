#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>

#include "Utils.h"
#include "Any.h"
#include "Args.h"

static const char _uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char _lowercase[] = "abcdefghijklmnopqrstuvwxyz";
static const char _numbers[] = "1234567890";
static const char _special[] = "~`!@#$%^&*()-_+={}[]|\\/:;\"'<>,.?";

#define rrand(min, max) ((rand() % ((max) + 1 - (min))) + (min))

double drrand(double min, double max)
{
	double range = max - min;
	double div = ((double) RAND_MAX) / range;
	return min + (rand() / div);
}

char getsymb1(const char *mass, const double *weights, int mwlen)
{
	double sum = 0;

	for (int i = 0; i < mwlen; ++i) 
	{
		sum += weights[i];
	}

	double rnd = drrand(0, sum);

	for (int i = 0; i < mwlen; ++i) 
	{
		if (rnd < weights[i])
			return mass[i];

		rnd -= weights[i];
	}

	return 0;
}

char getsymb2(const char **mass, const double *weights, const int *mass_sizes, int mwlen)
{
	double sum = 0;

	for (int i = 0; i < mwlen; ++i) 
	{
		sum += weights[i];
	}

	double rnd = drrand(0, sum);

	for (int i = 0; i < mwlen; ++i) 
	{
		if (rnd < weights[i])
			return mass[i][rand() % mass_sizes[i]];

		rnd -= weights[i];
	}

	return 0;
}

void calc_weights(double *weights, int wlen)
{
	double sum = 0;
	int no_weight = 0;

	for (int i = 0; i < wlen; ++i) 
	{
		if (weights[i] == -1)
		{
			no_weight++;
			continue;
		}

		sum += weights[i];
	}

	for (int i = 0; i < wlen; ++i) 
	{
		if (weights[i] != -1)
			continue;

		weights[i] = (100 - sum) / no_weight;
		sum += weights[i];
		no_weight--;
	}
}

double* generate_weights(int wlen)
{
	double *weights = (double*)malloc(wlen * sizeof(double));
	assert(weights != NULL);

	double sum = 0;

	for (int i = 0; i < wlen; ++i) 
	{
		weights[i] = (100 - sum) / (wlen - i);
		sum += weights[i];
	}

	return weights;
}

double* generate_empty_weights(int wlen)
{
	double *weights = (double*)malloc(wlen * sizeof(double));
	assert(weights != NULL);

	for (int i = 0; i < wlen; ++i) 
	{
		weights[i] = -1;
	}

	return weights;
}

int* generate_mass_sizes(const char **mass, int mlen)
{
	int *mass_sizes = (int*)malloc(mlen * sizeof(int));
	assert(mass_sizes != NULL);

	for (int i = 0; i < mlen; ++i) 
	{
		mass_sizes[i] = strlen(mass[i]);
	}

	return mass_sizes;
}

double* int_mass_to_double(int *mass, int mlen)
{
	double *new_mass = (double*)malloc(mlen * sizeof(double));
	assert(new_mass != NULL);

	for (int i = 0; i < mlen; ++i) 
	{
		new_mass[i] = (double) mass[i];
	}

	return new_mass;
}

char* generate_pwd2(const char **mass, const double *weights, const int *mass_sizes, int mwlen, int pwdlen)
{
	char *pwd = (char*)calloc(pwdlen + 1, 1);
	assert(pwd != NULL);

	for (int i = 0; i < pwdlen; ++i) 
	{
		pwd[i] = getsymb2(mass, weights, mass_sizes, mwlen);
	}

	return pwd;
}

char* generate_pwd1(const char *alphabet, const double *weights, int mwlen, int pwdlen)
{
	char *pwd = (char*)calloc(pwdlen + 1, 1);
	assert(pwd != NULL);

	for (int i = 0; i < pwdlen; ++i) 
	{
		pwd[i] = getsymb1(alphabet, weights, mwlen);
	}

	return pwd;
}

double parse_weight(char *p, char **np)
{
	int weight = 0;

	while (*p != '\0') 
	{
		if (!isdigit(*p))
		{
			printf("Not expected '%c' character in weight!\n", *p);
			exit(EXIT_FAILURE);
		}

		weight *= 10;
		weight += *p - '0';

		if (weight > 100)
		{
			printf("Weight can't exceed 100!\n");
			exit(EXIT_FAILURE);
		}

		p++;
	}

	*np = p;

	return (double) weight;
}

unsigned int parse_index(char *p, char **np)
{
	unsigned int idx = 0;

	while (*p != '}') 
	{
		if (!isdigit(*p))
		{
			printf("Not expected '%c' character in index!\n", *p);
			exit(EXIT_FAILURE);
		}

		if ((idx > UINT_MAX / 10) || (idx * 10 > UINT_MAX - (*p - '0')))
		{
			printf("Weight index (unsigned integer) overflow!\n");
			exit(EXIT_FAILURE);
		}

		idx *= 10;
		idx += *p - '0';

		p++;
	}

	*np = ++p;

	return idx;
}

void parse_range(char *p, char **np, unsigned int *_idx1, unsigned int *_idx2)
{
	unsigned int idx1 = 0;
	unsigned int idx2 = 0;

	while (*p != '-') 
	{
		if (!isdigit(*p))
		{
			printf("Not expected '%c' character in first index of range!\n", *p);
			exit(EXIT_FAILURE);
		}

		if ((idx1 > UINT_MAX / 10) || (idx1 * 10 > UINT_MAX - (*p - '0')))
		{
			printf("First index (unsigned integer) in range of weights overflow!\n");
			exit(EXIT_FAILURE);
		}

		idx1 *= 10;
		idx1 += *p - '0';

		p++;
	}

	p++;

	while (*p != ']') 
	{
		if (!isdigit(*p))
		{
			printf("Not expected '%c' character in second index of range!\n", *p);
			exit(EXIT_FAILURE);
		}

		if ((idx2 > UINT_MAX / 10) || (idx2 * 10 > UINT_MAX - (*p - '0')))
		{
			printf("Second index (unsigned integer) in range of weights overflow!\n");
			exit(EXIT_FAILURE);
		}

		idx2 *= 10;
		idx2 += *p - '0';

		p++;
	}

	if (_idx1 > _idx2)
	{
		printf("First index in range of weights can't be greater than second index!\n");
		exit(EXIT_FAILURE);
	}

	*np = ++p;
	*_idx1 = idx1;
	*_idx2 = idx2;
}

int main(int argc, char *argv[])
{
	srand(time(0));

	option_t opts[6] = {
		{ "min", LONG_VALUE,               UINT_TYPE, 'm', 1, (int[1]){2}    },
		{ "max", LONG_VALUE,               UINT_TYPE, 'M', 1, (int[1]){2}    },
		{ "n",   SHORT_VALUE | LONG_VALUE, UINT_TYPE, 'n', 2, (int[2]){0, 1} },
		{ "c",   SHORT_VALUE | LONG_VALUE, STR_TYPE,  'c', 1, (int[1]){4}    },
		{ "a",   LONG_VALUE, 			   STR_TYPE,  'a', 2, (int[2]){3, 5} },
		{ "w",   LONG_VALUE, 			   STR_TYPE,  'w', 0, NULL    		 },
	};

	int perr;
	argr_t argr = INIT_ARGR;

	unsigned int min = 0;
	unsigned int max = 0;
	unsigned int len = 0;

	int min_flag = 0;
	int max_flag = 0;

	char *cvalue = NULL;
	char *wvalue = NULL;
	char *alphabet = NULL;

	while ((perr = parse_args(argc, argv, opts, 6, &argr)) != -1) 
	{
		switch (argr.shrt) 
		{
			case 'm':
				any_get(argr.value, &min);
				min_flag = 1;

				if (min == 0)
				{
					printf("Minimum length of password can't be equal zero!\n");
					exit(EXIT_FAILURE);
				}

				break;
			case 'M':
				any_get(argr.value, &max);
				max_flag = 1;

				if (max == 0)
				{
					printf("Maximum length of password can't be equal zero!\n");
					exit(EXIT_FAILURE);
				}

				break;
			case 'n':
				any_get(argr.value, &len);

				if (len == 0)
				{
					printf("Length of password can't be equal zero!\n");
					exit(EXIT_FAILURE);
				}

				break;
			case 'c':
				any_get(argr.value, &cvalue);
				break;
			case 'w':
				any_get(argr.value, &wvalue);
				break;
			case 'a':
				any_get(argr.value, &alphabet);
				break;
			case '/':
				printf("Argument '-%s' don't expect such value!\n", argr.arg);
				exit(EXIT_FAILURE);
			case '@':
				printf("Argument '-%s' conflicts with argument '-%s'!\n", argr.arg, argr.carg);
				exit(EXIT_FAILURE);
			case '?':
				printf("No such argument '%s'!\n", argr.arg);
				exit(EXIT_FAILURE);
			case '!':
				printf("Argument '-%s' expected value!\n", argr.arg);
				exit(EXIT_FAILURE);
			case '#':
				printf("Arguments can't be specified twice!\n");
				exit(EXIT_FAILURE);
			default:
				break;
		}
	}

	if ((min_flag && !max_flag) || (!min_flag && max_flag))
	{
		printf("Arguments '-min' and '-max' should be specified together!\n");
		exit(EXIT_FAILURE);
	}

	if (min > max)
	{
		printf("Minimum length can't be greater than maximum length!\n");
		exit(EXIT_FAILURE);
	}

	if (wvalue != NULL && cvalue == NULL && alphabet == NULL)
	{
		printf("Argument '-w' can't be specified without arguments '-c' or '-a'!\n");
		exit(EXIT_FAILURE);
	}

	unsigned int pwdlen;
	const char **mass = NULL;
	double *weights = NULL;
	unsigned int mwlen = 0;

	int ca[4] = {-1, -1, -1, -1};

	if (cvalue != NULL)
	{
		char *pc;
		int ic;

		int cerr = 0;

		for (ic = 0, pc = cvalue; *pc != '\0'; pc++, ic++)
		{
			if (*pc == 'a')
			{
				if (ca[0] != -1)
				{
					cerr = 1;
					break;
				}

				ca[0] = ic;
			}
			else if (*pc == 'A')
			{
				if (ca[1] != -1)
				{
					cerr = 1;
					break;
				}

				ca[1] = ic;
			}
			else if (*pc == 'D')
			{
				if (ca[2] != -1)
				{
					cerr = 1;
					break;
				}

				ca[2] = ic;
			}
			else if (*pc == 'S')
			{
				if (ca[3] != -1)
				{
					cerr = 1;
					break;
				}

				ca[3] = ic;
			}
			else
			{
				printf("There is no '%c' array!", *pc);
				exit(EXIT_FAILURE);
			}

			mwlen++;
		}

		if (cerr)
		{
			printf("Can't specify same array twice!\n");
			exit(EXIT_FAILURE);
		}

		mass = (const char**)malloc(mwlen * sizeof(char*));

		if (ca[0] != -1)
			mass[ca[0]] = _lowercase;

		if (ca[1] != -1)
			mass[ca[1]] = _uppercase;

		if (ca[2] != -1)
			mass[ca[2]] = _numbers;

		if (ca[3] != -1)
			mass[ca[3]] = _special;

	}
	else if (alphabet == NULL)
	{
		mwlen = 3;
		mass = (const char**)malloc(mwlen * sizeof(char*));

		mass[0] = _lowercase;
		mass[1] = _uppercase;
		mass[2] = _numbers;
	}
	else
		mwlen = strlen(alphabet);

	if (wvalue != NULL)
	{
		weights = generate_empty_weights(mwlen);
		unsigned int sum = 0;

		char *pw;
		char *pww;
		int pi;

		if (cvalue != NULL)
		{
			for (pi = 0, pw = wvalue, pww = wvalue;; pw++)
			{
				if (*pw != '\0' && *pw != ',' && !isdigit(*pw))
				{
					printf("Not expected '%c' character in weights value!\n", *pw);
					exit(EXIT_FAILURE);
				}

				if (pi >= mwlen)
				{
					printf("There are't specified that many arrays!\n");
					exit(EXIT_FAILURE);
				}

				if (*pw == ',')
					*pw++ = '\0';
				else if (*pw != '\0' && isdigit(*pw))
					continue;

				int werr_code;
				unsigned int weight = ustrtoint(pww, 10, &werr_code, NULL);

				if (werr_code != 0 && werr_code == 2)
				{
					printf("Weight is not a number!\n");
					exit(EXIT_FAILURE);
				}
				else if (werr_code == 3)
				{
					printf("Weight (unsigned integer) overflow!\n");
					exit(EXIT_FAILURE);
				}

				if (weight > 100)
				{
					printf("One weight can't exceed 100!\n");
					exit(EXIT_FAILURE);
				}

				weights[pi++] = weight;
				sum += weight;
				pww = pw;

				if (*pw == '\0')
					break;
			}
		}
		else
		{
			for (pi = 0, pw = wvalue, pww = wvalue;; pw++)
			{
				int hc = has_char(",:-{}[]", *pw);

				if (!hc && *pw != '\0' && !isdigit(*pw))
				{
					printf("Not expected '%c' character in weights value!\n", *pw);
					exit(EXIT_FAILURE);
				}

				if (pi >= mwlen)
				{
					printf("There are't specified that many chars!\n");
					exit(EXIT_FAILURE);
				}

				if (*pw == ',')
					*pw++ = '\0';
				else if (*pw != '\0' && (hc || isdigit(*pw)))
					continue;

				unsigned int idx = 0;
				unsigned int idx1 = 0; 
				unsigned int idx2 = 0;

				int idx_flag = 0;
				int range_flag = 0;

				if (*pww == '{')
				{
					pww++;
					idx = parse_index(pww, &pww);
					idx_flag = 1;

					if (idx >= mwlen)
					{
						printf("No such index '%u' in alphabet!\n", idx);
						exit(EXIT_FAILURE);
					}
				}
				else if (*pww == '[')
				{
					pww++;
					parse_range(pww, &pww, &idx1, &idx2);

					if (idx1 >= mwlen || idx2 >= mwlen)
					{
						printf("There is no such range '[%d-%d]' in alphabet!\n", idx1, idx2);
						exit(EXIT_FAILURE);
					}

					range_flag = 1;
				}

				if (idx_flag + range_flag != 0 && *pww++ != ':')
				{
					printf("Expected ':' after closing bracket!\n");
					exit(EXIT_FAILURE);
				}

				double weight = parse_weight(pww, &pww);

				if (idx_flag)
				{
					pi = idx;
					weights[pi++] = weight;
				}
				else if (range_flag)
				{
					double tmp = 0;

					for (int i = idx1; i <= idx2; ++i)
					{
						weights[i] = (weight - tmp) / (idx2 - i + 1);
						tmp += weights[i];
					}

					pi = idx2 + 1;
				}
				else
					weights[pi++] = weight;

				sum += weight;
				pww++;

				if (*pw == '\0')
					break;
			}
		}

		if (sum > 100)
		{
			printf("Sum of weights can't exceed 100!\n");
			exit(EXIT_FAILURE);
		}

		calc_weights(weights, mwlen);
		free(wvalue);
	}

	if (cvalue != NULL)
		free(cvalue);

	if (max != 0)
		pwdlen = rrand(min, max);
	else if (len != 0)
		pwdlen = len;
	else
		pwdlen = rrand(6, 18);

	char *pwd;

	if (mass)
	{
		int *mass_sizes = generate_mass_sizes(mass, mwlen);

		if (weights == NULL)
			weights = int_mass_to_double(mass_sizes, mwlen);

		pwd = generate_pwd2(mass, weights, mass_sizes, mwlen, pwdlen);

		free(mass);
		free(mass_sizes);
	}
	else
	{
		if (weights == NULL)
			weights = generate_weights(mwlen);

		pwd = generate_pwd1(alphabet, weights, mwlen, pwdlen);
		free(alphabet);
	}

	free(weights);

	printf("Password: %s\n", pwd);

	return 0;
}
