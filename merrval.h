#ifndef MERRVAL_H
#define MERRVAL_H
#include "mlib.h"
#include <errno.h>

#define CONCAT(a, b) a##b
#define CONCAT_EXPAND(a, b) CONCAT(a, b)
#define UNIQUE_NAME(base) CONCAT_EXPAND(base, __LINE__)

#define MRetDefine(type, retname) typedef struct {\
        type val;\
        int error;\
    } retname; retname

#define MRetError(RetErrVal_t) ((RetErrVal_t) {\
        .error = errno,\
	})
	
#define MRetValue(RetErrVal_t, x) ((RetErrVal_t) {\
	    .val = (x),\
	    .error = 0\
	})
    
#define MRetEither(var, varErr, __functionExp) \
    __auto_type UNIQUE_NAME(temp) = __functionExp;\
    __auto_type varErr = UNIQUE_NAME(temp).error;\
    __auto_type var = UNIQUE_NAME(temp).val; UNUSED(var)
    
#define MRetUnwrapOr(value, PROVIDE_FALLBACK_VALUE) value.error ? (__typeof(value.val))PROVIDE_FALLBACK_VALUE : value.val

#endif
