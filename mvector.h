#ifndef MVECTOR_H
#define MVECTOR_H
#include "mlib.h"

#define MVECALLOC(ptr, length) malloc((length) * sizeof(typeof(MVec(ptr)[0])))
#define MVECREALLOC(ptr, length) realloc(MVec(ptr), length);
#define MVec(ptr) ptr##_arr
#define MVecCap(ptr) ptr##_arr_cap
#define MVecLen(ptr) ptr##_arr_len

#define MVecAlloc(ptr, type, length)						\
	type* MVec(ptr) = MVECALLOC(ptr, length); 				\
	size_t MVecCap(ptr) = (length);							\
	size_t MVecLen(ptr) = 0

#define MVecPreAlloc(ptr, type) MVecAlloc(ptr, type, 1000);

#define MVecPush(ptr, value) \
	do {\
		if (MVecLen(ptr) >= MVecCap(ptr)) {\
			MVecCap(ptr) *= 2;\
			MVec(ptr) = MVECREALLOC(ptr, MVecCap(ptr) * sizeof((MVec(ptr))[0]));\
		}\
		(MVec(ptr))[(MVecLen(ptr))++] = value;\
	} while(0)

#define MVecForeach(item, ptr)\
	if (MVecLen(ptr) > 0)																			\
		for(int __k = 1, index = 0; __k && index < (int) MVecLen(ptr); __k = !__k, index++)   \
			for(typeof ((MVec(ptr))[0]) item = (MVec(ptr))[index]; __k; __k = !__k)	

#define MVecParamRefPtr(ptr) MVec(ptr), MVecLen(ptr), MVecCap(ptr)
#define MVecParamDefPtr(ptr, t) t* MVec(ptr), size_t MVecLen(ptr), size_t MVecCap(ptr)

#define MVecGet(ptr, index) ({\
    if (index >= MVecCap(ptr)) {\
        errno = ERANGE; \
        fprintf(stderr, "Error accessing index %d of array (capacity: %zu): %s\n", index, MVecCap(ptr), strerror(ERANGE));\
        exit(69);\
    }\
    MVec(ptr)[index];\
})

#define MVecSet(ptr, index, value) do {\
    if (index >= MVecCap(ptr)) {\
        errno = ERANGE; \
        fprintf(stderr, "Error setting at index %d of array (capacity: %zu): %s\n", index, MVecCap(ptr), strerror(ERANGE));\
        exit(420);\
    }\
    (MVec(ptr))[index] = value;\
    if (index >= MVecLen(ptr))\
        MVecLen(ptr) = index + 1;\
} while(0)
        

#endif
