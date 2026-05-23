
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "mlib.h"
#include "mstr.h"
#include "merrval.h"
#include "mfile.h"
#include "mprint.h"

int main(void)
{
	__free(strfree) char* MVecAlloc(pool, 100);
    MRetEither(file, err, MfileReadCstr(&MVecRef(pool), "./main.c"));
    if (err) {
    	MPrintFmt("File read failed: "$(strerror(err)));
    	return 1;
    }
	MstrView left = {0}, right = file;
	while(!IsEmptyView(left = MstrSplitView(right, '\n', &right))) {
		MPrintFmt("LINE:"MstrViewFmt(left));
	};
	MPrintFmt("FILE: "MstrViewFmt(file));
	return 0;
}
