#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>

#include "Utils.h"

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

double parse_weight(char **p)
{
	int number = 0;

	while (**p != ',' && **p != '\0') 
	{
		if (**p == ' ' || **p == '\t')
		{
			(*p)++;
			continue;
		}

		if (!isdigit(**p))
		{
			fprintf(stderr, "Parsing error: didn't expect '%c' character at weights line!\n", **p);
			exit(EXIT_FAILURE);
		}

		number *= 10;
		number += **p - '0';

		if (number > 100)
		{
			fprintf(stderr, "Parsing error: weight can't exceed 100!\n");
			exit(EXIT_FAILURE);
		}

		(*p)++;
	}

	if (**p == '\0')
		(*p)--;

	return (double) number;
}

int parse_index(char **p)
{
	int idx = 0;

	while (**p != '}') 
	{
		if (!isdigit(**p))
		{
			fprintf(stderr, "Parsing error: didn't expect '%c' character at weights line!\n", **p);
			exit(EXIT_FAILURE);
		}

		if ((idx > INT_MAX / 10) || (idx * 10 > INT_MAX - (**p - '0')))
		{
			fprintf(stderr, "Parsing error: Index integer overflow!\n");
			exit(EXIT_FAILURE);
		}

		idx *= 10;
		idx += **p - '0';

		(*p)++;
	}

	(*p)++;

	if (**p != ':')
	{
		fprintf(stderr, "Parsing error: expected ':' after '}'!\n");
		exit(EXIT_FAILURE);
	}

	(*p)++;

	return idx;
}

void parse_range(char **p, int *_idx1, int *_idx2)
{
	int idx1 = 0;
	int idx2 = 0;

	while (**p != '-') 
	{
		if (!isdigit(**p))
		{
			fprintf(stderr, "Parsing error: didn't expect '%c' character at weights line!\n", **p);
			exit(EXIT_FAILURE);
		}

		if ((idx1 > INT_MAX / 10) || (idx1 * 10 > INT_MAX - (**p - '0')))
		{
			fprintf(stderr, "Parsing error: First index integer overflow!\n");
			exit(EXIT_FAILURE);
		}

		idx1 *= 10;
		idx1 += **p - '0';

		(*p)++;
	}

	(*p)++;

	while (**p != ']') 
	{
		if (!isdigit(**p))
		{
			fprintf(stderr, "Parsing error: didn't expect '%c' character at weights line!\n", **p);
			exit(EXIT_FAILURE);
		}

		if ((idx2 > INT_MAX / 10) || (idx2 * 10 > INT_MAX - (**p - '0')))
		{
			fprintf(stderr, "Parsing error: Second index integer overflow!\n");
			exit(EXIT_FAILURE);
		}

		idx2 *= 10;
		idx2 += **p - '0';

		(*p)++;
	}

	(*p)++;

	if (**p != ':')
	{
		fprintf(stderr, "Parsing error: expected ':' after ']'!\n");
		exit(EXIT_FAILURE);
	}
	
	(*p)++;

	*_idx1 = idx1;
	*_idx2 = idx2;
}

