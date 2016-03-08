#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "ptask.h"
#include "deadline_handler.h"


#define SIG CLD_STOPPED


/* Struct and var */
extern __thread int ptask_idx;
struct str_time_deadline tab[MAX_TASKS];

struct itimerspec its;

/* functions */
void timer_handler(int sig,siginfo_t *si,void *uc);
int get_index_from_pid(int pid);


/**
* Retourne l'index du processus dans le tableau en fonction de son pid
*/
int get_index_from_pid(int pid){
int i;
for(i=0;i<MAX_TASKS;i++)
	if(tab[i].pid == pid)
		return i;
return -1;
}

/**
* Ajoute la structure str au tableau de structure str_time_deadline
*/
void str_add(str_time_deadline str){

int i;
for(i=0;i<MAX_TASKS;i++)
	if(tab[i].timer==0x0){
		tab[i]=str;
		return;
	}	

}


void timer_handler(int sig,siginfo_t *si,void *uc){
int index = get_index_from_pid(sig);
		tab[index].temps_exec=tab[index].temps_exec+tab[index].its.it_interval.tv_nsec;
	if( tab[index].temps_exec>tab[index].dead_line){;
		printf("fin d'exec : %ld\n",tab[index].temps_exec);
		printf("dead_line : %ld\n",tab[index].dead_line);
		kill(tab[index].pid,SIGKILL);
		timer_delete(tab[index].timer);
	}
	else{	
		tab[index].temps_exec = 0;
	}
}

void create_deadline_handler(int task_pid){
	struct sigevent sev;
	struct sigaction sa;
	int pid=task_pid;
	timer_t timer;
	struct itimerspec its;
	unsigned long dead_line =(unsigned long)ptask_get_deadline(ptask_idx,SEC);
dead_line = dead_line*1000000000;

	/*Creation de la sigaction*/
	sa.sa_handler = timer_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIG,&sa,NULL);

	/*Creation de la structure*/
	struct str_time_deadline str;
	str.dead_line = dead_line;
	str.temps_exec= 0;
	str.timer=timer;
	str.its=its;
	str.pid=pid;
	str_add(str);

	/*Creation de l'event*/
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo =SIG;
	sev.sigev_value.sival_ptr = &(str.timer);
	timer_create(CLOCK_REALTIME,&sev,&(str.timer));
}


void timer_start(int pid,int milisec){
int index = get_index_from_pid(pid);
	/*Creation du timer*/
	if(milisec<999){
		tab[index].its.it_value.tv_sec=0;
		tab[index].its.it_value.tv_nsec=milisec*1000000;
	}
	else{
		tab[index].its.it_value.tv_sec=milisec/1000;
		tab[index].its.it_value.tv_nsec=0;
	}
	tab[index].its.it_interval.tv_sec = tab[index].its.it_value.tv_sec;
	tab[index].its.it_interval.tv_nsec = tab[index].its.it_value.tv_nsec;
	

	if (timer_settime(tab[index].timer,0,&(tab[index].its),NULL) < 0) {
		printf("Erreur création timer\n");
		exit(EXIT_FAILURE);
	}
}
