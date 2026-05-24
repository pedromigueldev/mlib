#ifndef MSTRH
#define MSTRH
#include "marr.h"
	   	
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
	size_t length, start;
	MByteArray** raw;
} MstrView;

#define EMPTYVIEW ((MstrView) { .length = 0, .raw = NULL })
#define IsEmptyView(x) ((x).raw == NULL)
#define MViewRaw(x) ((*(x).raw)->raw + (x).start)

MstrView MstrViewFromCstr(MByteArray** byteArr, char* string, size_t length) {
    size_t from = (*byteArr)->len;

    for (size_t i = 0; i < length; i++) {
        *byteArr = MByteArrayPush(*byteArr, string[i]);
    }

    return (MstrView){
        .start = from,
        .length = length,
        .raw = byteArr
    };
}

MstrView mstr_trim_left(MstrView view) {
    while (view.length > 0 && (*view.raw)->raw[view.start] == ' ') {
        view.start++;
        view.length--;
    }
    return view;
}

MstrView mstr_trim_right(MstrView view) {
    while (view.length > 0 && (*view.raw)->raw[view.start + view.length - 1] == ' ') {
        view.length--;
    }
    return view;
}

MstrView MstrTrim(MstrView view) {
    MstrView new = mstr_trim_left(view);
    new = mstr_trim_right(new);
    return new;
}
        
MstrView MstrSplitView(MstrView string, char delim, MstrView* outView) {
    if (IsEmptyView(string)) {
        *outView = EMPTYVIEW;
        return EMPTYVIEW;
    }

    size_t count = 0;

    while (count < string.length &&
           (*string.raw)->raw[string.start + count] != delim) {
        count++;
    }

    if (count == string.length) {
        *outView = EMPTYVIEW;
        return string;
    }

    *outView = (MstrView){
        .raw = string.raw,
        .start = string.start + count + 1,
        .length = string.length - count - 1
    };

    return (MstrView){
        .raw = string.raw,
        .start = string.start,
        .length = count
    };
}

#endif
