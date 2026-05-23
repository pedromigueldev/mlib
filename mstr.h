#ifndef MSTRH
#define MSTRH
#include "mvec.h"
	   	
char *quick_strndup(const char *s, size_t n)
{
    size_t len = 0;
    while (len < n && s[len] != '\0')
        len++;

    char *copy = (char *)malloc(len + 1);
    if (!copy)
        return NULL;

    memcpy(copy, s, len);
    copy[len] = '\0';

    return copy;
}

typedef struct {
	size_t length;
	char* raw;
} MstrView;

#define EMPTYVIEW ((MstrView) { .length = 0, .raw = NULL })
#define IsEmptyView(x) ((x).length == 0 && (x).raw == NULL)

MstrView mstr_view_mstrview(MstrView string, size_t from) {
    return ((MstrView) {
        .length = string.length - from,
        .raw = &string.raw[from]
    });
}

MstrView mstr_view_string_len(char* string, size_t from, size_t length) {
    return ((MstrView) {
        .length = length - from,
        .raw = &string[from]
    });
}

#define MstrViewFrom(string, ...) _Generic((string), \
            MstrView: mstr_view_mstrview, \
            char*: mstr_view_string_len\
            ) (string, __VA_ARGS__)
            
MstrView mstr_trim_left(MstrView view) {
    size_t count = 0;
    while(view.raw != NULL && view.raw[count] == ' ') count++;
    return MstrViewFrom(view, count);
}

MstrView mstr_trim_right(MstrView view) {
    size_t count = view.length;
    while(view.raw != NULL && view.raw[count - 1] == ' ') 
        count--;
        
    return ((MstrView) {
        .length = count > view.length ? 0 : count,
        .raw = view.raw
    });
}

MstrView MstrTrim(MstrView view) {
    MstrView new = mstr_trim_left(view);
    new = mstr_trim_right(new);
    return MstrViewFrom(new, 0);
}
        
MstrView MstrSplitView(MstrView string, char delim, MstrView* outView) {
    if (IsEmptyView(string)) {
        *outView = EMPTYVIEW;
        return EMPTYVIEW;
    }

    size_t count = 0;
    while (count < string.length && string.raw[count] != delim) {
        count++;
    }

    if (count == string.length) {
        *outView = EMPTYVIEW;
        return string;
    }

    *outView = MstrViewFrom(string, count + 1);
    return (MstrView){
        .raw = string.raw,
        .length = count
    };
}

#endif
