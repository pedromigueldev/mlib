
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "mstr.h"
#include "merrval.h"
#include "mfile.h"
#include "mprint.h"

int main(void)
{
	MVecAlloc(pool, char, 100);
	
    MRetEither(file, err, mfile_read(MVecParamRefPtr(&pool), MStrFmt(&pool, "./main.c")));
    if (err) {
    	MPrintFmt("FILE: "$(err));
    	return 1;
    }
	MstrView left = {0}, right = file;
	while(!MEOF(left = MstrSplitView(right, '\n', &right))) {
		MPrintFmt("LINE:"MstrViewFmt(left));
	};
	MPrintFmt("FILE: "MstrViewFmt(file));

	free(MVec(pool));
	return 0;
}
