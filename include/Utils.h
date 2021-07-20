#ifndef UTILS_H_K9XAX7EO
#define UTILS_H_K9XAX7EO

int strtoint(const char *str, int base, int *err_code, char **err_str);
unsigned int ustrtoint(const char *str, int base, int *err_code, char **err_str);
char* inttostr(int number, int base, int *err_code, char **err_str);
char* strdup_printf(const char *fmt, ...);
int has_char(const char *str, char ch);

#endif /* end of include guard: UTILS_H_K9XAX7EO */
