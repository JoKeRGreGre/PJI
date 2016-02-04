CFLAGS = -Wall -I../src/
LDFLAGS=-L ../src -lptask -lpthread -lrt
CC=gcc
LD=gcc

.SUFFIXES:
.SUFFIXES: .o .c

EXECS = time key key_opt test_cores task_create task_aperiodic yann sig

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

SRCS_yann = yann.c 
OBJS_yann = ${SRCS_yann:.c=.o}

SRCS_sig = sig.c 
OBJS_sig = ${SRCS_sig:.c=.o}


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

yann: $(OBJS_yann) ../src/libptask.a ./deadline_handler.c
	$(LD) -o $@ $^ $(LDFLAGS)

sig: $(OBJS_sig) ../src/libptask.a
	$(LD) -o $@ $^ $(LDFLAGS)


clean:
	rm -rf *.o
	rm -rf $(EXECS)

cleanedit:
	rm -rf *~

