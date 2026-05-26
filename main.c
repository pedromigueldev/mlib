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
    char* class;
    char* id;
	char* href;
 	char* aria;
 	char* lang;
 	char* dir;
 	char* style;
 	char* src;
 	char* onclick;
 	
	bool isVoid, tagInterface;
} HTMLInterface;

#define xvoid .isVoid=true

#define html(text, ...) mx(html, .lang=text, ##__VA_ARGS__)
#define head(...) mx(head, __VA_ARGS__)
#define body(...) mx(body,__VA_ARGS__)
#define div(...) mx(div, __VA_ARGS__)
#define p(text, ...) mx(p, __VA_ARGS__) mx$(text)
#define section(text, ...) mx(section, __VA_ARGS__) mx$(text)
#define mainSection(text, ...) mx(mainSection, __VA_ARGS__) mx$(text)

#define h1(text, ...) mx(h1, .isVoid=true, ##__VA_ARGS__) mx$(text)
#define h2(text, ...) mx(h2, .isVoid=true, ##__VA_ARGS__) mx$(text)
#define h3(text, ...) mx(h3, .isVoid=true, ##__VA_ARGS__) mx$(text)
#define h4(text, ...) mx(h4, .isVoid=true, ##__VA_ARGS__) mx$(text)
#define h5(text, ...) mx(h5, .isVoid=true, ##__VA_ARGS__) mx$(text)
#define button(text, ...) mx(button, .isVoid=true, ##__VA_ARGS__) mx$(text)

int main()
{
    MByteArray* pool = MByteArrayMalloc(0);
    XMLFILE* xmlfile = XMLFILEMalloc(0);

    MXMLBegin(&pool, &xmlfile, HTMLInterface)
    {
        html("pt-BR") {
            head(xvoid);
            body(xvoid){
	            h1("Button Example");
	            button("Click Me", .id="btnSend", .class="primary-btn", .onclick="alert('Button clicked!')");
	            button( "Console Log", .onclick="console.log('Hello from button')");
            }
        }
    }
	
    printf("Hello World \n%.*s\n", (int)xmlfile->len, xmlfile->raw);
	printf("freeing: %zu bytes | used %zu \n", xmlfile->cap, xmlfile->len);
    MByteArrayFree(pool);
	XMLFILEFree(xmlfile);
    return 0;
}
