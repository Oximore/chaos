CC=gcc
CFLAGS= -W -Wall
LDFLAGS=
EXEC=exe
EXEC_DEBUG=exe_debug

all: $(EXEC) debug

exe: example.o thread.o list.o
	$(CC) $(LDFLAGS) $^ -o $(EXEC)

debug: example_debug.o thread_debug.o list_debug.o
	$(CC) $(LDFLAGS) $^ -o $(EXEC_DEBUG) 

example.o: thread.h
	$(CC) $(CFLAGS) -c example.c

example_debug.o: thread.h
	$(CC) $(CFLAGS) -c example.c -o $@

thread.o: thread.c thread.h list.h
	$(CC) $(CFLAGS) -c thread.c

thread_debug.o: thread.c thread.h list.h
	$(CC) $(CFLAGS) -DMODE_DEBUG -c thread.c -o $@

list.o: list.c thread.h list.h
	$(CC) $(CFLAGS) -c list.c

list_debug.o: list.c thread.h list.h
	$(CC) $(CFLAGS) -c list.c -o $@

clean:
	rm -f *.o $(EXEC) $(EXEC_DEBUG) *# *~