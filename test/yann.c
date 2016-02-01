#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ptask.h"

void hello(){
	int r;
	if(r=ptask_deadline_miss())
		printf("Deadline dépacé (%d)\n",r);
	else{
		int i;
		for(i=1;i<=5;i++){
			int a=(int)ptask_get_deadline(ptask_get_index(),SEC);
			int b=(int)ptask_get_period(0,SEC);
			int c=(int)ptask_deadline_miss();
			printf("temps de la dead_line : %d sec\n",a);
			printf("temps de la period : %d sec\n",b);
			printf("dead_line dépacé ? : %d\n",c);
			ptask_set_deadline(0,a-1,SEC);
			/* echeance = i*periode ? */
			if(a<b*i){
				printf("Deadline dépacé ha ha !\n");
				return;
			}
			/* echeance = periode ?*/
			else if(a<b){
				printf("Deadline dépacé ha !\n");
				return;
			}
			else if(c){
				printf("Deadline dépacé\n");
				return;
			}
			else{
				printf("**********fin period%d********\n",i);
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
	params.period = tspec_from(2, SEC);
	ptask_param_deadline(params,4,SEC);
	params.priority = 1;
	params.act_flag = NOW;
	params.measure_flag = 1;
	printf("creating task\n");

	int pid = ptask_create_param(hello,&params);
	if (pid < 0) {
		printf("Cannot create aperiodic task pid : %d\n",pid);
		exit(EXIT_FAILURE);
	}
	pthread_join(ptask_get_threadid(pid), 0);

	return 0;
}
