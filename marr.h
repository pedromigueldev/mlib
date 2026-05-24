#ifndef MARR_H
#define MARR_H
#include "mlib.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
  #define WARN_UNUSED_RESULT [[__nodiscard__]]
#elif defined(__GNUC__) || defined(__clang__)
  #define WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#elif defined(_MSC_VER)
  #define WARN_UNUSED_RESULT _Check_return_
#else
  #define WARN_UNUSED_RESULT
#endif

#ifndef MVECALLOC
	#define MVECALLOC(bytes) malloc(bytes)
#endif
#ifndef MVECREALLOC
	#define MVECREALLOC(ptr, bytes) realloc((ptr), (bytes))
#endif
#ifndef MVECFREE
	#define MVECFREE(ptr) free(ptr)
#endif

#define MArrDefine(type, Name)                                                          \
    typedef struct {                                                                    \
        size_t len, cap;                                                                \
        type raw[];                                                                     \
    } Name;                                                                             \
    static inline Name* Name##Malloc(size_t size) {                                     \
        size_t struct_size = sizeof(Name) + (size * sizeof(type));                      \
        Name* temp = MVECALLOC(struct_size);                                            \
        memset(temp, 0, struct_size);                                                   \
        if (!temp) {                                                                    \
            fprintf(stderr, "Error creating "#Name"\n");                                \
            exit(69);                                                                   \
            return NULL;                                                                \
        }                                                                               \
        temp->cap = size;                                                               \
        return temp;                                                                    \
    }                                                                                   \
    static inline type* Name##PANIC(size_t index, size_t len) {                         \
        errno = ERANGE;                                                                 \
        fprintf(stderr, "Error accessing "#Name" index %zu (length: %zu): %s\n",        \
                index, len, strerror(ERANGE));                                          \
        exit(69);                                                                       \
        return NULL;                                                                    \
    }                                                                                   \
    WARN_UNUSED_RESULT static inline Name* Name##Push(Name* vec, type data) {           \
        if (vec->len >= vec->cap || vec->cap == 0) {                                    \
            size_t new_cap = vec->cap ? vec->cap * 2 : 1;                               \
            Name* new_vec = MVECREALLOC(vec,                                            \
                sizeof(Name) + (new_cap * sizeof(type)));                               \
            if (!new_vec) {                                                             \
                fprintf(stderr, "Out of memory\n");                                     \
                exit(1);                                                                \
            }                                                                           \
            new_vec->cap = new_cap;                                                     \
            vec = new_vec;                                                              \
        }                                                                               \
        vec->raw[vec->len++] = (data);                                                  \
        return vec;                                                                     \
    }                                                                                   \
    static inline type Name##Set(Name* vec, size_t index, type data) {                  \
        if (index >= vec->len) Name##PANIC(index, vec->len);                            \
        vec->raw[index] = data;                                                         \
        return vec->raw[index];                                                         \
    }                                                                                   \
    WARN_UNUSED_RESULT static inline type Name##Get(Name* vec, size_t index) {                             \
        return (index >= vec->len                                                       \
                ? *Name##PANIC(index, vec->len)                                         \
                : vec->raw[index]);                                                     \
    }                                                                                   \
    WARN_UNUSED_RESULT static inline type* Name##Ref(Name* vec, size_t index) {                            \
        return (index >= vec->len                                                       \
                ? Name##PANIC(index, vec->len)                                          \
                : &vec->raw[index]);                                                    \
    }                                                                                   \
    static inline type Name##Pop(Name* vec) {                                           \
        if (vec->len == 0) {                                                            \
            errno = ERANGE;                                                             \
            fprintf(stderr, "Error: cannot pop from empty "#Name"\n");                  \
        } else vec->len--;                                                              \
        return vec->raw[vec->len];                                                      \
    }                                                                                   \
    static inline void Name##Free(Name* vec) {                                          \
        MVECFREE(vec);                                                                  \
    }																					\
    WARN_UNUSED_RESULT static inline Name* Name##Reserve(Name* vec, size_t min_capacity) {					\
        if (min_capacity > vec->cap) {													\
            size_t new_cap = vec->cap ? vec->cap * 2 : 1;								\
            while (new_cap < min_capacity)												\
                new_cap *= 2;															\
            Name* new_vec = MVECREALLOC(vec, sizeof(Name) + (new_cap * sizeof(type)));	\
            if (!new_vec) {																\
                fprintf(stderr, "Out of memory\n");										\
                exit(1);																\
            }																			\
            new_vec->cap = new_cap;														\
            return new_vec;																\
        }																				\
        return vec;																		\
    }																					\
    WARN_UNUSED_RESULT static inline Name* Name##Grow(Name* vec, size_t additional) {                     	\
        size_t needed = vec->len + additional;                                         	\
        if (needed > vec->cap) {                                                       	\
            size_t new_cap = vec->cap ? vec->cap * 2 : 1;                              	\
            while (new_cap < needed) new_cap *= 2;                                     	\
            Name* new_vec = MVECREALLOC(vec,                                           	\
                sizeof(Name) + (new_cap * sizeof(type)));                               \
            if (!new_vec) {                                                             \
                fprintf(stderr, "Out of memory\n");                                     \
                exit(1);                                                                \
            }                                                                           \
            new_vec->cap = new_cap;                                                     \
            vec = new_vec;                                                              \
        }                                                                               \
        vec->len = needed;                                                              \
        return vec;                                                                     \
    }                                                                                   \
    WARN_UNUSED_RESULT static inline void* Name##Alloc(Name** vec, size_t count) {                         \
        size_t start = (*vec)->len;                                                     \
        *vec = Name##Grow(*vec, count);                                                	\
        return &(*vec)->raw[start];                                                     \
    }
    
#define MArrForeach(typeitem, ptr)                                                      \
    if (ptr->len > 0)                                                                   \
        for (int __k = 1, index = 0;                                                    \
             __k && index < (int)ptr->len;                                              \
             __k = !__k, index++)                                                       \
            for (typeitem = ptr->raw[index]; __k; __k = !__k)

MArrDefine(char, MByteArray)
DEFINE_FREE(MByteArrayFree, MByteArray*, MByteArrayFree)
#endif
