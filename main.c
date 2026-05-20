
#include <stdint.h>
#include "mstr.h"
#include "merrval.h"

int mfile_mkdir_path(char** path, int permission) {
    if (mkdir(*path, permission) == -1) {
        wrap_fail(path) = strdup(strerror(errno));
        return 1;
    }

    return 0;
}

MstrView mfile_read (MVecParamDefPtr(*pool, char), const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		return EMPTYVIEW(MstrView);
	}

	fseek(file, 0, SEEK_END);
	long buffer_len = ftell(file);
	rewind(file);

	char* buffer = MVecPoolAlloc(MVecParamRefPtr(pool), sizeof(char[buffer_len]));
	fread(buffer, 1, buffer_len, file);
	fclose(file);
	return MstrViewFrom(buffer, 0, buffer_len);
}

int mfile_create(char** out, char* path, char* contents) {
	if (!out || !path || !contents) {
		wrap_fail(out) = strdup("Parameters for file creation may be null");
        return 1;
    }

	FILE* file = fopen(path, "w");
	
    if (file == NULL) {
        wrap_fail(out) = strdup(strerror(errno));
        return 1;
    }
    
    if (fputs(contents, file) == EOF) {
        wrap_fail(out) = strdup(strerror(errno));
        fclose(file);
        return 1;
    }

    if (fclose(file) == EOF) {
        wrap_fail(out) = strdup(strerror(errno));
        return 1;
    }
    
    *out = contents;
	return 0;
}


int main()
{
	MVecAlloc(pool, char, 100);
	MstrView file = mfile_read(MVecParamRefPtr(&pool), "main.c");

	MstrView right = {0}, left = MstrSplitView(file, '\n', &right);
	while(!MEOF(left = MstrSplitView(right, '\n', &right))){
		MPRINT_FMT("LINE: "MstrViewFmt(left)"|");	
	}

	free(MVec(pool));
	return 0;
}
