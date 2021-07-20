#ifndef ARGS_H_T4ZCEBA3
#define ARGS_H_T4ZCEBA3

#include "Any.h"

#define INIT_ARGR ((argr_t){NULL, NULL, 0})

typedef enum
{
	NO_VALUE,
	SHORT_VALUE, // example: -cADS, where -c is argument and ADS is value
	LONG_VALUE   // example: -c ADS, where -c is argument and ADS is value
} optvp_t;

typedef struct
{
	char *arg;
	optvp_t vpt; // Value position type
	data_t vdt;  // Value data type
	char shrt;   // One char representation of arg
	int conflict_number;
	int *conflicts;
} option_t;

typedef struct
{
	char *arg;
	char *carg;   // Conflicting arg
	any_t *value;
	char shrt;    // One char representation of arg
} argr_t;

int argcmp(const char *arg, const char *oarg, int short_value);
void parse_args_type(char *str, option_t *opt, argr_t *ret);
int parse_args(int argc, char *argv[], option_t *opts, int opts_number, argr_t *ret);

#endif /* end of include guard: ARGS_H_T4ZCEBA3 */
