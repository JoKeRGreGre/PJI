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
int pid;


void timer_handler(int sig,siginfo_t *si,void *uc){
	temps_exec=temps_exec+its.it_interval.tv_nsec;
	if( temps_exec>dead_line){
		timer_delete(timer);
		kill(pid,SIGKILL);
	}
}

/**
*	Creation d'un timer qui verifie le temps d'exection d'une tache toutes les p milisecondes si le prochain temps d'execution de la tache devrais dépasser l'echeance, un signal est envoyé pour tuer le processus.

	task_pid : pid du processus
	milisec : tic du timer (miliseconde)
	dl : echance (miliseconde)
*/
void create_deadline_handler(int task_pid,int milisec,int deadline){
	struct sigevent sev;
	struct sigaction sa;
	pid=task_pid;
	dead_line = deadline*1000000;
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
	/*Creation du timer*/
	its.it_value.tv_sec=0;
	its.it_value.tv_nsec=milisec*1000000;
	its.it_interval.tv_sec = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;

}

void timer_start(){
	timer_settime(timer,0,&its,NULL);
}
