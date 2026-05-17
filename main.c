
#include <stdint.h>
#include "mfile.h"
#include "mstr.h"
#include "marr.h"

void many(Marr_param_ref(ptr, char)) {
	for(int i = 0; i < 10; i++) {
		Marr_put(ptr, 'a' + i);
	}
}

bool string_append(Marr_param_ref(ptr, char), const char* string) {
	do {
		if (string != NULL)
			Marr_put(ptr, *string);
	} while (*string++);
	return 0;
}

int main(void) {
	Marr_dft_def(lines, char*);
	Marr_new_def(numbers, char, 10);
	many(Marr_spreadref(numbers));
	Mstr* file = NULL;
	
	catch(mfile_read, &file, "main.c") {
		fprintf(stderr, "%s\n", unwrap_fail(file));
		return 1;
	};

	Mstr* token = NULL;
	while ((token = mstr_chop_by(file, '\n')) != NULL){
		Marr_put(&lines, MPRINT_FMT_OUT("LINE: "MFMT(token)));
	    free(token);
	}

	Marr_foreach(item, &lines) {
		printf("%s\n", item);
		free(item);
	};
	
	free(ARR(lines));
	free(ARR(numbers));
	free(file);
	return 0;
}
