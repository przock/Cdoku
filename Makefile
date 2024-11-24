# Compiler and flags
CC = clang
CFLAGS = -v -Wall -Wextra -O3 $(shell pkg-config --cflags libcjson)  # Use pkg-config for compiler flags
LDFLAGS = $(shell pkg-config --libs libcjson)  # Use pkg-config for linker flags

# Target executable
TARGET = Cdoku

# Source files
SRCS = main.c board.c solver.c cdoku.c

# Object files (generated from the source files)
OBJS = $(SRCS:.c=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)  # Link with cJSON library

# Rule to compile .c files to .o object files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Run the target after compilation
run: $(TARGET)
	rm -f $(OBJS)
	./$(TARGET)

# Phony targets (not actual files)
.PHONY: all clean

