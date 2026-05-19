
#include <stdint.h>
#include "mfile.h"
#include "mstr.h"

int main()
{
	MVecAlloc(pool, char, 100);
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

		MStringPoolPush(&string_pool, MPRINT_FMT_OUT("LINE: "MFMT(token)));
	    free(token);
	}

	MVecForeach(item, string_pool) {
		printf("%s\n", item);
		free(item);
	};
	
	free(MVec(string_pool));
	free(file);
	return 0;
}
