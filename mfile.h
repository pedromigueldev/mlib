#ifndef MFILEH
#define MFILEH

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path, mode) mkdir(path, mode)
#endif

#include "mlib.h"
#include "mview.h"
#include "merrval.h"
#include "marr.h"

#define MfileMkdirCstr(x, pool, cstring) MfileMkdir(MstrViewFrom(pool, cstring, strlen(cstring)), x)
MstrView MfileMkdir(MstrView path, __mode_t permission) {
    UNUSED(permission); // on windows needs to be ignored cuz MKDIR expands and mode is not used
	
    if (MstrViewIsEmpty(path)) {
        errno = EINVAL; 
    	return MstrViewEmpty();
    }

	char* p __free(strfree) = quick_strndup(MstrViewRaw(path), path.length);
    if (MKDIR(p, permission) == -1) return MstrViewEmpty();
    
    return path;
}

#define MfileReadCstr(pool, cstring) MfileRead(pool, MstrViewFromBuffer(pool, cstring, strlen(cstring)))
MstrView MfileRead(MByteArray** mbyte, MstrView filename) {
	if (MstrViewIsEmpty(filename)) {
		errno = EINVAL;
		return MstrViewEmpty();
	}
	char* p __free(strfree) = quick_strndup(MstrViewRaw(filename), filename.length);
    FILE* file = fopen(p, "rb");
    if (!file) {
    	return MstrViewEmpty();
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return MstrViewEmpty();
    }

    long buffer_len = ftell(file);
    size_t buffer_len_a0 = (size_t)buffer_len;
    if (buffer_len < 0) {
        fclose(file);
        return MstrViewEmpty();
    }


    rewind(file);

    size_t start = (*mbyte)->len;
    char* buffer = MByteArrayAlloc(mbyte, buffer_len_a0 + 1);
    size_t bytes_read = fread(buffer, 1, buffer_len_a0, file);
    buffer[bytes_read] = '\0';

    return (MstrView){
        .start = start,
        .length = bytes_read,
        .raw = mbyte
    };
}


#define MfileWriteCstr(path, cstring) MfileWrite(MstrViewFrom(cstring, 0, strlen(cstring)), path)
MstrView MfileWrite(MstrView path, MstrView contents) {
    if (MstrViewIsEmpty(path) || MstrViewIsEmpty(contents)) {
        errno = EINVAL;
        return MstrViewEmpty();
    }

    char* p __free(strfree) = quick_strndup(MstrViewRaw(path), path.length);
    FILE* file = fopen(p, "wb");
    if (!file) {
        return MstrViewEmpty();
    }

    size_t written = fwrite(MstrViewRaw(contents), 1, contents.length, file);
    fclose(file);
    
    if (written != contents.length) {
        return MstrViewEmpty();
    }

    return contents;
}

#endif
