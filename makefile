CC=gcc
FLAGS=-g -Wall -std=c23

all: main.c
	rm -rf test_dir && $(CC) main.c -o main $(FLAGS)
