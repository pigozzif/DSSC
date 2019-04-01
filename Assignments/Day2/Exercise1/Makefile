CC = gcc
CCFLAGS = -std=c99 -fopenmp
SRC = Exercise1.c
TARGET = exercise1.x

all: $(TARGET)

$(TARGET): $(SRC)
  $(CC) $(CCFLAGS) -o $@ $^
  
.PHONY: all clean

clean:
  rm $(TARGET)
