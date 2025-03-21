# Makefile for the calculator program

# Compiler to use (e.g., gcc, clang)
CC = gcc

# Compiler flags (e.g., -Wall -g -O2)
CFLAGS = -Wall -g

# Executable name
EXEC = calc

# Source files
SRCS = calc.c

# Header files
HDRS = calc.h

# Object files (not explicitly needed for this simple project, but good practice for larger ones)
OBJS = $(SRCS:.c=.o)

# Default target: "all"
all: $(EXEC)

# Target to build the executable
$(EXEC): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o $(EXEC)

# Target to clean up build artifacts
clean:
	rm -f $(EXEC) $(OBJS) .calc_ans

# Phony targets (targets that are not actual files)
.PHONY: all clean