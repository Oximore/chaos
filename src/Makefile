CC         =gcc
CFLAGS     = -W -Wall
DEBUGFLAG  = -DMODE_DEBUG 
DEBUGFLAG  += -g
LDFLAGS    =

EXEC       =exe
EXEC_FIBO  =fibo
EXEC_SUM   =sum

RECOMPILE  =false


ifneq ($(pthread),)
ALLFLAG += -DMODE_PTHREAD
LDFLAGS += -lpthread
endif

ifneq ($(debug),)
ALLFLAG += $(DEBUGFLAG)
endif

ifneq ($(time),)
ALLFLAG += -DTIME_MODE
endif


all: $(EXEC) $(EXEC_FIBO) $(EXEC_SUM)

exe: example.o thread_mix.o thread.o structure.o time.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC)

fibo: fibo.o thread_mix.o thread.o structure.o time.o 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC_FIBO)

sum: sum.o thread_mix.o thread.o structure.o time.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC_SUM)

%.o: %.c
	$(CC) $(CFLAGS) $(ALLFLAG) -c $< 

example.o: example.c thread_mix.h 

fibo.o: fibo.c thread_mix.c thread_mix.h 

sum.o: sum.c thread_mix.c thread_mix.h 

thread_mix.o: thread_mix.c thread_mix.h thread.h 

thread.o: thread.c thread.h thread_tools.h  structure.h 

structure.o: structure.c structure.h thread.c thread_tools.h 

time.o: time.c time.h

clean:
	@-rm -f *.o *# *~ 
#	clear ; ls

mrproper: clean
	@-rm -f $(EXEC) $(EXEC_FIBO) $(EXEC_SUM)
#	clear ; ls
