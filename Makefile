# directory definitions
SRCDIR=.
BUILTINS_SRCDIR=$(SRCDIR)/builtins
symbol_table_SRCDIR=$(SRCDIR)/symbol_table
history_SRCDIR=$(SRCDIR)/history_ulti
BUILD_DIR=$(SRCDIR)/build

# compiler name and flags
CC=gcc
LIBS=
CFLAGS=-Wall -Wextra -g -I$(SRCDIR)
LDFLAGS=-g

# generate the lists of source and object files
SRCS_BUILTINS=$(shell find $(SRCDIR)/builtins -name "*.c")

SRCS_symbol_table=$(SRCDIR)/symbol_table/symbol_table.c

SRCS_history=$(SRCDIR)/history_util/history_util.c

SRCS=main.c prompt.c node.c parser.c scanner.c source.c executor.c initshell.c  \
     pattern.c strings.c wordexp.c shunt.c extension.c                        \
     $(SRCS_BUILTINS) $(SRCS_symbol_table) $(SRCS_history)

OBJS=$(SRCS:%.c=$(BUILD_DIR)/%.o)

# output file name
TARGET=shell

# default target (when we call make with no arguments)
.PHONY: all
all: prep-build $(TARGET)

prep-build:
	mkdir -p $(BUILD_DIR)/builtins
	mkdir -p $(BUILD_DIR)/symbol_table
	mkdir -p $(BUILD_DIR)/history_util

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# target to auto-generate header file dependencies for source files
depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

#compile C source files
$(BUILD_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean target
.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET) core .depend
	$(RM) -r $(BUILD_DIR)
	
