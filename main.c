#include "mfile.h"
#include "mstr.h"


#define MFMT(x) _3_$("%.*s", (x)->length, (x)->raw)

int main(void) {
	
	Mstr* file = NULL;
	catch(mfile_read, &file, "main.c") {
		fprintf(stderr, "%s\n", unwrap_fail(file));
		return 1;
	};

	Mstr* token = NULL;
	int count = 0;
	while ((token = mstr_chop_by(file, '\n')) != NULL){
		MPRINT_FMT("QQR NUMERO: "$(count)" :::: "MFMT(token)" \n");
		count++;
	}

	return 0;
}
