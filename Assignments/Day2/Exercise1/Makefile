CC = gcc
CCFLAGS = -std=c99 -fopenmp
CRITICAL_SRC = critical.c 
ATOMIC_SRC = atomic.c 
REDUCTION_SRC = reduction.c
CRITICAL_TARGET = critical.x
ATOMIC_TARGET = atomic.x
REDUCTION_TARGET = reduction.x

all: $(CRITICAL_TARGET) $(ATOMIC_TARGET) $(REDUCTION_TARGET)

$(CRITICAL_TARGET): $(CRITICAL_SRC)
	$(CC) $(CCFLAGS) -o $@ $^

$(ATOMIC_TARGET): $(ATOMIC_SRC)
	$(CC) $(CCFLAGS) -o $@ $^
	
$(REDUCTION_TARGET): $(REDUCTION_SRC)
	$(CC) $(CCFLAGS) -o $@ $^


.PHONY: all clean

clean:
	rm *.x
