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

/* functions */
void timer_handler (int sig, siginfo_t * si, void *uc);


/**
* reset le temps d'execution du processus.
* utiliser cette fonction après chaque periode du processus
*/
void
reset_timer () 
{
printf("timer [%d] reset\n",tab[ptask_idx].pid);
    tab[ptask_idx].temps_exec = 0;
}

void delete_timer()
{
if(	!(tab[ptask_idx].its.it_interval.tv_sec==0
	&&tab[ptask_idx].its.it_interval.tv_nsec==0)){

        printf("timer [%d] delete\n",tab[ptask_idx].pid);
      tab[ptask_idx].its.it_value.tv_sec =0;
      tab[ptask_idx].its.it_value.tv_nsec = 0;
  tab[ptask_idx].its.it_interval.tv_sec = 0;
  tab[ptask_idx].its.it_interval.tv_nsec = 0;
  if (timer_settime (tab[ptask_idx].timer, 0, &(tab[ptask_idx].its), NULL) < 0) {
      printf ("Erreur création timer\n");
      exit (EXIT_FAILURE);
  }

    if(timer_delete(tab[ptask_idx].timer)<0)
	printf("error suppression timer\n");
}

}

/**
* Fonction lance par le timer a chaque tic
*/
void
timer_handler (int sig, siginfo_t * si, void *uc)
{

    tab[ptask_idx].temps_exec += tab[ptask_idx].its.it_interval.tv_nsec;
    /*
      printf ("fin d'execution : \t%lu\n", tab[ptask_idx].temps_exec);
      printf ("dead_line : \t\t%lu\n", tab[ptask_idx].dead_line);

    */
        //printf("tic processus %d\n",tab[ptask_idx].pid);
    if (tab[ptask_idx].temps_exec > tab[ptask_idx].dead_line) {
printf("DEPASSEMMENT :timer [%d] temps exec : %ld > deadline : %ld\n",tab[ptask_idx].pid,tab[ptask_idx].temps_exec ,tab[ptask_idx].dead_line);
        delete_timer();
	pthread_exit(0);
        //kill (tab[ptask_idx].pid, SIGKILL);
    }
}

void
create_deadline_handler (int task_pid)
{
    printf("nouvelle tache : %d\n", task_pid);
    struct sigevent sev;
    struct sigaction sa;
    //timer_t timer;
    struct itimerspec its;

    unsigned long dead_line =
        (unsigned long) ptask_get_deadline (task_pid, NANO);
    //dead_line = dead_line * 1000000000;
    printf("deadline : %ld\n", dead_line);

    /*Creation de la sigaction */
    sa.sa_sigaction = timer_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset (&sa.sa_mask);
    sigaction (SIG, &sa, NULL);

    /*Creation de la structure */
    //struct str_time_deadline str;
    tab[task_pid].dead_line = dead_line;
    tab[task_pid].temps_exec = 0;
    tab[task_pid].its = its;
    tab[task_pid].pid = task_pid;

    /*Creation de l'event */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &(tab[task_pid].timer);
    timer_create (CLOCK_REALTIME, &sev, &(tab[task_pid].timer));
}


void
timer_start (int milisec)
{
  /*Creation du timer */
  if (milisec < 999) {
      tab[ptask_idx].its.it_value.tv_sec = 0;
      tab[ptask_idx].its.it_value.tv_nsec = milisec * 1000000;
  }
  else {
      tab[ptask_idx].its.it_value.tv_sec = milisec / 1000;
      tab[ptask_idx].its.it_value.tv_nsec = 0;
  }

  tab[ptask_idx].its.it_interval.tv_sec = tab[ptask_idx].its.it_value.tv_sec;
  tab[ptask_idx].its.it_interval.tv_nsec = tab[ptask_idx].its.it_value.tv_nsec;
  
  if (timer_settime (tab[ptask_idx].timer, 0, &(tab[ptask_idx].its), NULL) < 0) {
      printf ("Erreur création timer\n");
      exit (EXIT_FAILURE);
  }
}
