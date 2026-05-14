#ifndef MSTRH
#define MSTRH
#include "mfile.h"

char* strndup(const char* s, size_t len) {
    char* copy = malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

typedef struct {
	size_t length;
	char raw[];
} Mstr;

int mstr_from_tryfail (Mstr** out, char* string) {
	size_t len = strlen(string);
	Mstr* mstrstring = malloc(sizeof(Mstr) + len);
	if (!mstrstring) {
		*(void**)(out) = strdup("mstr_from_fail alloc: buy more ram\n");
		return 1;
	}
	*mstrstring = (Mstr) {
		.length = len,
	};
	
    memcpy(mstrstring->raw, string, len);
	*out = mstrstring;
	return 0;
}

void mstr_chop_r(Mstr* string, size_t n) {
    if (n > string->length)
        n = string->length;

    string->length -= n;
}

void mstr_chop_l(Mstr* string, size_t n) {
    if (n > string->length)
        n = string->length;

    memmove(
        string->raw,
        string->raw + n,
        string->length - n
    );

    string->length -= n;
}

void mstr_trim_left (Mstr* string) {
	while(string->length > 0 && isspace(string->raw[0])) {
		mstr_chop_l(string, 1);
	}
}

void mstr_trim_right (Mstr* string) {
	while(string->length > 0 && isspace(string->raw[string->length -1])) {
		mstr_chop_r(string, 1);	
	}
}

void mstr_trim (Mstr* string) {
	mstr_trim_left(string);
	mstr_trim_right(string);
}

Mstr* mstr_chop_by(Mstr* string, char delim) {
	if (string->length <= 0) return NULL;
	size_t i = 0;

	while (i < string->length && string->raw[i] != delim)
		i++;


	Mstr* res = NULL;
	char* copy = strndup(string->raw, string->length);
	catch(mstr_from, &res, copy) {
		return NULL;
	}

	if (i < string->length)
		mstr_chop_r(res, res->length - i);
	
	if (i < string->length)
		mstr_chop_l(string, i + 1);
	else
		mstr_chop_r(string, string->length);

	free(copy);
	return res;
}

Mstr* mstr_chop_by_fun(Mstr* string, int (*fun)(int)) {
	if (string->length <= 0) return NULL;
	size_t i = 0;

	while (i < string->length && !fun((unsigned char)string->raw[i]))
		i++;

	Mstr* res = NULL;
	char* copy = strndup(string->raw, string->length);
	catch(mstr_from, &res, copy) {
		return NULL;
	}

	if (i < string->length)
		mstr_chop_r(res, res->length - i);
	
	if (i < string->length)
		mstr_chop_l(string, i + 1);
	else
		mstr_chop_r(string, string->length);

	free(copy);
	return res;
}

#endif
