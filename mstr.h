#ifndef MSTRH
#define MSTRH
#include "mvector.h"

typedef struct {
	size_t length;
	char raw[];
} Mstr;

static Mstr MstrEmpty = ((Mstr) {
    .length = 0,
    .raw = "",
});

static Mstr* EMPTY = &MstrEmpty;

typedef struct {
	size_t length;
	char* raw;
} MstrView;

MstrView mstr_view_mstr(Mstr* string, size_t from) {
    return ((MstrView) {
        .length = string->length - from,
        .raw = &string->raw[from]
    });
}

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
            Mstr*: mstr_view_mstr, \
            MstrView: mstr_view_mstrview, \
            char*: mstr_view_string_len\
            ) (string, __VA_ARGS__)
            
MstrView mstr_trim_left(MstrView view) {
    int count = 0;
    while(view.raw[count] == ' ') count++;
    return MstrViewFrom(view, count);
}

MstrView mstr_trim_right(MstrView view) {
    int count = view.length;
    while(view.raw[count - 1] == ' ') 
        count--;
        
    return ((MstrView) {
        .length = count > (int)view.length ? 0 : count,
        .raw = view.raw
    });
}

MstrView mstr_trim_view(MstrView view) {
    if (view.length <= 0) return MstrViewFrom(EMPTY, 0);
    MstrView new = mstr_trim_left(view);
    new = mstr_trim_right(new);
    return MstrViewFrom(new, 0);
}

MstrView mstr_trim_mstr(Mstr* view) {
    if (view->length <= 0) return MstrViewFrom(EMPTY, 0);
    MstrView new = mstr_trim_left(MstrViewFrom(view, 0));
    new = mstr_trim_right(new);
    return MstrViewFrom(new, 0);
}

#define MstrTrim(string) \
    _Generic((string), \
        Mstr*: mstr_trim_mstr, \
        MstrView: mstr_trim_view \
    )(string)
    
MstrView mstr_split_when_view(MstrView string, const char delim, MstrView* outView) {
    int count = 0;
    while(string.raw[count] != delim && count <= (int)string.length)
        count++;
    
    if (count == (int)string.length) {
        *outView = MstrViewFrom(string, 0);
        return MstrViewFrom(EMPTY, 0);
    };
    
    *outView = MstrViewFrom(string, count + 1);
    return ((MstrView) {
        .length = count,
        .raw = string.raw
    });
}

#define MstrSplitView(string, ...) \
    mstr_split_when_view(_Generic((string), Mstr*: MstrViewFrom(string, 0), MstrView: string), __VA_ARGS__)
    
Mstr* mstr_string_from_pool(MVecParamDefPtr(*pool, char), const char* string) {
    if (string == NULL) return EMPTY;
    
    size_t str_len = strlen(string);
    if (str_len == 0) return EMPTY;
    
    Mstr* ret = MVecPoolAlloc(MVecParamRefPtr(pool), sizeof(Mstr) + sizeof(char[str_len]));
    if (ret == NULL) {
        fprintf(stderr,"Not enough memory");
        exit(1);
        return NULL;
    };
    
    *ret = ((Mstr){
        .length = str_len
    });
    memcpy(ret->raw, string, str_len);
    return ret;
}
#define MstrStringFromPool(pool, string) mstr_string_from_pool(MVecParamRefPtr(pool), string)

#endif
