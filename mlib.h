#ifndef MLIB
#define MLIB

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>

#define __cleanup(func) __attribute__((__cleanup__(func)))
static inline void *__must_check_ptr(void *p) {
    return p;
}

#define no_free_ptr(p) __must_check_ptr(({ \
    __auto_type __ptr = (p); \
    (p) = NULL; \
    __ptr; \
}))
#define DEFINE_FREE(name, type, free_func) \
    static inline void __free_##name(void *p) { \
        type _T = *(type *)p; \
        if (_T) \
            free_func(_T); \
    }    
#define __free(name) __cleanup(__free_##name)

DEFINE_FREE(strfree, char*, free)

#define CONCAT(a, b) a##b
#define CONCAT_EXPAND(a, b) CONCAT(a, b)
#define UNIQUE_NAME(base) CONCAT_EXPAND(base, __LINE__)
#define UNUSED(x) (void)(x)
#define MstrFmt(x) _3_$("%.*s", (x)->length, (x)->raw)
#define MstrViewFmt(x) _3_$("%.*s", (int)(x).length, MViewRaw(x))

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
