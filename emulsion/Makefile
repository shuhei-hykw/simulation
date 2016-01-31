#!/usr/bin/make
##
#  file: Makefile
#  date: 2015.07.02
#
#

SRC_DIR	= src
BIN_DIR	= bin
BLD_DIR	= $(SRC_DIR)/build

CXX	= g++

root_config	= root-config
RTINCLUDE	= $(shell $(root_config) --cflags)
RTLIBS		= $(shell $(root_config) --libs) -lNew

CFLAGS	= -g -O3 -Wall

FLAGS	= $(CFLAGS) -I. $(RTINCLUDE) $(DFLAGS)
LIBS	= $(RTLIBS) -lz

SRCS	= $(wildcard $(SRC_DIR)/*.cc)
DEPS	= $(SRCS:$(SRC_DIR)/%.cc=$(BLD_DIR)/%.d)
OBJS	= $(SRCS:$(SRC_DIR)/%.cc=$(BLD_DIR)/%.o)
TGT	= $(SRCS:$(SRC_DIR)/%.cc=$(BIN_DIR)/%)

#===========================================================================#

.PHONY: all clean distclean

all: $(TGT)

$(TGT): $(OBJS)
	@ echo "- Linking $@"
	@ $(CXX) -o $@ $(@:$(BIN_DIR)/%=$(BLD_DIR)/%.o) $(LIBS)

clean:
	@ echo "==> Cleaning up ..."
	@ rm -rfv $(BLD_DIR)
	@ find . \( -name "*~" -o -name "\#*\#" \) -exec rm -fv \{\} \;

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cc
	@ echo "- Compiling $@"
	@ mkdir -p $(BLD_DIR)
	@ set -e; \
	$(CXX) $(FLAGS) -o $@ -MMD -c $<

-include $(DEPS)
