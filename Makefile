CC=gcc
FLAGS=--std=c11 -O3
SOURCES=main.c
OBJECTS=$(SOURCES:%.c=%.o)
TARGET=yin

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(FLAGS)

%.o: %.c
	$(CC) $^ -c -o $@ $(FLAGS)

clean: 
	-rm *.o $(TARGET)
