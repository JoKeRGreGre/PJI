#ifndef __DEADLINE_HANDLER_H__
#define __DEADLINE_HANDLER_H__

#include <time.h>
#include <signal.h>


typedef struct str_time_deadline
{
  unsigned long dead_line;	/*relativ deadline of the task */
  unsigned long temps_exec;	/*execution time of the timer */
  timer_t timer;		/*timer */
  struct itimerspec its;	/*structure du timer */
  int pid;			/*pid du processus */
} str_time_deadline;


/**
*	Creation d'un timer qui verifie le temps d'exection d'une tache toutes les p milisecondes si le *	prochain temps d'execution de la tache devrais dépasser l'echeance, un signal est envoyé pour 
*	tuer le processus.
*
*	task_pid : pid du processus
*	milisec : tic du timer (miliseconde)
*	dl : echance (miliseconde)
*/
void create_deadline_handler (int pid);

/**
* 	Demarre le timer du pid p avec un tic de timer toutes les m milisecondes
*/
void timer_start (int m);
void reset_timer ();
void delete_timer();


#endif
