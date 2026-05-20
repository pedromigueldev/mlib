#ifndef MPRINT_H
#define MPRINT_H
#include "mstr.h"

MstrView mprint_fmt_ln_v(MVecParamDefPtr(*pool, char), const char *first, va_list args) {
	size_t start_len = MVecLen(*pool);
	
	va_list args_copy;
	va_copy(args_copy, args);

	const char* fmt = first;
	while (fmt != NULL) {
		if (starts_with(fmt, "%.*")) {
            int len = va_arg(args_copy, int);
            char* str = va_arg(args_copy, char*);
        
            int needed = snprintf(NULL, 0, fmt, len, str);

            char* handle = MVecPoolAlloc(MVecParamRefPtr(pool), needed);
            snprintf(handle, needed + 1, fmt, len, str);
        }
		else if (starts_with(fmt, "%")) {
            void* variable = va_arg(args_copy, void*);
        
            int needed = snprintf(NULL, 0, fmt, variable);
        
            char* handle = MVecPoolAlloc(MVecParamRefPtr(pool), needed);
            snprintf(handle, needed + 1, fmt, variable);
        } else {
		    int needed = snprintf(NULL, 0, "%s", fmt);
		    if (needed > 0) {
    		    char* handle = MVecPoolAlloc(MVecParamRefPtr(pool), needed);
    			memcpy(handle, fmt, needed + 1);
		    }
		}
		
		fmt = va_arg(args_copy, const char*);
	}
	
	va_end(args_copy);
	return MstrViewFrom(MVec(*pool) + start_len, 0, (MVecLen(*pool) - start_len));
}

MstrView mstr_fmt_ln(MVecParamDefPtr(*pool, char), const char *first, ...) {
	va_list args;
	va_start(args, first);
	MstrView print_b = mprint_fmt_ln_v(MVecParamRefPtr(pool), first, args);
	va_end(args);
	return print_b;
}

void mprint_fmt_ln(const char *first, ...) {
    MVecAlloc(pool, char, 1000);
	va_list args;
	va_start(args, first);
	MstrView print_b = mprint_fmt_ln_v(MVecParamRefPtr(&pool), first, args);
	printf("%.*s\n", (int)print_b.length, print_b.raw);
	free(MVec(pool));
	va_end(args);
}

#define MPrintFmt(...) mprint_fmt_ln(""__VA_ARGS__, NULL)
#define MStrFmt(pool,...) mstr_fmt_ln(MVecParamRefPtr(pool), ""__VA_ARGS__, NULL)

#endif
