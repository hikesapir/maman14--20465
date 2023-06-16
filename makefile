# Source directories
SRC_DIR := src
UTILS_DIR := $(SRC_DIR)/utils
VALIDATIONS_DIR := $(SRC_DIR)/validations
LOGGER_DIR := $(SRC_DIR)/logger
MACRO_DIR := $(SRC_DIR)/macros
ALGORITHEM_DIR := $(SRC_DIR)/algorithem

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
UTILS_SRCS := $(wildcard $(UTILS_DIR)/*.c)
VALIDATIONS_SRCS := $(wildcard $(VALIDATIONS_DIR)/*.c)
LOGGER_SRCS := $(wildcard $(LOGGER_DIR)/*.c)
MACRO_SRCS := $(wildcard $(MACRO_DIR)/*.c)
ALGORITHEM_SRCS := $(wildcard $(ALGORITHEM_DIR)/*.c)

# Object files
OBJS := $(SRCS:.c=.o)
UTILS_OBJS := $(UTILS_SRCS:.c=.o)
VALIDATIONS_OBJS := $(VALIDATIONS_SRCS:.c=.o)
LOGGER_OBJS := $(LOGGER_SRCS:.c=.o)
MACRO_OBJS := $(MACRO_SRCS:.c=.o)
ALGORITHEM_OBJS := $(ALGORITHEM_SRCS:.c=.o)

# Compiler options
CC := gcc
CFLAGS := -Wall -ansi -pedantic -g

# Target executable
TARGET := main.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(UTILS_OBJS) $(VALIDATIONS_OBJS) $(LOGGER_OBJS) $(MACRO_OBJS) $(ALGORITHEM_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(UTILS_OBJS) $(VALIDATIONS_OBJS) $(LOGGER_OBJS) $(MACRO_OBJS) $(ALGORITHEM_OBJS)
