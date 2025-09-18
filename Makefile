CC ?= gcc
AR ?= ar
CFLAGS ?= -Wall -Wextra -O2 -Iinclude -I/usr/include/libnl3
ARFLAGS ?= rcs

SRC_DIR := src
OBJ_DIR := obj
LIB_NAME := libieee1905.so

LIBOBJS := timer.o \
           utils.o \
           cmdu.o \
           cmdu_ackq.o \
           ieee_1905_wsc.o \
           ieee_1905_cmdu.o \
           lldp.o

OBJS := $(addprefix $(OBJ_DIR)/, $(LIBOBJS))

.PHONY: all clean

all: $(LIB_NAME)

$(LIB_NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(LIB_NAME)