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

MRetErrDefine(const char*, char*, MmkdirResult)
mfile_mkdir_path(const char* path, int permission) {
    UNUSED(permission); // on windows needs to be ignored cuz MKDIR expands and mode is not used

    if (!path) return MRetError(MmkdirResult, "path may not be null");

    if (MKDIR(path, permission) == -1)
        return MRetError(MmkdirResult, strerror(errno));

    return MRetValue(MmkdirResult, path);
}

MRetErrDefine(MstrView, char*, MfileResult)
mfile_read(MVecParamDefPtr(*pool, char), const char* filename) {
    FILE* file = fopen(filename, "rb");
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
    return MRetValue(MfileResult, MstrViewFrom(buffer, 0, bytes_read));
}

MRetErrDefine(MstrView, char*, MfileCreateResult)
mfile_create(const char* path, MstrView contents) {
    if (!path || !contents.raw) {
        return MRetError(MfileCreateResult, "Parameters for file creation may be null");
    }
    FILE* file = fopen(path, "wb");
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
