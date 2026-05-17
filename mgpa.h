#ifndef MGPA_H
#define MGPA_H
#include <stdint.h>
#include "mlib.h"
#include "marr.h"

// this is WIP and not sure if it will wrk
typedef struct {
	uintptr_t memory;
	int line;
	char* file;
} MGPAllocMem;

void* MGPAlloc(size_t size, MGPAllocMem** memories, int line, char* file) {
	void* mem = malloc(size);
	if (!mem) {
		exit(1);
		return NULL;
	}
	
	MarrPut(*memories, ((MGPAllocMem){
			.memory = (uintptr_t)mem,
			.line = line,
			.file = file
		}));
	
	return mem;
}

void MGPAllocVerify(MGPAllocMem* mem) {
	MarrForeach(item, mem) {
		if ((void*)item.memory != NULL) {
			fprintf(stderr, "Memory not freed allocated at: %s:%d\n", item.file, item.line);
			exit(69);
		}
	}
}

DEFINE_FREE(mgpafree, MGPAllocMem*, MGPAllocVerify);
#define MGPA(allocvector, size) MGPAlloc(size, &allocvector, __LINE__, __FILE__)

#endif
