#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void timer_handler(int sig,siginfo_t *si,void *uc);
void create_deadline_handler(int pid,int milisec,int deadline);
void timer_start();
