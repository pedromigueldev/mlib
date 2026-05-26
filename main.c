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
    char* id; 
    char* lib;
    size_t version;
	bool isVoid, tagInterface;
} TagInterface;

int main()
{
	MByteArray* pool = MByteArrayMalloc(0);
    XMLFILE* xmlfile = XMLFILEMalloc(0);
        
    XMLBEGIN(&pool, &xmlfile, TagInterface) {
        XML(interface, .isVoid=true) {
			for (size_t i = 0; i < 10; i++)
			{
				for (size_t i = 0; i < 10; i++)
				{
					XML(requires, .lib="gtk", .version=4.0) {
						XML(object, .class="GtkApplicationWindow", .id="main_window");
					}
				}
				
				XML(requires, .lib="gtk", .version=4.0) {
					XML(object, .class="GtkApplicationWindow", .id="main_window");
				}
			}
			
        }
    }
	
    printf("Hello World \n%.*s\n", (int)xmlfile->len, xmlfile->raw);
	printf("Hello World \n%.*s\n", (int)pool->len, pool->raw);
	printf("freeing: %zu bytes | used %zu \n", pool->cap, pool->len);
	printf("freeing: %zu bytes | used %zu \n", xmlfile->cap, xmlfile->len);
    MByteArrayFree(pool);
	XMLFILEFree(xmlfile);

    return 0;
}