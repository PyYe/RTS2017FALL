#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
/*struct arg_type{
	int pth;
	int sche;
}*/
pthread_barrier_t barrier;
void* func(void* data){
	int s = 0;
	int *noptr = ((void**)data)[0];//t1 t2
	int *schptr = ((void**)data)[1];//schedule
	if (*schptr == 1){
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(0, &mask);
		s = sched_setaffinity(0, sizeof(mask), &mask);
		
		struct sched_param param;
		param.sched_priority = sched_get_priority_max(SCHED_FIFO);
		s = sched_setscheduler(0, SCHED_FIFO, &param);
	}
	
	pthread_barrier_wait(&barrier);
	int j ,i;
	for ( j = 0; j < 4; j++) {
		printf("Thread %d is running\n", *noptr);
		// busy 1 second
		double a = 0;
		for (i = 0; i < 10000000; i++) {
			a += 0.1f;
		}
	}
	return (void *) 0;
	
}




int main(int argc,char *argv[]){
	//struct arg_type formain;
	int schedule = 0;
	if (argc >1){
		if (strcmp(argv[1],"SCHED_FIFO") == 0) {
			schedule = 1;
			//printf ("%d\n",schedule);
		}
		
	}
	
	pthread_t pid_t1; 
	pthread_t pid_t2;
	
	int error_t1 , error_t2;
	
	pthread_barrier_init(&barrier, NULL, 2);
	
	int no = 1;
	void* arg[2] = {&no,&schedule};
	//formain.pth = 1; 
	error_t1 = pthread_create(&pid_t1,NULL,func,arg);
	if (error_t1!=0)  printf("pthread1_create error.\n");
	else printf("Thread %d was created.\n",no);
	
	int no2 = 2;
	void* arg2[2] = {&no2,&schedule};
	//formain.pth = 2;
	error_t2 = pthread_create(&pid_t2,NULL,func,arg2);
	if (error_t2!=0)  printf("pthread2_create error.\n");
	else printf("Thread %d was created.\n",no2);
	
	pthread_join(pid_t1,NULL);
	pthread_join(pid_t2,NULL);
	
	return 0;
}
