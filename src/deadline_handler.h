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
*	Creation d'un timer handler, du timer associé. Le timer doit être lancé avec timer_start().
*
*	task_pid : pid du processus
*/
void create_deadline_handler (int pid);

/**
* 	Demarre le timer du pid p avec un tic de timer sur la deadline
*/
void timer_start ();


//void reset_timer ();

/**
*	Met en pause le timer en stoppant l'envoi de signal, à utiliser après l'execution de la ou les fonctions du thread, juste avant d'attendre la fin de periode,
cette fonction doit être appelé pour eviter que le timer tourne encore pendant l'attente active de la fin de periode du thread.
*/
void pause_timer ();

/**
*	Remet en route le timer en reinitialisant les parametres initiaux.
*/
void resume_timer ();

/**
*	Coupe l'execution du timer. 
*/
void delete_timer ();


#endif
