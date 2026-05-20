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

#define CONCAT(a, b) a##b
#define CONCAT_EXPAND(a, b) CONCAT(a, b)
#define UNIQUE_NAME(base) CONCAT_EXPAND(base, __LINE__)
#define UNUSED(x) (void)(x)
#define MstrFmt(x) _3_$("%.*s", (x)->length, (x)->raw)
#define MstrViewFmt(x) _3_$("%.*s", (x).length, (x).raw)

bool starts_with(const char *str, const char *prefix) {
	size_t len_prefix = strlen(prefix);
	size_t len_str = strlen(str);
	if (len_prefix > len_str) return false;
	return strncmp(str, prefix, len_prefix) == 0;
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
#endif
