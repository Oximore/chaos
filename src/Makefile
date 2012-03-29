CC=gcc
CFLAGS= -W -Wall
LDFLAGS=
EXEC=exe
EXEC_DEBUG=exe_debug
EXEC_FIBO=fibo

all: $(EXEC) 

both: $(EXEC) debug

exe: example.o thread.o list.o
	$(CC) $(LDFLAGS) $^ -o $(EXEC)

debug: example_debug.o thread_debug.o list.o
	$(CC) $(LDFLAGS) $^ -o $(EXEC_DEBUG) 

fibo: fibo.o thread.o list.o
	$(CC) $(LDFLAGS) $^ -o $(EXEC_FIBO)

example.o: thread.h
	$(CC) $(CFLAGS) -c example.c

example_debug.o: thread.h
	$(CC) $(CFLAGS) -c example.c -o $@

fibo.o: fibo.c thread.h
	$(CC) $(CFLAGS) -c fibo.c

thread.o: thread.c thread.h list.h
	$(CC) $(CFLAGS) -c thread.c

thread_debug.o: thread.c thread.h list.h
	$(CC) $(CFLAGS) -DMODE_DEBUG -c thread.c -o $@

list.o: list.c thread.h list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f *.o $(EXEC) $(EXEC_DEBUG) $(EXEC_FIBO) *# *~ 
