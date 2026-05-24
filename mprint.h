#ifndef MPRINT_H
#define MPRINT_H
#include "mstr.h"
#include "marr.h"

MstrView mprint_fmt_ln_v(MByteArray** pool, const char *first, va_list args) {
	size_t start = (*pool)->len;
	size_t start_len = 0;
	
	va_list args_copy;
	va_copy(args_copy, args);

	const char* fmt = first;
	while (fmt != NULL) {
		if (starts_with(fmt, "%.*")) {
            int len = va_arg(args_copy, int);
            char* str = va_arg(args_copy, char*);
        
			int needed = snprintf(NULL, 0, fmt, len, str);
			if (needed > 0) {
				start_len += needed;
				size_t length = (size_t)needed;
				char* handle = MByteArrayAlloc(pool, length + 1);
				snprintf(handle, length + 1, fmt,  len, str);
				(*pool)->len--;
			}
        }
		else if (starts_with(fmt, "%")) {
            void* variable = va_arg(args_copy, void*);
        
            int needed = snprintf(NULL, 0, fmt, variable);
            if (needed > 0) {
				start_len += needed;
   				size_t length = (size_t)needed;
	            char* handle = MByteArrayAlloc(pool, length + 1);
	            snprintf(handle, length + 1, fmt, variable);
				(*pool)->len--;
            }
        } else {
		    int needed = snprintf(NULL, 0, "%s", fmt);
		    if (needed > 0) {
				start_len += needed;
		    	size_t length = (size_t)needed;
    		    char* handle = MByteArrayAlloc(pool, length);
    			memcpy(handle, fmt, length);
		    }
		}
		
		fmt = va_arg(args_copy, const char*);
	}
	
	va_end(args_copy);
	return ((MstrView) {.raw = pool, .start = start, .length= start_len });
}

MstrView mstr_fmt_ln(MByteArray** pool, const char *first, ...) {
	va_list args;
	va_start(args, first);
	MstrView print_b = mprint_fmt_ln_v(pool, first, args);
	va_end(args);
	return print_b;
}

int mprint_fmt_ln(const char *first, ...) {
    MByteArray* pool = MByteArrayMalloc(1000);
	va_list args;
	va_start(args, first);
	MstrView print_b = mprint_fmt_ln_v(&pool, first, args);
	int res = printf("%.*s\n", (int)print_b.length, MViewRaw(print_b));
	free(pool);
	va_end(args);

	return res;
}

#define MPrintFmt(...) mprint_fmt_ln(""__VA_ARGS__, NULL)
#define MStrFmt(pool,...) mstr_fmt_ln(pool, ""__VA_ARGS__, NULL)

#endif
