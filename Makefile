CC=g++

CFLAGS= -std=c++11 -o main
SDL2= `sdl2-config --libs --cflags` -lSDL2
SOURCES= main.cpp ./source/*

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=main

all:

	$(CC) $(CFLAGS) $(SOURCES) $(SDL2)

clean:

	rm *o main a.out
