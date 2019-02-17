# Makefile 

CC = gcc
MAINSRCS = main.c

OTHSRCS += board.c
OTHSRCS += tree.c
OTHSRCS += book.c
OTHSRCS += search.c
OTHSRCS += uiinc.c

HEADERS += macro.h
HEADERS += pattern.h
HEADERS += mvlist.h
HEADERS += board.h
HEADERS += key.h
HEADERS += tree.h
HEADERS += book.h
HEADERS += search.h
HEADERS += uiinc.h

SRCS = $(MAINSRCS) $(OTHSRCS)
SRCS = $(MAINSRCS) $(OTHSRCS)
OBJS = $(SRCS:.c=.o)
TARGETS = main
MEMCHECKARGS = 

CFLAGS = -g -Wall -std=c99
#CFLAGS = -O3 -std=c99

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

all: $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS)

memcheck: $(TARGETS)
	valgrind --leak-check=full ./$(TARGETS) $(MEMCHECKARGS)

clean:
	mv -f $(OBJS) $(TARGETS) ~/trash

.PHONY: all memcheck clean

