# gcc - compiler
CC=gcc
#compiler flags
CFLAGS=-o NSHOOT.exe -lgdi32 -lws2_32
#main file
MAIN=main

default: build

build:
	$(CC) $(MAIN).c $(CFLAGS)

clean:
	del NSHOOT.exe