# Source directories
SRC_DIR := .
VALIDATIONS_DIR := validations

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
VALIDATIONS_SRCS := $(wildcard $(VALIDATIONS_DIR)/*.c)

# Object files
OBJS := $(SRCS:.c=.o)
VALIDATIONS_OBJS := $(VALIDATIONS_SRCS:.c=.o)

# Compiler options
CC := gcc
CFLAGS := -Wall -ansi -pedantic -g

# Target executable
TARGET := main.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(VALIDATIONS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(VALIDATIONS_OBJS)
