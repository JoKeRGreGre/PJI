CFLAGS = -Wall -I../src/ -g
LDFLAGS=-L ../src -lptask -lpthread -lrt -g
CC=gcc
LD=gcc

.SUFFIXES:
.SUFFIXES: .o .c

EXECS = time key key_opt test_cores task_create task_aperiodic multi_task

SRCS_time = time.c 
OBJS_time = ${SRCS_time:.c=.o}

SRCS_key = key.c 
OBJS_key = ${SRCS_key:.c=.o}

SRCS_key_opt = key_opt.c 
OBJS_key_opt = ${SRCS_key_opt:.c=.o}

SRCS_test_cores = test_cores.c 
OBJS_test_cores = ${SRCS_test_cores:.c=.o}

SRCS_task_create = task_create.c 
OBJS_task_create = ${SRCS_task_create:.c=.o}

SRCS_task_aperiodic = task_aperiodic.c 
OBJS_task_aperiodic = ${SRCS_task_aperiodic:.c=.o}

SRCS_multi_task = multi_task.c
OBJS_multi_task = ${SRCS_multi_task:.c=.o}

all: $(EXECS)

-include Makefile.deps

depend:
	$(CC) $(CFLAGS) -MM *.[ch] > Makefile.deps

.c.o: 
	$(CC) $(CFLAGS) -c $<

time: $(OBJS_time) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS) 

key: $(OBJS_key) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS) 

key_opt: $(OBJS_key_opt) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS) 

test_cores: $(OBJS_test_cores) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS) 

task_create: $(OBJS_task_create) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS) 

task_aperiodic: $(OBJS_task_aperiodic) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS)

multi_task: $(OBJS_multi_task) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS)


clean:
	rm -rf *.o
	rm -rf $(EXECS)

cleanedit:
	rm -rf *~

