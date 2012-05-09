CC         =gcc
CFLAGS     = -W -Wall
DEBUGFLAG  = -DMODE_DEBUG
LDFLAGS    = 
ALLFLAG    =


EXEC       =exe
EXEC_FIBO  =fibo
EXEC_SUM   =sum
EXEC_BOUCLE=while

OBJETS     =thread_mix.o thread.o structure.o time.o tree_thread.o list_thread.o

RECOMPILE  =false


ifneq ($(pthread),)
ALLFLAG += -DMODE_PTHREAD
LDFLAGS += -lpthread
endif

ifneq ($(debug),)
ALLFLAG += $(DEBUGFLAG)
endif

ifneq ($(valgrind),)
ALLFLAG += -g
endif

ifneq ($(time),)
ALLFLAG += -DTIME_MODE
endif

ifneq ($(list),)
ALLFLAG += -DMODE_LIST
endif



all: $(EXEC) $(EXEC_FIBO) $(EXEC_SUM) $(EXEC_BOUCLE)

exe: example.o $(OBJETS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC)

fibo: fibo.o $(OBJETS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC_FIBO)

sum: sum.o $(OBJETS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC_SUM)

while: boucle.o $(OBJETS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEC_BOUCLE)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(ALLFLAG) -c $< 

example.o: example.c thread_mix.h 

fibo.o: fibo.c thread_mix.c thread_mix.h 

sum.o: sum.c thread_mix.c thread_mix.h 

boucle.o: boucle.c thread_mix.c thread_mix.h

thread_mix.o: thread_mix.c thread_mix.h thread.h 

thread.o: thread.c thread.h thread_tools.h structure.h 

structure.o: structure.c structure.h tree_thread.h list_thread.h

tree_thread.o: tree_thread.c tree_thread.h thread.c thread_tools.h  

list_thread.o: list_thread.c list_thread.h thread.c thread_tools.h 

time.o: time.c time.h

clean:
	@-rm -f *.o *# *~ 
#	clear ; ls

mrproper: clean
	@-rm -f $(EXEC) $(EXEC_FIBO) $(EXEC_SUM) $(EXEC_BOUCLE)
#	clear ; ls


