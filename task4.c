#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Sort.h"

#define at_end(a, orig_a, elemsize) ((*(a) == '\0') || ((a) == ((orig_a) + elemsize)))

int strings_cmp1(const void *a, const void *b, size_t elemsize)
{
	const char *orig_a = a;
	const char *orig_b = b;

	const char *pa = a;
	const char *pb = b;

	while (1) 
	{
		if (at_end(pa, orig_a, elemsize) && !at_end(pb, orig_b, elemsize))
			return -1;
		
		if (!at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 1;
		
		if (at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 0;

		if (*pa < *pb)
			return -1;
		else if (*pa > *pb)
			return 1;

		pa++;
		pb++;
	}
}

int strings_cmp2(const void *a, const void *b, size_t elemsize)
{
	const char *orig_a = a;
	const char *orig_b = b;

	const char *pa = a;
	const char *pb = b;

	char tmp_a;
	char tmp_b;

	while (1) 
	{
		if (at_end(pa, orig_a, elemsize) && !at_end(pb, orig_b, elemsize))
			return -1;

		if (!at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 1;

		if (at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 0;

		if (isalpha(*pa) && isalpha(*pb))
		{
			tmp_a = toupper(*pa);
			tmp_b = toupper(*pb);
		}
		else
		{
			tmp_a = *pa;
			tmp_b = *pb;
		}

		if (tmp_a < tmp_b)
			return -1;
		else if (tmp_a > tmp_b)
			return 1;

		pa++;
		pb++;
	}
}

int strings_cmp3(const void *a, const void *b, size_t elemsize)
{
	const char *orig_a = a;
	const char *orig_b = b;

	const char *pa = a;
	const char *pb = b;

	while (1) 
	{
		if (at_end(pa, orig_a, elemsize) && !at_end(pb, orig_b, elemsize))
			return -1;

		if (!at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 1;

		if (at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 0;

		if (*pa > *pb)
			return -1;
		else if (*pa < *pb)
			return 1;

		pa++;
		pb++;
	}
}

int strings_cmp4(const void *a, const void *b, size_t elemsize)
{
	const char *orig_a = a;
	const char *orig_b = b;

	const char *pa = a;
	const char *pb = b;

	char tmp_a;
	char tmp_b;

	while (1) 
	{
		if (at_end(pa, orig_a, elemsize) && !at_end(pb, orig_b, elemsize))
			return -1;

		if (!at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 1;

		if (at_end(pa, orig_a, elemsize) && at_end(pb, orig_b, elemsize))
			return 0;

		if (isalpha(*pa) && isalpha(*pb))
		{
			tmp_a = toupper(*pa);
			tmp_b = toupper(*pb);
		}
		else
		{
			tmp_a = *pa;
			tmp_b = *pb;
		}

		if (tmp_a > tmp_b)
			return -1;
		else if (tmp_a < tmp_b)
			return 1;

		pa++;
		pb++;
	}
}

int main(int argc, char *argv[])
{
	char buf[][10] = { "Zorro", "Alex", "Celine", "Bill", "Forest", "Dexter" };

	quicksort(buf, 6, 10, strings_cmp4);

	for (int i = 0; i < 6; ++i) 
	{
		printf("%s ", buf[i]);
	}

	return 0;
}
