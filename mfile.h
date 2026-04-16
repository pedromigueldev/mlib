#ifndef MFILEH
#define MFILEH
#include <stdio.h>

bool read_file_fail (const char* filename, Mstr* buffer_out) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		printf("It was not possible to read file");
		return true;
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
	
	return false;
}

#endif
