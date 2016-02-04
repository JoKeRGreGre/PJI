#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define SIG CLD_STOPPED

long dead_line;
long temps_exec;
timer_t timer;
struct itimerspec its;

static void timer_handler(int sig,siginfo_t *si,void *uc){
	temps_exec=temps_exec+its.it_interval.tv_nsec;
	if( (temps_exec + its.it_interval.tv_nsec)<=dead_line)
		printf("%ld\n",temps_exec);
	else{
		kill(si->si_pid,SIGKILL);
	}
}


int main(){

struct sigevent sev;
struct sigaction sa;
dead_line = 24000000;
printf("Creation de la sigaction\n");

sa.sa_handler = timer_handler;
sa.sa_flags = SA_SIGINFO;
sigemptyset(&sa.sa_mask);
sigaction(SIG,&sa,NULL);

printf("Creation de l'event\n");
sev.sigev_notify = SIGEV_SIGNAL;
sev.sigev_signo =SIG;
sev.sigev_value.sival_ptr = &timer;
timer_create(CLOCK_REALTIME,&sev,&timer);

printf("Creation du timer\n");
its.it_value.tv_sec=0;
its.it_value.tv_nsec=6000000;
its.it_interval.tv_sec = its.it_value.tv_sec;
its.it_interval.tv_nsec = its.it_value.tv_nsec;

printf("Lancement du timer\n");
timer_settime(timer,0,&its,NULL);
while(1);
exit(EXIT_SUCCESS);
}
