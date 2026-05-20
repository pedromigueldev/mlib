#ifndef MERRVAL_H
#define MERRVAL_H
#include "mlib.h"
#define CONCAT(a, b) a##b
#define CONCAT_EXPAND(a, b) CONCAT(a, b)
#define UNIQUE_NAME(base) CONCAT_EXPAND(base, __LINE__)

#define MRetErrDefine(type, err, retname) typedef struct {\
        type val;\
        err error;\
    } retname; retname

#define MRetDefine(type, retname) typedef struct {\
        type val;\
        bool error;\
    } retname; retname

#define MRetValue(RetErrVal_t, x) ((RetErrVal_t) {\
	    .val = (x)\
	})

#define MRetError(RetErrVal_t, x) ((RetErrVal_t) {\
        .error = x,\
	})

#define MRet(type)\
    struct {\
        type val;\
        char* error;\
    }
    
#define AutoType __auto_type
    
#define MRetEither(var, varErr, __functionExp) \
    __auto_type UNIQUE_NAME(temp) = __functionExp;\
    __auto_type varErr = UNIQUE_NAME(temp).error;\
    __auto_type var = UNIQUE_NAME(temp).val; UNUSED(var)
    
#define MRetUnwrapOr(value, PROVIDE_FALLBACK_VALUE) value.error != 0 ? (__typeof(value.val))PROVIDE_FALLBACK_VALUE : value.val

#endif
