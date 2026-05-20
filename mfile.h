#ifndef MFILEH
#define MFILEH
#include "mlib.h"
#include "mstr.h"
#include "merrval.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path, mode) mkdir(path, mode)
#endif

MRetErrDefine(MstrView, char*, MmkdirResult)
mfile_mkdir_path(MstrView path, int permission) {
    UNUSED(permission); // on windows needs to be ignored cuz MKDIR expands and mode is not used
	
    if (MEOF(path)) return MRetError(MmkdirResult, "path may not be null");

	char* p = quick_strndup(path.raw, path.length);
    if (MKDIR(p, permission) == -1) {
		free(p);
        return MRetError(MmkdirResult, strerror(errno));
    }
    
    free(p);
    return MRetValue(MmkdirResult, path);
}

MRetErrDefine(MstrView, char*, MfileResult)
mfile_read(MVecParamDefPtr(*pool, char), MstrView filename) {
	if (MEOF(filename)) return MRetError(MfileResult, "filename may not be null");
	char* p = quick_strndup(filename.raw, filename.length);
    FILE* file = fopen(p, "rb");
    if (!file) return MRetError(MfileResult, strerror(errno));

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return MRetError(MfileResult, strerror(errno));
    }

    long buffer_len = ftell(file);
    if (buffer_len < 0) {
        fclose(file);
        return MRetError(MfileResult, strerror(errno));
    }

    rewind(file);

    char* buffer = MVecPoolAlloc(MVecParamRefPtr(pool), sizeof(char) * (buffer_len + 1));

    size_t bytes_read = fread(buffer, 1, buffer_len, file);

    if (bytes_read != (size_t)buffer_len && ferror(file)) {
	    fclose(file);
	    return MRetError(MfileResult, strerror(errno));
    }

    buffer[bytes_read] = '\0';
    fclose(file);
    free(p);
    return MRetValue(MfileResult, MstrViewFrom(buffer, 0, bytes_read));
}

MRetErrDefine(MstrView, char*, MfileCreateResult)
mfile_create(MstrView path, MstrView contents) {
    if (MEOF(path) || !contents.raw) {
        return MRetError(MfileCreateResult, "Parameters for file creation may be null");
    }

    char* p = quick_strndup(path.raw, path.length);
    FILE* file = fopen(p, "wb");
    if (!file) return MRetError(MfileCreateResult, strerror(errno));

    size_t written = fwrite(contents.raw, 1, contents.length, file);
    if (written != contents.length) {
        fclose(file);
        return MRetError(MfileCreateResult, strerror(errno));
    }

    fclose(file);
    return MRetValue(MfileCreateResult, contents);
}

#endif
