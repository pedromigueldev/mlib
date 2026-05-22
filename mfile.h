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

#define MfileMkdirCstr(x, cstring) mfile_mkdir_path(MstrViewFrom(cstring, 0, strlen(cstring)), x)
#define MfileMkdirView(x, view) mfile_mkdir_path(view, x)
MRetDefine(MstrView, MmkdirResult) mfile_mkdir_path(MstrView path, __mode_t permission) {
    UNUSED(permission); // on windows needs to be ignored cuz MKDIR expands and mode is not used
	
    if (MEOF(path)) {
        errno = EINVAL; 
    	return MRetError(MmkdirResult);
    }

	char* p __free(strfree) = quick_strndup(path.raw, path.length);
    if (MKDIR(p, permission) == -1) return MRetError(MmkdirResult);
    
    return MRetValue(MmkdirResult, path);
}

#define MfileReadCstr(pool, cstring) mfile_read(pool, MstrViewFrom(cstring, 0, strlen(cstring)))
#define MfileReadView(pool, view) mfile_read(pool, view)
MRetDefine(MstrView, MfileResult) mfile_read(char** MVecDef(pool), MstrView filename) {
	if (MEOF(filename)) {
		errno = EINVAL;
		return MRetError(MfileResult);
	}
	char* p __free(strfree) = quick_strndup(filename.raw, filename.length);
    FILE* file = fopen(p, "rb");
    if (!file) {
    	return MRetError(MfileResult);
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return MRetError(MfileResult);
    }

    long buffer_len = ftell(file);
    size_t buffer_len_a0 = (size_t)buffer_len;
    if (buffer_len < 0) {
        fclose(file);
        return MRetError(MfileResult);
    }


    rewind(file);
    char* buffer = MVecPoolAlloc(&MVecRef(*pool), sizeof(char) * (buffer_len_a0 + 1));
    size_t bytes_read = fread(buffer, 1, buffer_len_a0, file);

    if (bytes_read != buffer_len_a0 && ferror(file)) {
	    fclose(file);
	    return MRetError(MfileResult);
    }

    buffer[bytes_read] = '\0';
    fclose(file);
    return MRetValue(MfileResult, MstrViewFrom(buffer, 0, bytes_read));
}

MRetDefine(MstrView, MfileCreateResult)
mfile_create(MstrView path, MstrView contents) {
    if (MEOF(path) || !contents.raw) {
    	errno = EINVAL;
        return MRetError(MfileCreateResult);
    }

    char* p __free(strfree) = quick_strndup(path.raw, path.length);
    FILE* file = fopen(p, "wb");
    if (!file) {
    	return MRetError(MfileCreateResult);
    }

    size_t written = fwrite(contents.raw, 1, contents.length, file);
    if (written != contents.length) {
        fclose(file);
        return MRetError(MfileCreateResult);
    }

    fclose(file);
    return MRetValue(MfileCreateResult, contents);
}

#endif
