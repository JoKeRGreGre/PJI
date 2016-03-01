#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "ptime.h"
#include "ptask.h"

struct str_time_deadline{
unsigned long dead_line;	/*relativ deadline of the task*/
unsigned long temps_exec;	/*execution time of the timer*/ 
timer_t timer;			/*timer*/
struct itimerspec its;		/*structure du timer*/
int pid;			/*pid du processus*/
};


/**
*	Creation d'un timer qui verifie le temps d'exection d'une tache toutes les p milisecondes si le *	prochain temps d'execution de la tache devrais dépasser l'echeance, un signal est envoyé pour 
*	tuer le processus.
*
*	task_pid : pid du processus
*	milisec : tic du timer (miliseconde)
*	dl : echance (miliseconde)
*/
void create_deadline_handler(int pid);

/**
* 	Demarre le timer
*/
void timer_start(int milisec);
