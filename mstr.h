#ifndef MSTRH
#define MSTRH

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char* raw;
	size_t length;
} Mstr_view;

typedef struct {
	char* raw;
	size_t length;
} Mstr;

Mstr mstr_from (char* string) {
	char* buf = strdup(string);
	if (!buf) {
		printf("ERROR: Could not malloc memory for string\n");
		exit(1);
	}

	return (Mstr) {
		.raw = buf,
		.length = strlen(buf),
	};
}

void mstr_chop_r (Mstr* string, size_t n) {
	if (n > string->length) n = string->length;
	string->length -= n;
}

void mstr_chop_l (Mstr* string, size_t n) {
	if (n > string-> length) n = string -> length;
	string->raw += n;
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

Mstr mstr_chop_by (Mstr* string, char delim) {
	size_t i = 0;
	while (i < string->length && string->raw[i] != delim) i++;

	if (i < string-> length) {
		Mstr res = {
			.raw = string->raw,
			.length = i,
		};

		mstr_chop_l(string, i + 1);
		return res;
	}

	Mstr res = *string;
	mstr_chop_r(string, string->length);

	return res;
}

Mstr mstr_chop_by_fun (Mstr* string, int (*fun)(int)) {
	size_t i = 0;
	while (i < string->length && !fun(string->raw[i])) i++;

	if (i < string->length) {
		Mstr res = {
			.raw = string->raw,
			.length = i,
		};

		mstr_chop_l(string, i + 1);
		return res;
	}

	Mstr res = *string;
	mstr_chop_r(string, string->length);
	return res;
}

#endif
