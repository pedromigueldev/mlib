#ifndef MERRVAL_H
#define MERRVAL_H

#define CONCAT(a, b) a##b
#define CONCAT_EXPAND(a, b) CONCAT(a, b)
#define UNIQUE_NAME(base) CONCAT_EXPAND(base, __LINE__)

typedef struct {
    char* val;
    char* error;
} RetErrVal_t;
    
#define MRetValue(x) (void*)&((RetErrVal_t) {\
        .error = NULL,\
	    .val = *(void**)(uintptr_t)&(x)\
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
    
#define MRetUnwrapOr(value, PROVIDE_FALLBACK_VALUE) ({\
    value->val == NULL ? (__typeof(value->val))PROVIDE_FALLBACK_VALUE : value->val;\
})

#endif
