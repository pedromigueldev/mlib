
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "mlib.h"
#include "mview.h"
#include "merrval.h"
#include "mfile.h"
#include "mprint.h"
#include "marr.h"

int char_eq(char a, char b) { return a == b; }
int main(void)
{
	MByteArray* MByteArena = MByteArrayMalloc(1);

    MstrView file = MfileReadCstr(&MByteArena, "./main.c");
    if (MstrViewIsEmpty(file)) return MPrintFmt("File read failed: "$(strerror(errno)));

	MstrView left = {0}, right = file;
	while(!MstrViewIsEmpty(left = MstrViewSplit(right, '\n', char_eq, &right))) {
		MPrintFmt("LINE:"MstrViewFmt(left));
	};
	
	MPrintFmt("FILE: "MstrViewFmt(file));

	free(MByteArena);
	return 0;
}