double* parse_weights(char *ws, int wlen)
{
	double *weights = generate_empty_weights(wlen);

	int last_index = 0;
	double sum = 0;

	for (char *p = ws; *p != '\0'; p++) 
	{
		if (*p == ' ' || *p == '\t')
			continue;

		if (*p >= '0' && *p <= '9')
		{
			double weight = parse_weight(&p);

			if (last_index >= wlen)
			{
				fprintf(stderr, "Parsing error: no such index '%d' in alphabet!\n", last_index);
				exit(EXIT_FAILURE);
			}

			weights[last_index++] = weight;
			sum += weight;
		}
		else if (*p == '{')
		{
			p++;

			int idx = parse_index(&p);
			double weight = parse_weight(&p);

			if (idx >= wlen)
			{
				fprintf(stderr, "Parsing error: no such index '%d' in alphabet!\n", idx);
				exit(EXIT_FAILURE);
			}

			weights[idx] = weight;
			last_index = idx + 1;
			sum += weight;
		}
		else if (*p == '[')
		{
			p++;

			int idx1, idx2;
			parse_range(&p, &idx1, &idx2);
			double weight = parse_weight(&p);

			if (idx1 > idx2)
			{
				fprintf(stderr, "Parsing error: wrong range '[%d-%d]'!\n", idx1, idx2);
				exit(EXIT_FAILURE);
			}

			if (idx1 >= wlen || idx2 >= wlen)
			{
				fprintf(stderr, "Parsing error: no such range '[%d-%d]' in alphabet!\n", idx1, idx2);
				exit(EXIT_FAILURE);
			}

			double tmp = 0;

			for (int i = idx1; i <= idx2; ++i) 
			{
				weights[i] = (weight - tmp) / (idx2 - i + 1);
				tmp += weights[i];
			}

			last_index = idx2 + 1;
			sum += weight;
		}
		else
		{
			fprintf(stderr, "Parsing error: didn't expect '%c' character at weights line!\n", *p);
			exit(EXIT_FAILURE);
		}

		if (sum > 100)
		{
			fprintf(stderr, "Parsing error: sum of weights can't exceed 100!\n");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < wlen; ++i)
	{
		if (weights[i] != -1)
			continue;

		weights[i] = (100 - sum) / (wlen - i);
		sum += weights[i];
	}

	return weights;
}

int parse_alphabet_from_file(const char *filename, char **_alphabet, double **_weights)
{
	FILE *in = fopen(filename, "r");
	
	if (in == NULL)
	{
		fprintf(stderr, "Couldn't open file '%s': %s!\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	char *alphabet = NULL;
	size_t an = 0;
	int alen;

	if ((alen = getline(&alphabet, &an, in)) == -1)
	{
		fprintf(stderr, "Couldn find alphabet in file!\n");
		exit(EXIT_FAILURE);
	}

	alen--;
	alphabet[alen] = '\0';

	char *ws = NULL;
	size_t wsn = 0;
	int wslen;

	double *weights = NULL;

	if ((wslen = getline(&ws, &wsn, in)) == -1)
		weights = generate_weights(alen);
	else
	{
		ws[wslen - 1] = '\0';
		weights = parse_weights(ws, alen);
	}

	free(ws);

	*_alphabet = alphabet;
	*_weights = weights;

	fclose(in);

	return alen;
}

#define get_value(name)                                                     \
	{                                                                       \
		if (i + 1 > argc || (i + 1 <= argc && argv[i+1][0] == '-'))         \
		{                                                                   \
			fprintf(stderr, "No value given for argument '-" #name "'!\n"); \
			exit(EXIT_FAILURE);                                             \
		}                                                                   \
		name##_value = argv[++i];                                           \
	}

#define check_repeat(name)                                               \
	if ((name##_flag) == 1)                                              \
	{                                                                    \
		fprintf(stderr, "Argument '" #name "' is already specified!\n"); \
		exit(EXIT_FAILURE);                                              \
	}

#define check_repeat_sub(name, subname)                                                           \
	if ((name##subname##_flag) == 1)                                                              \
	{                                                                                             \
		fprintf(stderr, "Option '" #subname "' of argument '" #name "' is already specified!\n"); \
		exit(EXIT_FAILURE);                                                                       \
	}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "No arguments given (waits at least for 1 specified argument with value)!\n");
		exit(EXIT_FAILURE);
	}

	char m1_flag = 0;
	char m2_flag = 0;
	char n_flag = 0;
	char a_flag = 0;
	char f_flag = 0;
	char c_flag = 0;

	int ca_flag = 0;
	int cA_flag = 0;
	int cD_flag = 0;
	int cS_flag = 0;

	char wa_flag = 0;
	char wA_flag = 0;
	char wD_flag = 0;
	char wS_flag = 0;

	char *m1_value = NULL;
	char *m2_value = NULL;
	char *n_value = NULL;
	char *a_value = NULL;
	char *f_value = NULL;
	char *c_value = NULL;

	char *wa_value = NULL;
	char *wA_value = NULL;
	char *wD_value = NULL;
	char *wS_value = NULL;

	srand(time(0));

	for (int i = 1; i < argc; ++i) 
	{
		if (argv[i][0] != '-')
		{
			fprintf(stderr, "Wrong argument '%s'!\n", argv[i]);
			exit(EXIT_FAILURE);
		}

		switch (argv[i][1]) 
		{
			case 'm':
				if (argv[i][2] == '\0')
				{
					fprintf(stderr, "No such argument '-m'!\n");
					exit(EXIT_FAILURE);
				}

				switch (argv[i][2]) 
				{
					case '1':
						check_repeat(m1);
						m1_flag = 1;
						get_value(m1);

						break;
					case '2':
						check_repeat(m2);
						m2_flag = 1;
						get_value(m2);

						break;
					default:
						fprintf(stderr, "No such argument '-m%c'!\n", argv[i][2]);
						exit(EXIT_FAILURE);
				}
				break;
			case 'n':
				check_repeat(n);
				n_flag = 1;
				get_value(n);

				break;
			case 'a':
				check_repeat(a);
				a_flag = 1;
				get_value(a);

				break;
			case 'f':
				check_repeat(f);
				f_flag = 1;
				get_value(f);

				break;
			case 'c':
				check_repeat(c);

				c_flag = 1;

				if (argv[i][2] == '\0')
				{
					get_value(c);
				}
				else
					c_value = &argv[i][2];

				for (char *p = c_value; *p != '\0'; p++) 
				{
					switch (*p) {
						case 'a':
							check_repeat_sub(c, a);
							ca_flag = 1;

							break;
						case 'A':
							check_repeat_sub(c, A);
							cA_flag = 1;

							break;
						case 'D':
							check_repeat_sub(c, D);
							cD_flag = 1;

							break;
						case 'S':
							check_repeat_sub(c, S);
							cS_flag = 1;

							break;
						default:
							fprintf(stderr, "No such option '%c' of argument '-c'!\n", *p);
							exit(EXIT_FAILURE);

							break;
					}
				}

				break;
			case 'w':
				switch (argv[i][2]) 
				{
					case 'a':
						check_repeat(wa);
						wa_flag = 1;
						get_value(wa);

						break;
					case 'A':
						check_repeat(wA);
						wA_flag = 1;
						get_value(wA);

						break;
					case 'D':
						check_repeat(wD);
						wD_flag = 1;
						get_value(wD);
						
						break;
					case 'S':
						check_repeat(wS);
						wS_flag = 1;
						get_value(wS);

						break;
					default:
						fprintf(stderr, "No such argument '-w%c'!\n", argv[i][2]);
						exit(EXIT_FAILURE);
				}

				break;
			default:
				fprintf(stderr, "No such argument '-%c'!\n", argv[i][1]);
				exit(EXIT_FAILURE);
		}
	}

	if ((m1_flag || m2_flag) && n_flag)
	{
		fprintf(stderr, "Arguments '-m1' and '-m2' can't be specified with '-n' at the same time!\n");
		exit(EXIT_FAILURE);
	}

	if ((f_flag || a_flag) && (c_flag || wa_flag || wA_flag || wS_flag || wD_flag))
	{
		fprintf(stderr, "Arguments '-f' and '-a' can't be specified with '-c' and '-wa'/'-wA'/'-wD'/'-wS' at the same time!\n");
		exit(EXIT_FAILURE);
	}

	if ((m1_flag && !m2_flag) || (!m1_flag && m2_flag))
	{
		fprintf(stderr, "Arguments '-m1' and '-m2' should be specified together!\n");
		exit(EXIT_FAILURE);
	}

	if ((!ca_flag && wa_flag))
	{
		fprintf(stderr, "Argument '-wa' can't be specified when option 'a' of argument '-c' is not specified!\n");
		exit(EXIT_FAILURE);
	}

	if (!cA_flag && wA_flag)
	{
		fprintf(stderr, "Argument '-wA' can't be specified when option 'A' of argument '-c' is not specified!\n");
		exit(EXIT_FAILURE);
	}

	if (!cD_flag && wD_flag)
	{
		fprintf(stderr, "Argument '-wD' can't be specified when option 'D' of argument '-c' is not specified!\n");
		exit(EXIT_FAILURE);
	}

	if (!cS_flag && wS_flag)
	{
		fprintf(stderr, "Argument '-wS' can't be specified when option 'S' of argument '-c' is not specified!\n");
		exit(EXIT_FAILURE);
	}

	int pwdlen = 0;
	char *alphabet = NULL;
	const char **mass = NULL;
	double *weights = NULL;
	int mwlen = 0;

	if (m1_flag && m2_flag)
	{
		int m1_number = strtoint(m1_value, 10, NULL);
		char *tmp_m1 = inttostr(m1_number, 10, NULL);

		if (strlen(m1_value) != strlen(tmp_m1))
		{
			fprintf(stderr, "Value of '-m1' argument is not a number!\n");
			exit(EXIT_FAILURE);
		}

		free(tmp_m1);

		int m2_number = strtoint(m2_value, 10, NULL);
		char *tmp_m2 = inttostr(m2_number, 10, NULL);

		if (strlen(m2_value) != strlen(tmp_m2))
		{
			fprintf(stderr, "Value of '-m2' argument is not a number!\n");
			exit(EXIT_FAILURE);
		}

		free(tmp_m2);

		if (m1_number >= m2_number)
		{
			fprintf(stderr, "Value of '-m1' should be bigger than value of '-m2'!\n");
			exit(EXIT_FAILURE);
		}

		pwdlen = rrand(m1_number, m2_number);
	} 
	else if (n_flag)
	{
		int n_number = strtoint(n_value, 10, NULL);
		char *tmp_n = inttostr(n_number, 10, NULL);

		if (strlen(n_value) != strlen(tmp_n))
		{
			fprintf(stderr, "Value of '-n' argument is not a number!\n");
			exit(EXIT_FAILURE);
		}
		
		free(tmp_n);
		pwdlen = n_number;
	}
	else
		pwdlen = rrand(6, 18);

	if (a_flag)
	{
		mwlen = strlen(a_value);
		alphabet = a_value;
		weights = generate_weights(mwlen);
	}

	if (!a_flag && !c_flag && !f_flag)
	{
		c_flag = 1;
		ca_flag = 1;
		cA_flag = 1;
		cD_flag = 1;
	}

	if (c_flag)
	{
		mwlen = ca_flag + cA_flag + cD_flag + cS_flag;

		mass = (const char**)malloc(mwlen * sizeof(char*));
		assert(mass != NULL);

		int last_index = 0;

		int w_flags = wa_flag + wA_flag + wD_flag + wS_flag;
		if (w_flags != 0)
			weights = generate_empty_weights(mwlen);

		int sum = 0;

		if (ca_flag)
		{
			mass[last_index] = _lowercase;
			
			if (wa_flag)
			{
				int wa_number = strtoint(wa_value, 10, NULL);
				char *tmp_wa = inttostr(wa_number, 10, NULL);

				if (strlen(wa_value) != strlen(tmp_wa))
				{
					fprintf(stderr, "Value of '-wa' argument is not a number!\n");
					exit(EXIT_FAILURE);
				}

				free(tmp_wa);
				weights[last_index] = wa_number;
				sum += wa_number;
			}

			last_index++;
		}

		if (cA_flag)
		{
			mass[last_index] = _uppercase;

			if (wA_flag)
			{
				int wA_number = strtoint(wA_value, 10, NULL);
				char *tmp_wA = inttostr(wA_number, 10, NULL);

				if (strlen(wA_value) != strlen(tmp_wA))
				{
					fprintf(stderr, "Value of '-wA' argument is not a number!\n");
					exit(EXIT_FAILURE);
				}

				free(tmp_wA);
				weights[last_index] = wA_number;
				sum += wA_number;
			}

			last_index++;
		}

		if (cD_flag)
		{
			mass[last_index] = _numbers;

			if (wD_flag)
			{
				int wD_number = strtoint(wD_value, 10, NULL);
				char *tmp_wD = inttostr(wD_number, 10, NULL);

				if (strlen(wD_value) != strlen(tmp_wD))
				{
					fprintf(stderr, "Value of '-wD' argument is not a number!\n");
					exit(EXIT_FAILURE);
				}

				free(tmp_wD);
				weights[last_index] = wD_number;
				sum += wD_number;
			}

			last_index++;
		}

		if (cS_flag)
		{
			mass[last_index] = _special;

			if (wS_flag)
			{
				int wS_number = strtoint(wS_value, 10, NULL);
				char *tmp_wS = inttostr(wS_number, 10, NULL);

				if (strlen(wS_value) != strlen(tmp_wS))
				{
					fprintf(stderr, "Value of '-wS' argument is not a number!\n");
					exit(EXIT_FAILURE);
				}

				free(tmp_wS);
				weights[last_index] = wS_number;
				sum += wS_number;
			}

			last_index++;
		}

		if (sum > 100)
		{
			fprintf(stderr, "Sum of weights can't exceed 100!\n");
			exit(EXIT_FAILURE);
		}

		if (w_flags != 0)
			calc_weights(weights, mwlen);
	}

	if (f_flag)
		mwlen = parse_alphabet_from_file(f_value, &alphabet, &weights);

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
		pwd = generate_pwd1(alphabet, weights, mwlen, pwdlen);
		free(alphabet);
	}

	free(weights);

	printf("Generated password: %s\n", pwd);

	return 0;
}
