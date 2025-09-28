CC ?= gcc
CXX      ?= g++
AR ?= ar
CXXFLAGS ?= -Wall -Wextra -O2 -Iinclude -I/usr/include/libnl3
LDFLAGS  ?= -lcrypto -lnl-3 -lnl-genl-3 -lspdlog

SRC_DIR  := src
OBJ_DIR  := obj
LIB_NAME := libieee1905.a

LIBOBJS  := timer.o utils.o cmdu.o cmdu_ackq.o ieee_1905_wsc.o crypt_util.o debug.o
OBJS     := $(addprefix $(OBJ_DIR)/, $(LIBOBJS))

.PHONY: all clean

all: $(LIB_NAME)

$(LIB_NAME): $(OBJS)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(LIB_NAME)