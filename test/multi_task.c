#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ptime.h"
#include "ptask.h"
#include "deadline_handler.h"


#define NTASKS 5


int
fibonacci (int n)
{
  if (n == 0)
    return 0;
  else if (n == 1)
    return 1;
  else
    return (fibonacci (n - 1) + fibonacci (n - 2));
}

void
hello ()
{
  int i;
  sleep (2);
  timer_start ();
  for (i = 1; i <= 3; i++)
    {
      resume_timer ();
      printf ("	rslt : %d\n", fibonacci ((int) ptask_get_argument ()));
      pause_timer ();
      ptask_wait_for_period ();
    }
  delete_timer ();

}




int
main ()
{

  int pid[NTASKS];

  int i;
  printf ("init task\n");
  ptask_init (SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

  /*Reglages des parametres de la tache */
  tpars params = TASK_SPEC_DFL;
  params.period = tspec_from (4, SEC);
  ptask_param_deadline (params, 2, SEC);
  params.priority = 99;
  params.act_flag = NOW;
  params.measure_flag = 1;

  for (i = 0; i < NTASKS; i++)
    {

      params.arg = (void *) 10 + 5 * i;
      pid[i] = ptask_create_param (hello, &params);
      if (pid[i] < 0)
	{
	  printf ("Sudo ... : %d\n", pid[i]);
	  exit (EXIT_FAILURE);
	}
      create_deadline_handler (pid[i]);
    }

  for (i = 0; i < NTASKS; i++)
    {
      pthread_join (ptask_get_threadid (pid[i]), 0);
    }

  return 0;
}
