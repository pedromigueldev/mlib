#ifndef MVIEW_H
#define MVIEW_H
#include "marr.h"

#define MViewDefine(Type, ArrayName, ViewName)                                       \
typedef struct {                                                                     \
    size_t length, start;                                                            \
    ArrayName** raw;                                                                 \
} ViewName;                                                                          \
static inline ViewName ViewName##Empty(void) {                                       \
    return (ViewName){ .length = 0, .start = 0, .raw = NULL };                       \
}                                                                                    \
static inline int ViewName##IsEmpty(ViewName view) {                                 \
    return view.raw == NULL;                                                         \
}                                                                                    \
static inline Type* ViewName##Raw(ViewName view) {                                   \
    return (*(view.raw))->raw + view.start;                                          \
}                                                                                    \
WARN_UNUSED_RESULT static inline ViewName                                            \
ViewName##FromBuffer(ArrayName** arr, const Type* src, size_t count) {               \
    size_t start = (*arr)->len;                                                      \
    for (size_t i = 0; i < count; ++i) {                                             \
        *arr = ArrayName##Push(*arr, src[i]);                                        \
    }                                                                                \
    return (ViewName){ .start = start, .length = count, .raw = arr };                \
}                                                                                    \
WARN_UNUSED_RESULT static inline ViewName                                            \
ViewName##TrimLeft(ViewName view, Type value, int (*eq)(Type, Type)) {               \
    while (view.length > 0 && eq((*view.raw)->raw[view.start], value)) {             \
        view.start++;                                                                \
        view.length--;                                                               \
    }                                                                                \
    return view;                                                                     \
}                                                                                    \
WARN_UNUSED_RESULT static inline ViewName                                            \
ViewName##TrimRight(ViewName view, Type value, int (*eq)(Type, Type)) {              \
    while (view.length > 0 &&                                                        \
           eq((*view.raw)->raw[view.start + view.length - 1], value)) {              \
        view.length--;                                                               \
    }                                                                                \
    return view;                                                                     \
}                                                                                    \
WARN_UNUSED_RESULT static inline ViewName                                            \
ViewName##Trim(ViewName view, Type value, int (*eq)(Type, Type)) {                   \
    view = ViewName##TrimLeft(view, value, eq);                                      \
    return ViewName##TrimRight(view, value, eq);                                     \
}                                                                                    \
WARN_UNUSED_RESULT static inline ViewName                                            \
ViewName##Split(ViewName view, Type delim, int (*eq)(Type, Type), ViewName* outRest) { \
    if (view.length == 0 || view.raw == NULL) {                                      \
        if (outRest) *outRest = ViewName##Empty();                                   \
        return ViewName##Empty();                                                    \
    }                                                                                \
    size_t count = 0;                                                                \
    while (count < view.length && !eq((*view.raw)->raw[view.start + count], delim)) { \
        count++;                                                                     \
    }                                                                                \
    if (count == view.length) {                                                      \
        if (outRest) *outRest = ViewName##Empty();                                   \
        return view;                                                                 \
    }                                                                                \
    if (outRest) {                                                                   \
        *outRest = (ViewName){                                                       \
            .raw = view.raw,                                                         \
            .start = view.start + count + 1,                                         \
            .length = view.length - count - 1                                        \
        };                                                                           \
    }                                                                                \
    return (ViewName){                                                               \
        .raw = view.raw,                                                             \
        .start = view.start,                                                         \
        .length = count                                                              \
    };                                                                               \
}                                                                                    \
WARN_UNUSED_RESULT static inline ViewName                                            \
ViewName##FindSpan(ViewName view, const Type* needle, size_t needle_len,             \
                   int (*eq)(Type, Type)) {                                          \
    if (view.length == 0 || view.raw == NULL || needle_len == 0 ||                   \
        needle_len > view.length) {                                                  \
        return ViewName##Empty();                                                    \
    }                                                                                \
    for (size_t i = 0; i <= view.length - needle_len; i++) {                        \
        int found = 1;                                                               \
        for (size_t j = 0; j < needle_len; j++) {                                   \
            if (!eq((*view.raw)->raw[view.start + i + j], needle[j])) {              \
                found = 0;                                                           \
                break;                                                               \
            }                                                                        \
        }                                                                            \
        if (found) {                                                                 \
            return (ViewName){                                                       \
                .raw = view.raw,                                                     \
                .start = view.start + i,                                             \
                .length = needle_len                                                 \
            };                                                                       \
        }                                                                            \
    }                                                                                \
    return ViewName##Empty();                                                        \
}                                                                                    \
WARN_UNUSED_RESULT static inline size_t                                              \
ViewName##FindSpanIndex(ViewName view, const Type* needle, size_t needle_len,        \
                        int (*eq)(Type, Type)) {                                     \
    if (view.length == 0 || view.raw == NULL || needle_len == 0 ||                   \
        needle_len > view.length) {                                                  \
        return (size_t)-1;                                                           \
    }                                                                                \
    for (size_t i = 0; i <= view.length - needle_len; i++) {                        \
        int found = 1;                                                               \
        for (size_t j = 0; j < needle_len; j++) {                                   \
            if (!eq((*view.raw)->raw[view.start + i + j], needle[j])) {              \
                found = 0;                                                           \
                break;                                                               \
            }                                                                        \
        }                                                                            \
        if (found) {                                                                 \
            return view.start + i;                                                   \
        }                                                                            \
    }                                                                                \
    return (size_t)-1;                                                               \
}

MViewDefine(char, MByteArray, MstrView)
#define MstrViewFmt(x) ((x).raw == NULL || (x).length == 0 ? "" : _3_$("%.*s", (int)(x).length, MstrViewRaw(x)))
#endif