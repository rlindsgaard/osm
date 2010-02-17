CC			=gcc
CFLAGS 		=-Wall -pedantic -std=c99 -ggdb
LDFLAGS		=-lpthread
EXECUTABLE	=threadexample
SOURCES		=main.c pqueue.c wqueue.c pqueue-test.c

default: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXECUTABLE) $(SOURCES)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -r $(EXECUTABLE) $(EXECUTABLE).dSYM

totalitarian: 
	splint -strict -preproc $(SOURCES)
