/***************************************************************************************
 *
 *  File Name........: my_thread_library.c
 *
 *  Description......: Non-preemptive user-level thread library with semaphore support
 *
 *  Created by Pratik Yeole on 02/03/2016
 *
 *  Revision History.:
 *
 **************************************************************************************/

#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include "mythread.h"
#include "structures.h"

#define MEM 8192

typedef void *MyThread;
typedef void *MySemaphore;

node* ready_front=NULL;
node* ready_rear=NULL;

int count=1;

// ****** CALLS ONLY FOR UNIX PROCESS ****** 
// Create and run the "main" thread
void MyThreadInit(void(*start_funct)(void *), void *args){
	pratik* unx=(pratik*)malloc(sizeof(pratik));	//Unix process
	pratik* main=(pratik*)malloc(sizeof(pratik));	//Main thread
	getcontext(&(main->cntxt));
	(main->cntxt).uc_stack.ss_sp = malloc(MEM);
	(main->cntxt).uc_stack.ss_size = MEM;
	(main->cntxt).uc_link = &(unx->cntxt);
	makecontext(&(main->cntxt), (void(*)())start_funct, 1, args);	
	main->tid=count;
	main->parent_tid=0;
	push_back(main,&ready_front,&ready_rear);
	swapcontext(&(unx->cntxt), &(main->cntxt));	//start Main thread	
}

// ****** THREAD OPERATIONS ****** 
// Create a new thread.
MyThread MyThreadCreate(void(*start_funct)(void *), void *args){
	//printf("inside CREATE");
	pratik* new_thread=(pratik*)malloc(sizeof(pratik));	
	getcontext(&(new_thread->cntxt));			//initialize new thread
	pratik* temp1=front(&ready_front,&ready_rear);		
	new_thread->parent_tid=temp1->tid;			//assign parent thread ID to the new thread
	(new_thread->cntxt).uc_stack.ss_sp = malloc(MEM);
	(new_thread->cntxt).uc_stack.ss_size = MEM;
	(new_thread->cntxt).uc_link = NULL;
	makecontext(&(new_thread->cntxt), (void(*)())start_funct, 1, args);
	push_back(new_thread,&ready_front,&ready_rear);		//insert into the ready queue
	new_thread->tid= (++count);
	//printf("create done\n");
}

// Yield invoking thread
void MyThreadYield(void){
	//printf("inside YIELD");
	if(front(&ready_front,&ready_rear)==NULL){		//check if queue is empty
		return;
	}
	pratik* temp = front(&ready_front,&ready_rear);
	pop_front(&ready_front,&ready_rear);
	if(ready_front==NULL && ready_rear==NULL){		//check if queue becomes empty after pop
		push_back(temp,&ready_front,&ready_rear);
		return;
	}
	push_back(temp,&ready_front,&ready_rear);
	pratik* new_cntxt = front(&ready_front,&ready_rear);
	swapcontext(&(temp->cntxt), &(new_cntxt->cntxt));
	//printf("yield done\n");
}

// Join with a child thread
int MyThreadJoin(MyThread thread){
	pratik* t = front(&ready_front,&ready_rear);
	node* i=ready_front;
	pratik* temp1=(pratik*)thread;	
	while(i != NULL){			//traverse the ready queue
		pratik* temp2=i->context;
		if(temp2 == temp1){		//check whether the specified thread is present in ready queue
			if(t->tid == temp1->parent_tid){	//check if the specified thread is immediate child of the invoking thread
				pop_front(&ready_front,&ready_rear);
				push_back(t,&ready_front,&ready_rear);
				pratik* new_cntxt = front(&ready_front,&ready_rear);
				swapcontext(&(t->cntxt), &(new_cntxt->cntxt));
			}
			else
				return -1;
		}
		else
			i=i->next;
	}
	return 0;
}

// Join with all children
void MyThreadJoinAll(void){
	//printf("inside JOINALL");
	pratik* t = front(&ready_front,&ready_rear);
	node* i=ready_front;
	int flag=0;
	pratik *thread;
	while(1){				//traverse the ready queue until all the children have been visited
		while(i != NULL){
			thread=i->context;
			if(t->tid == thread->parent_tid){	
				flag=1;
				pop_front(&ready_front,&ready_rear);
				push_back(t,&ready_front,&ready_rear);
				pratik* new_cntxt = front(&ready_front,&ready_rear);
				swapcontext(&(t->cntxt), &(new_cntxt->cntxt));
			}
			if(flag==1){
				flag=0;
				i=ready_front->next;
			}
			else
				i=i->next;
		}
		if(flag==0){
			break;
		}
	}
}

// Terminate invoking thread
void MyThreadExit(void){
	//printf("inside EXIT\n");
	pratik* temp = front(&ready_front,&ready_rear);
	pop_front(&ready_front,&ready_rear);
	if(ready_front!=NULL){
		pratik* new_cntxt = front(&ready_front,&ready_rear);
		swapcontext(&(temp->cntxt), &(new_cntxt->cntxt));
	}
}

// ****** SEMAPHORE OPERATIONS ****** 
// Create a semaphore
MySemaphore MySemaphoreInit(int initialValue){
	semp* pratik_semaphore = (semp*)malloc(sizeof(semp));	
	pratik_semaphore->value=initialValue;
	return (MySemaphore*)pratik_semaphore;
}

// Signal a semaphore
void MySemaphoreSignal(MySemaphore sem){
	semp* my_sem=(semp*)sem;
	if((my_sem->front_element)==NULL){
		(my_sem->value)++;
	}
	else{
		pratik* temp=(my_sem->front_element)->context;
		push_back(temp,&ready_front,&ready_rear);
		pop_front(&(my_sem->front_element),&(my_sem->rear_element));
		(my_sem->value)++;
	}
}

// Wait on a semaphore
void MySemaphoreWait(MySemaphore sem){
	semp* my_sem=(semp*)sem;
	while(my_sem->value==0){
		pratik* temp1 = front(&ready_front,&ready_rear);
		pop_front(&ready_front,&ready_rear);
		push_back(temp1,&(my_sem->front_element),&(my_sem->rear_element));
		pratik* t = front(&ready_front,&ready_rear);
		swapcontext(&(temp1->cntxt), &(t->cntxt));
		my_sem=(semp*)sem;
		if(my_sem->value>0){
			break;
		}
	}
	if(my_sem->value>0){
		my_sem->value--;
	}
}

// Destroy on a semaphore
int MySemaphoreDestroy(MySemaphore sem){
	semp* my_sem=(semp*)sem;
	if(my_sem->front_element==NULL){
		return -1;
	}
	free(my_sem);
	return 0;
}
