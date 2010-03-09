TARGETS = create yield param mutex
LIBOBJ = othread.o dlink.o
CC = gcc -Wall

.PHONY: clean all works

all: $(TARGETS)

othread.o : othread.c othread.h errstop.h
	$(CC) -c othread.c
dlink.o : dlink.c dlink.h 
	$(CC) -c dlink.c
%: 	%.c $(LIBOBJ) othread.h dlink.h
	$(CC) -o $@ $< $(LIBOBJ)
clean:
	rm $(TARGETS) $(LIBOBJ)
