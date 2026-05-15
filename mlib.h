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
            }\
        } while (0)
        
#define take_ptr(ptr)                    \
    ({                                   \
        if ((ptr) == NULL) {                  \
            fprintf(stderr, "Error message not defined on error branch %s:%d\n",__FILE__, __LINE__);\
            exit(EXIT_FAILURE);               \
        };									\
        ptr;                             \
    })
    
#define end_loop_free(i,ptr)             \
    ({                              \
        if ((ptr) != NULL) {        \
            free((ptr));            \
            (ptr) = NULL;           \
        }                           \
        (i) = 1;                    \
    })
    
#define catch(fun, out, ...)                                            \
    for (int __i = 0; __i == 0; __i = 1)  \
        if(fun##_tryfail((out), __VA_ARGS__) && take_ptr(out)) \

#define wrap_fail(out) *(char**)(out)
#define unwrap_fail(out) (char*)(out)

bool starts_with(const char *str, const char *prefix) {
	size_t len_prefix = strlen(prefix);
	size_t len_str = strlen(str);
	if (len_prefix > len_str) return false;
	return strncmp(str, prefix, len_prefix) == 0;
}


char* MPRINT_FMT_LN(const char *first, ...)
{
	va_list args;
	va_start(args, first);
    
    char out_buffer[5000] = {0};
    size_t offset = 0;

	const char* fmt = first;    
	while (1) {;
		if (fmt == NULL) break;
		if (starts_with(fmt, "%.*")) {
		    int len = va_arg(args, int);       // first argument for %.*
            char* str = va_arg(args, char*);   // second argument for %.*
            offset += snprintf(out_buffer + offset, sizeof(out_buffer) - offset, fmt, len, str);
		}
		else if (starts_with(fmt, "%")) {
			void* variable = va_arg(args, void*);
    		offset += snprintf(out_buffer + offset, sizeof(out_buffer) - offset, fmt, variable);
		} else {
		     offset += snprintf(out_buffer + offset, sizeof(out_buffer) - offset, "%s", fmt);
		}
		
		fmt = va_arg(args, const char*);
	}
	
	va_end(args);
	return strdup(out_buffer);
}

void MPRINT_STDOUT(const char *first, ...) {
	va_list args;
	va_start(args, first);

	char* print_b = MPRINT_FMT_LN(first, args);
	puts(print_b);

	va_end(args);
	free(print_b);
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
#define MPRINT_FMT(...) MPRINT_STDOUT(""__VA_ARGS__, NULL)
#define MPRINT_FMT_OUT(...) MPRINT_FMT_LN(""__VA_ARGS__, NULL)
#endif
