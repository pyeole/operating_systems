/******************************************************************************
 *
 *  File Name........: structures.h
 *
 *  Description......: Declarations for library routines
 *
 *  Modified by Pratik Yeole on 02/03/2016
 *
 *  Revision History.:
 *
 *****************************************************************************/

//#ifndef MYTHREAD_H
//#define MYTHREAD_H
#include <mythread.h>
#include <ucontext.h>

typedef struct pratik{
	ucontext_t cntxt;
	int tid; 
	int parent_tid;
}pratik;

typedef struct list{
	pratik* context;
	struct list* next;
}node;
typedef struct semp{
	int value;
	node* front_element;
	node* rear_element;
}semp;

//#endif /* MYTHREAD_H */
/*........................ end of mythread.h ................................*/
