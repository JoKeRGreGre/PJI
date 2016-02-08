#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ptask.h"
#include "deadline_handler.h"

void hello(){
	int i;
	timer_start();
	for(i=1;i<=10;i++){
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
	ptask_param_deadline(params,5,SEC);
	params.priority = 1;
	params.act_flag = NOW;
	params.measure_flag = 1;
	printf("creating task\n");

	int pid = ptask_create_param(hello,&params);
	create_deadline_handler(pid,10,5000);
	if (pid < 0) {
		printf("Sudo ... : %d\n",pid);
		exit(EXIT_FAILURE);
	}
	pthread_join(ptask_get_threadid(pid), 0);

	return 0;
}
