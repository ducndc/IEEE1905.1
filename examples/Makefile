CC ?= gcc
AR ?= ar
CFLAGS ?= -Wall -Wextra -O2 -I../include -I/usr/include/libnl3
LDFLAGS ?= -L.. -lieee1905 -lcrypto -lnl-3 -lnl-genl-3

SRC_DIR := ../src
OBJ_DIR := ../obj
LIB_NAME := ../libieee1905.a

LIBOBJS := timer.o utils.o cmdu.o cmdu_ackq.o ieee_1905_wsc.o crypt_util.o debug.o
OBJS := $(addprefix $(OBJ_DIR)/, $(LIBOBJS))

EXAMPLESRC := cmdu_test.c
EXE := cmdu_test

.PHONY: all clean

all: $(LIB_NAME) $(EXE)

$(LIB_NAME): $(OBJS)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXE): $(EXAMPLESRC) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(EXAMPLESRC) $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(EXE)