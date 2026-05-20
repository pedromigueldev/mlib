#ifndef MVECTOR_H
#define MVECTOR_H
#include "mlib.h"
#include <stdint.h>

#define MVECALLOC(ptr, length) malloc((length) * sizeof(MVec(ptr)[0]))
#define MVECREALLOC(ptr, length) realloc(MVec(ptr), length);
#define MVec(ptr) ptr##_arr
#define MVecCap(ptr) ptr##_arr_cap
#define MVecLen(ptr) ptr##_arr_len
#define MVecItemSize(ptr) sizeof((MVec(ptr))[0])

#define MVecAlloc(ptr, type, length)						\
	type* MVec(ptr) = MVECALLOC(ptr, length); 				\
	size_t MVecCap(ptr) = (length);							\
	size_t MVecLen(ptr) = 0

#define MVecAllocDefault(ptr, type) MVecAlloc(ptr, type, 1000)

#define MVecForeach(item, ptr)\
	if (MVecLen(ptr) > 0)																			\
		for(int __k = 1, index = 0; __k && index < (int) MVecLen(ptr); __k = !__k, index++)   \
			for(__typeof__((MVec(ptr))[0]) item = (MVec(ptr))[index]; __k; __k = !__k)	

#define MVecParamRefPtr(ptr) MVec(ptr), MVecLen(ptr), MVecCap(ptr)
#define MVecParamDefPtr(ptr, t) t* MVec(ptr), size_t MVecLen(ptr), size_t MVecCap(ptr)

static inline void* MVecGetImpl(MVecParamDefPtr(*vec, uintptr_t), size_t index, size_t sizeofVecItem) {
	UNUSED(MVecCap(vec));
    if (index >= MVecLen(*vec)) {
        errno = ERANGE;
        fprintf(stderr, "Error accessing index %zu (length: %zu): %s\n",
                index, MVecLen(*vec), strerror(ERANGE));
        exit(69);
    }
    return (char*)*MVec(vec) + (index * sizeofVecItem);
}

static inline void* MVecSetImpl(MVecParamDefPtr(*vec, uintptr_t), size_t index, uintptr_t data, size_t sizeofVecItem) {
    if (index >= MVecLen(*vec)) {
        errno = ERANGE;
        fprintf(stderr, "Error setting index %zu (length: %zu): %s\n",
                index, MVecLen(*vec), strerror(ERANGE));
        exit(69);
    }
    
    char* target = MVecGetImpl(MVecParamRefPtr(vec), index, sizeofVecItem);
    memcpy(target, (void*)&data, sizeofVecItem);
    return target;
}

static inline void* MVecPushImpl(MVecParamDefPtr(*vec, uintptr_t), uintptr_t data, size_t sizeofVecItem) {
    if (MVecLen(*vec) >= MVecCap(*vec)) {
        MVecCap(*vec) *= 2;
		MVec(*vec) = MVECREALLOC(*vec, MVecCap(*vec) * sizeofVecItem);
    }
    (MVecLen(*vec)) += 1;

    char* target = MVecGetImpl(MVecParamRefPtr(vec), MVecLen(*vec) - 1, sizeofVecItem);
    memcpy(target, (void*)&data, sizeofVecItem);
    return target;
}


#define MVecPush(ptr, data) \
    *(__typeof__(MVec(ptr))) MVecPushImpl((uintptr_t**)MVecParamRefPtr(&ptr), (uintptr_t)data, MVecItemSize(ptr))
#define MVecGet(ptr, index) \
    *(__typeof__(MVec(ptr))) MVecGetImpl((uintptr_t**)MVecParamRefPtr(&ptr), index, MVecItemSize(ptr))
#define MVecSet(ptr, index, data) \
    *(__typeof__(MVec(ptr))) MVecSetImpl((uintptr_t**)MVecParamRefPtr(&ptr), index, (uintptr_t)data, MVecItemSize(ptr))

void* MVecPoolAlloc(MVecParamDefPtr(*pool, char), size_t size) {
    if (size >= MVecCap(*pool)) {
        MVecCap(*pool) *= 2 + size;
		MVec(*pool) = MVECREALLOC(*pool, MVecCap(*pool) *  MVecItemSize(*pool));
    }
    (MVecLen(*pool)) += size;
    
    return &MVecGet(*pool, MVecLen(*pool) - size);
}


#endif
