
#include <stdint.h>
#include <assert.h>
#include "mstr.h"
#include "merrval.h"
#include "mfile.h"
#include "mprint.h"

int main()
{
	MVecAlloc(pool, char, 100);

    MRetEither(dir, err1, mfile_mkdir_path("./test_dir", 0755));
    assert(err1 == NULL);
    assert(dir != NULL);

    MRetEither(dir2, err2, mfile_mkdir_path(NULL, 0755));
    assert(err2 != NULL);
    MPrintFmt("expected file create failure -------- *: "$(err2));
    UNUSED(dir2);

    MstrView content = MstrViewFrom("hello\nworld", 0, 11);

    MRetEither(file, err3, mfile_create("./test_dir/test.txt", content));
    assert(err3 == NULL);
    assert(file.raw != NULL);
    assert(file.length == 11);

    MRetEither(file2, err4, mfile_create(NULL, content));
    assert(err4 != NULL);
    MPrintFmt("expected file create failure -------- *: "$(err4));
    UNUSED(file2);

    MRetEither(file3, err5, mfile_read(MVecParamRefPtr(&pool), "./test_dir/test.txt"));
    assert(err5 == NULL);
    
	MstrView left = {0}, right = file3;
	while(!MEOF(left = MstrSplitView(right, '\n', &right))) {
		MPrintFmt("LINE:"MstrViewFmt(left));
	};
	MPrintFmt("FILE: "MstrViewFmt(file3));

	free(MVec(pool));
	return 0;
}
