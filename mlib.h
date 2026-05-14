#ifndef MLIB
#define MLIB

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

#define ENSURE_NOT_NULL(ptr)                  \
        do {                                      \
            if ((ptr) == NULL) {                  \
                fprintf(stderr, "Error message not defined on error branch %s:%d\n",__FILE__, __LINE__);\
                exit(EXIT_FAILURE);               \
            }                                     \
        } while (0)
        
#define take_ptr(ptr)                    \
    ({                                   \
        ENSURE_NOT_NULL(ptr);            \
        ptr;                             \
    })
    
#define catch(fun, out, ...)                                            \
    for (char *__a = NULL; __a == NULL; __a = (char*)1)  \
        if(fun##_tryfail((out), __VA_ARGS__) && take_ptr(out))

#define wrap_fail(out) *(char**)(out)
#define unwrap_fail(out) (char*)(out)

bool starts_with(const char *str, const char *prefix) {
	size_t len_prefix = strlen(prefix);
	size_t len_str = strlen(str);
	if (len_prefix > len_str) return false;
	return strncmp(str, prefix, len_prefix) == 0;
}


void MPRINT_FMT_LN(const char *first, ...)
{
	va_list args;
	va_start(args, first);

	while (1) {
		const char* fmt = va_arg(args, const char*);
		if (fmt == NULL) break;
		if (starts_with(fmt, "%.*")) {
		    int len = va_arg(args, int);       // first argument for %.*
            char* str = va_arg(args, char*);   // second argument for %.*
            fprintf(stdout, fmt, len, str);
		}
		else if (starts_with(fmt, "%")) {
			void* variable = va_arg(args, void*);
			fprintf(stdout, fmt, variable);
		} else {
			fprintf(stdout, fmt);
		}
	}
	va_end(args);
}

#define _ARG_G(x) _Generic((x), \
            int: "%d", \
            long: "%ld", \
            long long: "%lld", \
            unsigned int: "%u", \
            unsigned long: "%lu", \
            unsigned long long: "%llu", \
            float: "%f", \
            double: "%f", \
            long double: "%Lf", \
            char: "%c", \
            char*: "%s", \
            const char*: "%s", \
            void*: "%p", \
            default: "%p")

#define _2_$(fmt,var) ,(fmt),(var),""
#define _3_$(fmt,len,var) ,(fmt),(len),(var),""
#define $(x) _2_$(_ARG_G(x), _Generic(x, bool: ((x) ? "true" : "false"), default: x))
#define MPRINT_FMT(...) MPRINT_FMT_LN("", __VA_ARGS__, NULL)
#endif
