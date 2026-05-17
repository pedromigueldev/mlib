
#include <stdint.h>
#include "mfile.h"
#include "mstr.h"
#include "marr.h"

void many(Marr_param_ref(ptr, char)) {
	static bool second_layer = false;

	for(int i = 0; i < 10; i++) {
		Marr_put(ptr, 'a' + i);
	}

	if (second_layer) return;
	second_layer = true;
	
	many(Marr_spreadref(ptr));
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
	Mstr* file = NULL;
	
	catch(mfile_read, &file, "main.c") {
		fprintf(stderr, "%s\n", unwrap_fail(file));
		return 1;
	};

	for(;;) {
		Mstr* token = NULL;
		catch(mstr_chop_by, &token, file, '\n') {
			fprintf(stderr, "%s\n", unwrap_fail(token));
			return 1;
		}

		if (token == NULL) break;

		Marr_put(&lines, MPRINT_FMT_OUT("LINE: "MFMT(token)));

	    free(token);
	}

	Marr_foreach(item, &lines) {
		printf("%s\n", item);
		free(item);
	};
	
	free(ARR(lines));
	free(file);
	return 0;
}
