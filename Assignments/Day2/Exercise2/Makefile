CC = gcc
CCFLAGS = -std=c99 -fopenmp
SRC = Exercise2.c
TARGET = exercise2.x

all: $(TARGET)

$(TARGET): $(SRC)
  $(CC) $(CCFLAGS) -o $@ $^
  
.PHONY: all clean

clean:
  rm $(TARGET)
