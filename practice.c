#include<stdio.h>
#include<sys/types.h>
#include<string.h>
void main(void){
	pid_t pid;
	int i;
	char buf[100];
		
	fork();
	pid=getpid();
	for(i=1; i<=10; i++){
		sprintf(buf, "This line is from pid %d: value=%d\n",pid,i);
		write(1,buf,strlen(buf));
	}
}
