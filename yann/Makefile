###------------------------------
### Main rules 
###------------------------------------------------------------
main :task_test.o
	gcc -Wall -ansi -m32 -pedantic -g -o main task_test.o
task_test.o :task_test.c deadline_handler.c
	gcc -Wall -ansi -m32 -pedantic -g -c task_test.c deadline_handler.c 
###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o 
realclean: clean 
	$(RM) main
