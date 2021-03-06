#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "ptask.h"
#include "tstat.h"
#include "deadline_handler.h"


#define SIG CLD_STOPPED


/* Struct and var */
extern __thread int ptask_idx;
struct str_time_deadline tab[MAX_TASKS];

/* functions */
void timer_handler (int sig, siginfo_t * si, void *uc);



void
pause_timer ()
{

  if (timer_settime (tab[ptask_idx].timer, 0, &(tab[ptask_idx].sits), NULL)
      < 0)
    {
      printf ("Erreur remise a 0 timer\n");
      exit (EXIT_FAILURE);
    }
}

void
resume_timer ()
{

  unsigned long nano = ptask_get_absolute_deadline (tab[ptask_idx].pid, NANO);

  tab[ptask_idx].its.it_value.tv_sec = 0;
  tab[ptask_idx].its.it_value.tv_nsec = nano;


  tab[ptask_idx].its.it_interval.tv_sec = tab[ptask_idx].its.it_value.tv_sec;
  tab[ptask_idx].its.it_interval.tv_nsec =
    tab[ptask_idx].its.it_value.tv_nsec;


  timer_settime (tab[ptask_idx].timer, 0, &(tab[ptask_idx].its), NULL);
  //printf ("timer [%d]", tab[ptask_idx].pid);
}

void
delete_timer ()
{

  if (!(tab[ptask_idx].its.it_interval.tv_sec == 0
	&& tab[ptask_idx].its.it_interval.tv_nsec == 0))
    {

      pause_timer ();

      if (timer_delete (tab[ptask_idx].timer) < 0)
	printf ("error suppression timer\n");

      printf ("timer [%d] delete\n", tab[ptask_idx].pid);
    }


  tspec wcet = ptask_get_wcet (ptask_idx);
  tspec acet = ptask_get_avg (ptask_idx);

  printf ("TASK %d: WCET = %ld\t ACET = %ld\t NINST=%d\n",
	  ptask_idx,
	  tspec_to (&wcet, MICRO),
	  tspec_to (&acet, MICRO), ptask_get_numinstances (ptask_idx));

}

/**
* Fonction lance automatiquement par le timer a chaque tic
*/
void
timer_handler (int sig, siginfo_t * si, void *uc)
{

  printf ("DEPASSEMMENT :timer [%d]\n", tab[ptask_idx].pid);
  pthread_exit (0);
  //delete_timer ();
}


/*
*Fonction pour creer le controleur d'echeance pour un processus donnée
*/
void
create_deadline_handler (int task_pid)
{
  printf ("nouvelle tache : %d\n", task_pid);
  struct sigevent sev;
  struct sigaction sa;
  //timer_t timer;
  struct itimerspec its;
  struct itimerspec sits;
  sits.it_value.tv_sec = 0;
  sits.it_value.tv_nsec = 0;
  sits.it_interval.tv_sec = 0;
  sits.it_interval.tv_nsec = 0;

  unsigned long dead_line =
    (unsigned long) ptask_get_deadline (task_pid, NANO);
  //dead_line = dead_line * 1000000000;
  printf ("deadline : %ld\n", dead_line);

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
  tab[task_pid].sits = sits;


  /*Creation de l'event */
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIG;
  sev.sigev_value.sival_ptr = &(tab[task_pid].timer);
  timer_create (CLOCK_REALTIME, &sev, &(tab[task_pid].timer));
}

/*
* Cette fonction demarre le timer du controleur d'echeance
* Doit être lancé 1 fois par le processus après le creation du controleur
*/
void
timer_start ()
{
  int nano = tab[ptask_idx].dead_line;
  /*Creation du timer */
  if (nano < 99999)
    {
      tab[ptask_idx].its.it_value.tv_sec = 0;
      tab[ptask_idx].its.it_value.tv_nsec = nano;
    }
  else
    {
      tab[ptask_idx].its.it_value.tv_sec = nano / 1000000000;
      tab[ptask_idx].its.it_value.tv_nsec = 0;
    }

  tab[ptask_idx].its.it_interval.tv_sec = tab[ptask_idx].its.it_value.tv_sec;
  tab[ptask_idx].its.it_interval.tv_nsec =
    tab[ptask_idx].its.it_value.tv_nsec;

  if (timer_settime (tab[ptask_idx].timer, 0, &(tab[ptask_idx].its), NULL) <
      0)
    {
      printf ("Erreur création timer\n");
      exit (EXIT_FAILURE);
    }
}
