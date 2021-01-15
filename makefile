CC = gcc
CCFLAGS = -Wall -lncurses -pedantic -std=c99

SOURCES = main.c model.c render.c
OBJECTS = $(SOURCES:.c=.o)
OUTPUT = tetris

all: $(OBJECTS)
	$(CC) -o $(OUTPUT) $^ $(CCFLAGS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

clean:
	rm *.o