CC=gcc
FLAGS=-g -Wall -std=c23

all: main.c
	$(CC) main.c -o main $(FLAGS)
