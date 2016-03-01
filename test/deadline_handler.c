#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>


#define SIG CLD_STOPPED

void timer_handler(int sig,siginfo_t *si,void *uc);
int get_index_from_pid(int pid);
str_time_deadline[MAX_TASKS];


int get_index_from_pid(int pid){
int i;
for(i=0;i<MAX_TASKS;i++)
	if(str_time_deadline[i].pid == pid)
		return i;
return 0;
}

void timer_handler(int sig,siginfo_t *si,void *uc){
int index = get_index_from_pid(sig);
	str_time_deadline[index].temps_exec=str_time_deadline[index].temps_exec+its.it_interval.tv_nsec;
//printf("%ld\n",temps_exec);
//printf("fin d'exec : %ld\n",dead_line);
	if( temps_exec>dead_line){;
		timer_delete(timer);
		kill(pid,SIGKILL);
	}
}

void create_deadline_handler(int task_pid){
	struct sigevent sev;
	struct sigaction sa;
	pid=task_pid;
	dead_line =(unsigned long) ptask_get_deadline()*1000000;

	/*Creation de la sigaction*/
	sa.sa_handler = timer_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIG,&sa,NULL);


	/*Creation de l'event*/
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo =SIG;
	sev.sigev_value.sival_ptr = &timer;
	timer_create(CLOCK_REALTIME,&sev,&timer);


}

void timer_start(int milisec){
	/*Creation du timer*/

	if(milisec<999){
		its.it_value.tv_sec=0;
		its.it_value.tv_nsec=milisec*1000000;
	}
	else{
		its.it_value.tv_sec=milisec/1000;
		its.it_value.tv_nsec=0;
	}
	its.it_interval.tv_sec = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;

	timer_settime(timer,0,&its,NULL);
}
