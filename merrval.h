#ifndef MERRVAL_H
#define MERRVAL_H
#include "mlib.h"

#define MRetValue(x) (void*)&((RetErrVal_t) {\
        .error = NULL,\
	    .val = *(void**)(uintptr_t)&x\
	})

#define MRetError(x) (void*)&((RetErrVal_t) {\
        .error = x,\
	    .val = NULL\
	})

#define MRet(type)\
    struct {\
        type val;\
        char* error;\
    }*
    
#define MRetErr(type, err)\
    struct {\
        type val;\
        err error;\
    }*

#define MRetEither(var, varErr, __functionExp) \
    char* varErr = NULL; \
    __auto_type UNIQUE_NAME(temp) = __functionExp;\
    varErr = UNIQUE_NAME(temp)->error;\
    __auto_type var = UNIQUE_NAME(temp)->val;

#endif
