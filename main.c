
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

    MstrView file = MfileReadCstr(&MVecRef(pool), "./main.c");
    if (IsEmptyView(file)) return MPrintFmt("File read failed: "$(strerror(errno)));

	MstrView left = {0}, right = file;
	while(!IsEmptyView(left = MstrSplitView(right, '\n', &right))) {
		MPrintFmt("LINE:"MstrViewFmt(left));
	};
	
	MPrintFmt("FILE: "MstrViewFmt(file));
	return 0;
}
