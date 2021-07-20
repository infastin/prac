#ifndef ANY_H_5TDF2LDA
#define ANY_H_5TDF2LDA

#include <stddef.h>

typedef void (*any_free_func)(void *ptr);

typedef enum
{
	CHAR_TYPE,
	INT_TYPE,
	UINT_TYPE,
	STR_TYPE,
	PTR_TYPE
} data_t;

typedef struct
{
	void *data[2];
	data_t type;
} any_t;

void   any_init(any_t *any, data_t type, const void *value);
any_t* any_alloc(data_t type, const void *value);
void   any_get(any_t *any, void *ret);
void   any_free_data(any_t *any, any_free_func ff);

#endif /* end of include guard: ANY_H_5TDF2LDA */
