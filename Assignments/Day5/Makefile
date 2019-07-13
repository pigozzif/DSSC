CC = nvcc
SRC = transpose.cu
TARGET = cuda_transpose.x

all: $(TARGET)

$(TARGET): $(SRC)
        $(CC) -o $@ $^

.PHONY: all clean

clean:
      	rm $(TARGET)
