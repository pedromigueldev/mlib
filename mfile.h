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
#include "mstr.h"
#include "merrval.h"

#define MfileMkdirCstr(x, cstring) MfileMkdir(MstrViewFrom(cstring, 0, strlen(cstring)), x)
MstrView MfileMkdir(MstrView path, __mode_t permission) {
    UNUSED(permission); // on windows needs to be ignored cuz MKDIR expands and mode is not used
	
    if (IsEmptyView(path)) {
        errno = EINVAL; 
    	return EMPTYVIEW;
    }

	char* p __free(strfree) = quick_strndup(path.raw, path.length);
    if (MKDIR(p, permission) == -1) return EMPTYVIEW;
    
    return path;
}

#define MfileReadCstr(pool, cstring) MfileRead(pool, MstrViewFrom(cstring, 0, strlen(cstring)))
MstrView MfileRead(char** MVecDef(pool), MstrView filename) {
	if (IsEmptyView(filename)) {
		errno = EINVAL;
		return EMPTYVIEW;
	}
	char* p __free(strfree) = quick_strndup(filename.raw, filename.length);
    FILE* file = fopen(p, "rb");
    if (!file) {
    	return EMPTYVIEW;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return EMPTYVIEW;
    }

    long buffer_len = ftell(file);
    size_t buffer_len_a0 = (size_t)buffer_len;
    if (buffer_len < 0) {
        fclose(file);
        return EMPTYVIEW;
    }


    rewind(file);
    char* buffer = MVecPoolAlloc(&MVecRef(*pool), sizeof(char) * (buffer_len_a0 + 1));
    size_t bytes_read = fread(buffer, 1, buffer_len_a0, file);

    if (bytes_read != buffer_len_a0 && ferror(file)) {
	    fclose(file);
	    return EMPTYVIEW;
    }

    buffer[bytes_read] = '\0';
    fclose(file);
    return MstrViewFrom(buffer, 0, bytes_read);
}


MstrView MFileCreate(MstrView path, MstrView contents) {
    if (IsEmptyView(path) || !contents.raw) {
    	errno = EINVAL;
        return EMPTYVIEW;
    }

    char* p __free(strfree) = quick_strndup(path.raw, path.length);
    FILE* file = fopen(p, "wb");
    if (!file) {
    	return EMPTYVIEW;
    }

    size_t written = fwrite(contents.raw, 1, contents.length, file);
    if (written != contents.length) {
        fclose(file);
        return EMPTYVIEW;
    }

    fclose(file);
    return contents;
}

#endif
