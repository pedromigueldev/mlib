
#include <stdint.h>
#include "mfile.h"
#include "mstr.h"
#include "marr.h"

void many(char** ptr) {
	for(int i = 0; i < 10; i++) {
		MarrPut(*ptr, 'a' + i);
	}
}

bool string_append(char** ptr, const char* string) {
	do {
		if (string != NULL)
			MarrPut(*ptr, *string);
	} while (*string++);
	return 0;
}
   


int main(void) {
// 	MGPAllocMem* allocvector __free(mgpafree) = NULL;
// 	char* string = MGPA(allocvector, sizeof(char) * 10);
// 
// 	for(int i = 0; i < 10; i++) {
// 		string[i] = 'a' + i;
// 		printf("%c", string[i]);
// 	}
// 
// 	puts("\n");
// 	return 0;
	
	char** lines = NULL;
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

		MarrPut(lines, MPRINT_FMT_OUT("LINE: "MFMT(token)));
	    free(token);
	}

	MarrForeach(item, lines) {
		printf("%s\n", item);
		free(item);
	};
	
	MarrForeach(line, lines) free(line);
	free(file);
	return 0;
}
