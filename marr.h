#ifndef MARR_H
#define MARR_H
#include <stdint.h>
#include <stddef.h>
#include "mlib.h"

typedef struct {
	size_t length;
	size_t capacity;
	uintptr_t items[];
} MarrImpl;

#define MarrImplFrom(arr) ((MarrImpl*)((uintptr_t)(arr) - sizeof(MarrImpl)))
#define MarrVerifyInit(some) \
		do {\
			if (some == NULL) {\
			    size_t cap = 256 * sizeof(int);\
			    MarrImpl* arr = malloc(sizeof(MarrImpl) + cap);\
				arr->capacity = cap;\
				arr->length = 0;\
				some = (typeof(some))arr->items;\
			}\
			else if ((MarrImplFrom(some)->length+1) == MarrImplFrom(some)->capacity / sizeof(some[0])) {\
			    MarrImpl* arr = MarrImplFrom(some);\
                size_t new_capacity = arr->capacity * 2; \
			    arr = realloc(arr, sizeof(MarrImpl) + new_capacity); \
                if (arr != NULL) { \
                    arr->capacity = new_capacity; \
                    some = (typeof(some))arr->items; \
                } \
			}\
		} while(0)
		
#define MarrPut(some, value)\
    do {\
		MarrVerifyInit(some);\
		MarrImplFrom(some)->length++;\
		some[MarrImplFrom(some)->length-1] = value;\
    } while (0)
    
#define MarrLength(some) (some ? MarrImplFrom(some)->length : 0)

#define MarrForeach(item, ptr)\
	if (MarrLength(ptr) > 0)																			\
		for(int __k = 1, index = 0; __k && index < (int) MarrLength(ptr); __k = !__k, index++)   \
			for(typeof (ptr[0]) item = (ptr)[index]; __k; __k = !__k)
#endif
