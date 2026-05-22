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
	char raw[];
} Mstr;

typedef struct {
	size_t length;
	char* raw;
} MstrView;

bool MEOF(MstrView view) {
	return view.raw == NULL;
}

#define EMPTYVIEW(type) \
		_Generic(type, \
			Mstr*: NULL,\
			MstrView: ((MstrView) {\
	            .length = 0,\
	            .raw = NULL\
	        })\
	   	)

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

MstrView mstr_trim_view(MstrView view) {
    MstrView new = mstr_trim_left(view);
    new = mstr_trim_right(new);
    return MstrViewFrom(new, 0);
}

MstrView mstr_trim_mstr(Mstr* view) {
    MstrView new = mstr_trim_left(MstrViewFrom(view, 0));
    new = mstr_trim_right(new);
    return MstrViewFrom(new, 0);
}

#define MstrTrim(string) \
    _Generic((string), \
        Mstr*: mstr_trim_mstr, \
        MstrView: mstr_trim_view \
    )(string)
        
MstrView mstr_split_when_view(MstrView string, char delim, MstrView* outView) {
    if (string.raw == NULL || string.length == 0) {
        *outView = EMPTYVIEW(MstrView);
        return EMPTYVIEW(MstrView);
    }

    size_t count = 0;
    while (count < string.length && string.raw[count] != delim) {
        count++;
    }

    if (count == string.length) {
        *outView = EMPTYVIEW(MstrView);
        return string;
    }

    *outView = MstrViewFrom(string, count + 1);
    return (MstrView){
        .raw = string.raw,
        .length = count
    };
}

#define MstrSplitView(string, ...) \
    mstr_split_when_view(_Generic((string), Mstr*: MstrViewFrom(string, 0), MstrView: string), __VA_ARGS__)
    
Mstr* mstr_string_from_pool(char** MVecDef(pool), const char* string) {
    if (string == NULL) return EMPTYVIEW(Mstr*);
    size_t str_len = strlen(string);
    if (str_len == 0) return EMPTYVIEW(Mstr*);
    
    Mstr* ret = MVecPoolAlloc(&MVecRef(*pool), sizeof(Mstr) + sizeof(char[str_len]));
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
