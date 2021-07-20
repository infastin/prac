#include <memory.h>
#include <malloc.h>

#include "Any.h"

int _data_get_size(data_t type)
{
	size_t size;

	switch (type) 
	{
		case CHAR_TYPE:
			size = sizeof(char);
			break;
		case INT_TYPE:
			size = sizeof(int);
			break;
		case UINT_TYPE:
			size = sizeof(unsigned int);
			break;
		case STR_TYPE:
		case PTR_TYPE:
			size = sizeof(void*);
			break;
		default:
			size = 1;
			break;
	}

	return size;
}

void any_init(any_t *any, data_t type, const void *value)
{
	any->type = type;
	int size = _data_get_size(any->type);

	if (type == STR_TYPE)
	{
		char *str = strdup(value);
		memcpy(any->data, &str, size);
	}
	else
		memcpy(any->data, value, size);
}

any_t* any_alloc(data_t type, const void *value)
{
	any_t* any = malloc(sizeof(any_t));
	
	if (any == NULL)
		return NULL;

	any_init(any, type, value);

	return any;
}

void any_get(any_t *any, void *ret)
{
	int size = _data_get_size(any->type);
	memcpy(ret, any->data, size);
}

void any_free_data(any_t *any, any_free_func ff)
{
	if (ff != NULL)
		ff(*((void**) any->data));
}
