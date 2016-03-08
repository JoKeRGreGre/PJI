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
* Ajoute la structure str au tableau de structure str_time_deadline
*/
void
str_add (str_time_deadline str)
{
    if (tab[ptask_idx].timer == 0x0)
      {
	tab[ptask_idx] = str;
	return;
      }

}


/**
* reset le temps d'execution du processus.
* utiliser cette fonction après chaque periode du processus
*/
void
reset_timer (int pid)
{
  tab[ptask_idx].temps_exec = 0;
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
  if (tab[ptask_idx].temps_exec >= tab[ptask_idx].dead_line)
    {
      kill (tab[ptask_idx].pid, SIGKILL);
      timer_delete (tab[ptask_idx].timer);
    }
}

void
create_deadline_handler (int task_pid)
{
  struct sigevent sev;
  struct sigaction sa;
  int pid = task_pid;
  timer_t timer;
  struct itimerspec its;
  unsigned long dead_line =
    (unsigned long) ptask_get_deadline (ptask_idx, SEC);
  dead_line = dead_line * 1000000000;

  /*Creation de la sigaction */
  sa.sa_handler = timer_handler;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset (&sa.sa_mask);
  sigaction (SIG, &sa, NULL);

  /*Creation de la structure */
  struct str_time_deadline str;
  str.dead_line = dead_line;
  str.temps_exec = 0;
  str.timer = timer;
  str.its = its;
  str.pid = pid;
  str_add (str);

  /*Creation de l'event */
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIG;
  sev.sigev_value.sival_ptr = &(str.timer);
  timer_create (CLOCK_REALTIME, &sev, &(str.timer));

}


void
timer_start (int pid, int milisec)
{
  /*Creation du timer */
  if (milisec < 999)
    {
      tab[ptask_idx].its.it_value.tv_sec = 0;
      tab[ptask_idx].its.it_value.tv_nsec = milisec * 1000000;
    }
  else
    {
      tab[ptask_idx].its.it_value.tv_sec = milisec / 1000;
      tab[ptask_idx].its.it_value.tv_nsec = 0;
    }
  tab[ptask_idx].its.it_interval.tv_sec = tab[ptask_idx].its.it_value.tv_sec;
  tab[ptask_idx].its.it_interval.tv_nsec = tab[ptask_idx].its.it_value.tv_nsec;

  if (timer_settime (tab[ptask_idx].timer, 0, &(tab[ptask_idx].its), NULL) < 0)
    {
      printf ("Erreur création timer\n");
      exit (EXIT_FAILURE);
    }
}
