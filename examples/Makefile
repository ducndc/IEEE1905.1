CC      ?= gcc
AR      ?= ar
CFLAGS  ?= -Wall -Wextra -O2 -fPIC -I../include -I/usr/include/libnl3
LDFLAGS ?= -L.. -lieee1905 -lcrypto -lnl-3 -lnl-genl-3

LIB_DIR := ..

LIB_SHARED := $(LIB_DIR)/libieee1905.so

LIBOBJS := timer.o utils.o cmdu.o cmdu_ackq.o ieee_1905_wsc.o crypt_util.o debug.o
OBJS    := $(addprefix $(OBJ_DIR)/, $(LIBOBJS))

EXAMPLESRC := cmdu_test.c
EXE        := cmdu_test

.PHONY: all clean

all: $(EXE)

# Build executable
$(EXE): $(EXAMPLESRC) $(LIB_SHARED)
	$(CC) $(CFLAGS) -o $@ $(EXAMPLESRC) $(LDFLAGS)

clean:
	rm -rf $(EXE)
