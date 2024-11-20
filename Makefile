# Compiler and flags
CC = clang
CFLAGS = -v -Wall -Wextra -O0 -g  # Enable detailed warnings and optimizations

# Target executable
TARGET = Cdoku

# Source files
SRCS = main.c board.c solver.c

# Object files (generated from the source files)
OBJS = $(SRCS:.c=.o)

# Default target to build the executable
all: $(TARGET)
	rm -f $(OBJS)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Rule to compile .c files to .o object files
%.o: %.c board.h solver.h
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
