#include<stdio.h>
#include<pthread.h>

void *inc_x(void *arg_x){
	int *x=(int *)arg_x;
	while(++(*x) < 100);
	printf("x count finished!\n");
	return NULL;
}

int main(){
	int x=0, y=0;
	printf("x: %d and y: %d\n",x,y);	
	pthread_t new_thread;
	if(pthread_create(&new_thread,NULL,inc_x,&x)){
		fprintf(stderr,"Error in creating new thread!\n");
		return 1;
	}
	while (++y < 100);
	printf("y count finished!\n");

	if(pthread_join(new_thread,NULL)){
		fprintf(stderr,"Error joining two threads!\n");
		return 2;
	}

	printf("x: %d and y: %d\n",x,y);
	return 0;
}
