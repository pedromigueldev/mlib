#ifndef MFILEH
#define MFILEH

#include "mlib.h"
#include "mstr.h"

int mfile_mkdir_path_tryfail(char** error, char* path) {
    if (mkdir(path, 0755) == -1) {
        *error = strerror(errno);
        return 1;
    }
    
    return 0;
}

int mfile_read_tryfail (Mstr** buffer_out, const char* filename) {
	FILE* file = fopen(filename, "rb");
	
	if (!file) {
		char* error = strerror(errno);
		catch(mstr_from, buffer_out, error) {};
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long buffer_len = ftell(file);
	rewind(file);
	char* buffer = malloc(buffer_len + 1);
	fread(buffer, 1, buffer_len, file);
	buffer[buffer_len] = '\0';
	fclose(file);

	catch(mstr_from, buffer_out, buffer) {
		fprintf(stderr, "It was not possible to create file buffer: %s\n", unwrap_fail(*buffer_out));
		return 1;
	};
	
	return 0;
}

#endif
