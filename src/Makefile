CC         =gcc
CFLAGS     = -W -Wall
DEBUGFLAG  = -DMODE_DEBUG 
DEBUGFLAG  += -g
LIBFLAG    = -DMODE_PTHREAD
LDFLAGS    =
EXEC       =exe
EXEC_FIBO  =fibo

RECOMPILE  =false


ifneq ($(pthread),)
ALLFLAG += $(LIBFLAG)
LDFLAGS += -lpthread
endif


ifneq ($(debug),)
ALLFLAG += $(DEBUGFLAG)
endif



all: $(EXEC)

exe: example.o thread_mix.o thread.o structure.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC)

fibo: fibo.o thread_mix.o thread.o structure.o 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC_FIBO)

example.o: example.c thread_mix.h 
	$(CC) $(CFLAGS) $(ALLFLAG) -c example.c 

fibo.o: fibo.c thread_mix.c thread_mix.h 
	$(CC) $(CFLAGS) $(ALLFLAG) -c fibo.c

thread_mix.o: thread_mix.c thread_mix.h thread.h 
	$(CC) $(CFLAGS) $(ALLFLAG) -c thread_mix.c

thread.o: thread.c thread.h thread_tools.h  structure.h 
	$(CC) $(CFLAGS) $(ALLFLAG) -c thread.c

structure.o: structure.c structure.h thread.c thread_tools.h 
	$(CC) $(CFLAGS) $(ALLFLAG) -c structure.c

clean:
	-rm -f *.o *# *~ 
#	clear ; ls

mrproper: clean
	-rm -f $(EXEC) $(EXEC_FIBO)
#	clear ; ls
