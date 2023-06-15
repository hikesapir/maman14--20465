# Source directories
SRC_DIR := .
SRC_SUBDIRS := src
VALIDATIONS_DIR := $(SRC_SUBDIRS)/validations
LOGGER_DIR := $(SRC_SUBDIRS)/logger

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
SRCS += $(foreach dir,$(SRC_SUBDIRS),$(wildcard $(dir)/*.c))
VALIDATIONS_SRCS := $(wildcard $(VALIDATIONS_DIR)/*.c)
LOGGER_SRCS := $(wildcard $(LOGGER_DIR)/*.c)

# Object files
OBJS := $(SRCS:.c=.o)
VALIDATIONS_OBJS := $(VALIDATIONS_SRCS:.c=.o)
LOGGER_OBJS := $(LOGGER_SRCS:.c=.o)

# Compiler options
CC := gcc
CFLAGS := -Wall -ansi -pedantic -g

# Target executable
TARGET := main.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(VALIDATIONS_OBJS) $(LOGGER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(VALIDATIONS_OBJS) $(LOGGER_OBJS)
