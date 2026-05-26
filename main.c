#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>

#include "marr.h"
#include "mview.h"
#include "helpers.h"
#include "mxml.h"

typedef struct {
    char* tag; 
    char* class; 
	char* name; 
    char* id; 
    char* lib;
    size_t version;
	bool isVoid, tagInterface;
} TagInterface;

int main()
{
	MByteArray* pool = MByteArrayMalloc(0);
    XMLFILE* xmlfile = XMLFILEMalloc(0);
	
    MXMLBegin(&pool, &xmlfile, TagInterface) {
        mx(interface, .isVoid=true) {
			mx(object, .class="GtkApplicationWindow", .id="main_window") {
				mx(property, .name="title") mx$("Counter "$(__LINE__));
				mx(property, .name="default-width") mx$("Counter "$(__LINE__));
				mx(property, .name="default-height") mx$("Counter "$(__LINE__));
			};

			mx(object, .class="GtkApplicationWindow", .id="main_window") {
				mx(property, .name="title") mx$("Counter");
				mx(property, .name="default-width") mx$("Counter");
				mx(property, .name="default-height") mx$("Counter "$(__LINE__));
			};
        }
    }
	
    printf("Hello World \n%.*s\n", (int)xmlfile->len, xmlfile->raw);
	printf("freeing: %zu bytes | used %zu \n", xmlfile->cap, xmlfile->len);
    MByteArrayFree(pool);
	XMLFILEFree(xmlfile);

    return 0;
}