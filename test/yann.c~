#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ptask.h"

void hello(){
int r;
if(r=ptask_deadline_miss())
	printf("Deadline dépacé (%d)\n",r);
else{
	printf("hello\n");
	int i;
	for(i=0;i<5;i++){
		int u=(int)ptask_get_deadline(ptask_get_index(),SEC);
		printf("%d\n",u);
		u=ptask_deadline_miss();
		printf("%d\n",u);
		if(u)
			printf("Deadline dépacé (%d)\n",u);
		else{
			printf("hello fin period%d\n",i);
			ptask_wait_for_period();
		}
	}
}
}

int main(){

	printf("init task\n");
	ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

	/*Reglages des parametres de la tache*/
	tpars params = TASK_SPEC_DFL;
	params.period = tspec_from(4, SEC);
	ptask_param_deadline(params,2,SEC);
	params.priority = 1;
	params.act_flag = NOW;
	printf("creating task\n");

	int pid = ptask_create_param(hello,&params);
	if (pid < 0) {
		printf("Cannot create aperiodic task pid : %d\n",pid);
		exit(EXIT_FAILURE);
	}
	pthread_join(ptask_get_threadid(pid), 0);

	return 0;
}
