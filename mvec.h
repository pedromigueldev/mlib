#ifndef MVEC_H
#define MVEC_H
#include "mlib.h"

#ifndef MVECALLOC
	#define MVECALLOC(bytes) malloc(bytes)
#endif
#ifndef MVECREALLOC
	#define MVECREALLOC(ptr, bytes) realloc((ptr), (bytes))
#endif

#define MVec(ptr) ptr##_arr
#define MVecCap(ptr) ptr##_arr_cap
#define MVecLen(ptr) ptr##_arr_len
#define MVecItemSize(ptr) sizeof((MVec(ptr))[0])

#define MVecAlloc(ptr, bytes)						\
	MVec(ptr) = MVECALLOC((bytes) * sizeof(MVec(ptr)[0])); 	\
	size_t MVecCap(ptr) = (bytes);							\
	size_t MVecLen(ptr) = 0

#define MVecAllocDefault(ptr) MVecAlloc(ptr, 1000)

#define MVecForeach(item, ptr)\
	if (MVecLen(ptr) > 0)																			\
		for(int __k = 1, index = 0; __k && index < (int) MVecLen(ptr); __k = !__k, index++)   \
			for(__typeof__(&(MVec(ptr))[0]) item = &(MVec(ptr))[index]; __k; __k = !__k)	

#define MVecRef(ptr) MVec(ptr), &MVecLen(ptr), &MVecCap(ptr)
#define MVecDef(ptr) MVec(ptr), size_t* MVecLen(ptr), size_t* MVecCap(ptr)    

#define MVecSet(vec, index, data) \
    do {\
        if (index >= MVecLen(vec)) {\
            errno = ERANGE;\
            fprintf(stderr, "Error setting index %zu (length: %zu): %s\n",\
                    index, MVecLen(vec), strerror(ERANGE));\
            exit(69);\
        }\
        MVec(vec)[index] = (data);\
    } while (0);
    
#define MVecPush(vec, data)                                      \
    do {                                                          \
        if (MVecLen(vec) >= MVecCap(vec)) {                       \
            MVecCap(vec) = MVecCap(vec) ? MVecCap(vec) * 2 : 1;  \
            MVec(vec) = MVECREALLOC(                                  \
                MVec(vec),                                        \
                MVecCap(vec) * sizeof(MVec(vec)[0])               \
            );                                                    \
            if (!MVec(vec)) {\
                fprintf(stderr, "Out of memory\n");\
                exit(1);\
            }\
        }                                                         \
        MVec(vec)[MVecLen(vec)++] = (data);                       \
    } while (0)
 
#define MVecGet(vec, index) ({\
        if (index >= MVecLen(vec)) {\
            errno = ERANGE;\
            fprintf(stderr, "Error getting index %zu (length: %zu): %s\n",\
                    index, MVecLen(vec), strerror(ERANGE));\
            exit(69);\
        }\
        &MVec(vec)[index];\
    })

#define MVecFree(vec)      \
    do {                   \
        free(MVec(vec));   \
        MVec(vec) = NULL;  \
        MVecLen(vec) = 0;  \
        MVecCap(vec) = 0;  \
    } while (0)
    
void* MVecPoolAlloc(char** MVecDef(pool), size_t size)
{
    size_t needed = *pool_arr_len + size;

    if (needed > *pool_arr_cap) {
        size_t new_cap = *pool_arr_cap ? *pool_arr_cap * 2 : 1;
        while (new_cap < needed)
            new_cap *= 2;

        *pool_arr_cap = new_cap;

        *pool_arr = MVECREALLOC(
            *pool_arr,
            *pool_arr_cap * MVecItemSize(*pool)
        );
        
        if (!*pool_arr) {
            fprintf(stderr, "Not enough memory\n");
            exit(420);
        }
    }

    size_t start = *pool_arr_len;
    *pool_arr_len += size;
    return MVec(*pool) + start;
}

static inline void* MVecPoolPop(char** MVecDef(pool), size_t size) {
	UNUSED(MVecCap(pool));
    if (size > *pool_arr_len) {
        *pool_arr_len = 0;
    } else {
        *pool_arr_len -= size;
    }
    
    return MVec(*pool) + *pool_arr_len;
}

#endif
