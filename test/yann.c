#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ptime.h"
#include "ptask.h"
#include "deadline_handler.h"

int pid;

void hello(){
	int i;
sleep(1);
	timer_start(pid,10);

	for(i=1;i<=100;i++){
		printf("hello\n");
		ptask_wait_for_period();
	}

}

int main(){

	printf("init task\n");
	ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

	/*Reglages des parametres de la tache*/
	tpars params = TASK_SPEC_DFL;
	params.period = tspec_from(1, SEC);
	ptask_param_deadline(params,2,SEC);
	params.priority = 1;
	params.act_flag = NOW;
	params.measure_flag = 1;
	printf("creating tasks\n");

	pid = ptask_create_param(hello,&params);
	if (pid < 0) {
		printf("Sudo ... : %d\n",pid);
		exit(EXIT_FAILURE);
	}
	create_deadline_handler(pid);
	pthread_join(ptask_get_threadid(pid), 0);
	return 0;
}
