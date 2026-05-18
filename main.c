
#include <stdint.h>
#include "mfile.h"
#include "mstr.h"
#include "mvector.h"
   
int main(void) {
	MVecPreAlloc(lines, char*);
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

		MVecPush(lines, MPRINT_FMT_OUT("LINE: "MFMT(token)));
	    free(token);
	}

	MVecForeach(item, lines) {
		printf("%s\n", item);
	};
	
	MVecForeach(line, lines) free(line);
	free(MVec(lines));
	free(file);
	return 0;
}
