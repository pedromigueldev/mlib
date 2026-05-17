#ifndef MARR_H
#define MARR_H

#define MARRALLOC(ptr, length) malloc((length) * sizeof(typeof(ARR(ptr)[0])))
#define MARRREALLOC(ptr) realloc(ARR(*ptr), ARRCAP(*ptr) * sizeof((ARR(*ptr))[0]));
#define ARR(ptr) ptr##_arr
#define ARRCAP(ptr) ptr##_arr_cap
#define ARRLEN(ptr) ptr##_arr_len

#define Marr_new(ptr, length)								\
	MARRALLOC(ptr, length); 								\
	size_t ARRCAP(ptr) = (length);							\
	size_t ARRLEN(ptr) = 0

#define Marr_new_def(ptr, type, length)						\
	type* ARR(ptr) = MARRALLOC(ptr, length); 				\
	size_t ARRCAP(ptr) = (length);							\
	size_t ARRLEN(ptr) = 0

#define Marr_dft(ptr) Marr_new(ptr, 1000);
#define Marr_dft_def(ptr, type) Marr_new_def(ptr, type, 1000);

#define Marr_put(ptr, value) \
	do {\
		if (ARRLEN(*ptr) >= ARRCAP(*ptr)) {\
			ARRCAP(*ptr) *= 2;\
			ARR(*ptr) = MARRREALLOC(ptr);\
		}\
		(ARR(*ptr))[(ARRLEN(*ptr))++] = value;\
	} while(0)

#define Marr_foreach(item, ptr)\
	if (ARRLEN(*ptr) > 0)																			\
			for(int __k = 1, index = 0; __k && index < (int) ARRLEN(*ptr); __k = !__k, index++)   \
				for(typeof ((ARR(*ptr))[0]) item = (ARR(*ptr))[index]; __k; __k = !__k)	

#define Marr_free(ptr)					\
		Marr_foreach(item, ptr) 		\
			free(&item);		

#define Marr_spreadref(ptr) ARR(ptr), ARRLEN(ptr), ARRCAP(ptr)
#define Marr_param_ref(ptr, t) t* ARR(*ptr), size_t ARRLEN(*ptr), size_t ARRCAP(*ptr)

#endif
