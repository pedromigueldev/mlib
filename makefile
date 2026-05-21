CC=gcc
DEBUG_FLAGS     = -g3 \
                  -Wall -Wextra -Wconversion -Wsign-conversion \
                  -fsanitize=address \
                  -fsanitize=undefined \
                  -fsanitize=leak\
                  -std=c11
                  
LDFLAGS         = -pie
CFLAGS          = $(DEBUG_FLAGS)

all: main.c
	rm -rf test_dir && $(CC) main.c -o main $(CFLAGS)

run: main
	export UBSAN_OPTIONS=print_stacktrace=1
	./main
