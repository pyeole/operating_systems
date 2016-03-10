#include<stdio.h>
#include<sys/types.h>
#include<string.h>

void child_process();
void parent_process();

void main(void){
	pid_t pid;
	//int i;
	//char buf[100];
		
	/*fork();
	pid=getpid();
	for(i=1; i<=200; i++){
		sprintf(buf, "This line is from pid %d: value=%d\n",pid,i);
		write(1,buf,strlen(buf));
	}*/
	
	pid=fork();
	if(pid==0){
		child_process(pid);
	}
	else{
		parent_process(pid);
	}
}

void child_process(pid_t pid){
	for(int i=1; i<=10; i++){
		printf("Pid=%d, This line is from child, value=%d\n",pid,i);
	}
	printf("****Child process finished****");
}

void parent_process(pid_t pid){
	for(int i=1; i<=10; i++){
		printf("Pid=%d, This line is from parent, value=%d\n",pid,i);
	}
	printf("****Parent process finished****");
}
