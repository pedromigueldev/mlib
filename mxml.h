#ifndef MXML_H
#define MXML_H

#include "marr.h"
#include "mprint.h"

MArrDefine(MstrView, XMLTAGS)
MArrDefine(char, XMLFILE)

typedef struct {
    XMLTAGS* openedTags;
    MByteArray** pool;
	XMLFILE** xmlfile;
    bool sameLine;
} MXMLContext;

MXMLContext MXMLContextInit(MByteArray** b, XMLFILE** c) {
    XMLTAGS* a = XMLTAGSMalloc(0);
    return (MXMLContext) {
        .openedTags=a,
        .pool=b,
		.xmlfile=c
    };
}

int chareq (char a, char b) { return a == b; }

int MwriteXMLOpenTag(MXMLContext* xmlcontext, const char* objectName, size_t name_len, const char* param, size_t param_len, int* level, ...) {
    // indent and open tag <
	for (int i = 0; i < *level; i++)
        *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '\t');
    *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '<');

	// extract tag and place in the file <tag
	MstrView nametag = MstrViewFromBuffer(xmlcontext->pool, objectName, name_len);
    xmlcontext->openedTags = XMLTAGSPush(xmlcontext->openedTags, nametag);
	MViewPushBackArray(XMLFILE, xmlcontext->xmlfile, MstrView, nametag);

	// extract params and push into the file <tag param=1
	const size_t isVoidConst = strlen("isVoid");
    MstrView left = {0}, params = MstrViewFromBuffer(xmlcontext->pool, param, param_len);
    while(!MstrViewIsEmpty(left = MstrViewSplit(params, ',', chareq, &params))) {
		if (!MstrViewIsEmpty(MstrViewFindSpan(left, "isVoid", isVoidConst, chareq))) continue;
		*(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), ' ');
        MViewPushBackArray(XMLFILE, xmlcontext->xmlfile, MstrView, MstrViewTrimLeft(MstrViewTrim(left, ' ', chareq), '.', chareq));
    }

	//close the tag <tag param=1>
    *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '>');
    
    *level = *level + 1; 
    for (int i = 0; i < *level; i++)
        *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '\t');
    
    *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '\n');
    return 0;
}

int MwriteXMLCloseTag(MXMLContext* xmlcontext, int* level) {
    *level = *level - 1; 
    if (!xmlcontext->sameLine)
        for (int i = 0; i < *level; i++)
            *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '\t');

    *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '<');
    *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '/');
    MViewPushBackArray(XMLFILE, xmlcontext->xmlfile, MstrView, XMLTAGSPop(xmlcontext->openedTags));
    *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '>');
    *(xmlcontext->xmlfile) = XMLFILEPush(*(xmlcontext->xmlfile), '\n');
    xmlcontext->sameLine = false;
    return 1;
}

int MwriteInXMLTag(MXMLContext* xmlcontext, MstrView string, int* level) {
    xmlcontext->sameLine = true;
    char popped;
    for (int i = 0; i < *level + 1; i++) {
        popped = XMLFILEPop(*(xmlcontext->xmlfile)); UNUSED(popped);
    }
    MViewPushBackArray(XMLFILE, xmlcontext->xmlfile, MstrView, string);
    return 1;
}


MXMLContext XMLContextFree(MXMLContext context) {
	printf("freeing: %zu bytes\n", context.openedTags->cap);
    XMLTAGSFree(context.openedTags);
    context.openedTags = NULL;
    context.pool = NULL;
    return context;
}

#define CONFORM__interface__(x) $(__interface__ x)
#define MXMLBegin(pool, xmlfile, i) \
		for (i __interface__ = {.tagInterface=true }; __interface__.tagInterface == true; __interface__.tagInterface = false)\
		for(\
			MXMLContext ctx__ = MXMLContextInit(pool, xmlfile);\
            ctx__.openedTags != NULL;\
            ctx__ = XMLContextFree(ctx__)\
        ) \
		for (int __level__ = 0; __level__ == 0; __level__++)
    
#define mx(name, ...) for(\
        int UNIQUE_NAME(__i) = MwriteXMLOpenTag(&ctx__, #name, strlen(#name), #__VA_ARGS__, strlen(#__VA_ARGS__), &__level__ FOREACH(CONFORM__interface__, __VA_ARGS__), NULL);\
            UNIQUE_NAME(__i) == 0;\
            UNIQUE_NAME(__i) = MwriteXMLCloseTag(&ctx__, &__level__)\
        )
#define mx$(...) MwriteInXMLTag(&ctx__, MStrFmt(ctx__.pool, __VA_ARGS__), &__level__)
#endif