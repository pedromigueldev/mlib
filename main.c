
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "mlib.h"
#include "mstr.h"
#include "merrval.h"
#include "mfile.h"
#include "mprint.h"
#include "marr.h"

int main(void)
{
	MByteArray* MByteArena = MByteArrayMalloc(1);

    MstrView file = MfileReadCstr(&MByteArena, "./main.c");
    if (IsEmptyView(file)) return MPrintFmt("File read failed: "$(strerror(errno)));

	MstrView left = {0}, right = file;
	while(!IsEmptyView(left = MstrSplitView(right, '\n', &right))) {
		MPrintFmt("LINE:"MstrViewFmt(left));
	};
	
	MPrintFmt("FILE: "MstrViewFmt(file));

	free(MByteArena);
	return 0;
}
