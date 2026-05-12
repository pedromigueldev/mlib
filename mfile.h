#ifndef MFILEH
#define MFILEH
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "./mstr.h"


int create_directory_tryfail(int argc, char** argv, char** path) {
	if (argc < 2) {
		*path = strdup("Not enough arguments\n");
        return 1;
    }
    char* project_name = argv[1];
    char cwd[PATH_MAX];
    char newProjectFile[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        *path = strerror(errno);
        return 1;
    }

    if (snprintf(newProjectFile, sizeof(newProjectFile), "%s/%s", cwd, project_name) >= sizeof(newProjectFile)) {
        *path = strdup("Path too long\n");
        return 1;
    }

    if (mkdir(newProjectFile, 0755) == -1) {
        *path = strerror(errno);
        return 1;
    }

    *path = strdup(newProjectFile);
    return 0;
}


int read_file_tryfail (const char* filename, Mstr* buffer_out) {
	FILE* file = fopen(filename, "rb");
	
	if (!file) {
		char* error = strerror(errno);
		
		*buffer_out = (Mstr) {
			.raw = strdup(error),
			.length = strlen(error)
		};
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long buffer_len = ftell(file);
	rewind(file);
	char* buffer = malloc(buffer_len + 1);
	fread(buffer, 1, buffer_len, file);
	buffer[buffer_len] = '\0';
	fclose(file);

	*buffer_out = (Mstr) {
		.raw = buffer,
		.length = buffer_len
	};
	
	return 0;
}

#endif
