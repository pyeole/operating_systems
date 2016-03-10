/******************************************************************************
 *
 *  File Name........: queue.c
 *
 *  Description......: Queue operations for thread and semaphore routines
 *
 *  Modified by Pratik Yeole on 02/03/2016
 *
 *  Revision History.:
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mythread.h"
#include "structures.h"

//node* front_element=NULL;
//node* rear_element=NULL;

void push_back(pratik* n, node** front_element, node** rear_element){
	node* new_node=(node*)malloc(sizeof(node));
	new_node->context=n;
	new_node->next=NULL;
	if((*rear_element)==NULL){		//check for the rear of the queue
		(*rear_element)=new_node;
		(*front_element)=(*rear_element);
	}
	else{
		(*rear_element)->next=new_node;
		(*rear_element)=new_node;
	}
}

void pop_front(node** front_element, node** rear_element){
	if((*front_element)==NULL){
		return;
	}
	else if((*front_element)==(*rear_element)){
		node* temp=(*front_element);
		free(temp);
		(*front_element)=NULL;
		(*rear_element)=NULL;
		return;
	}	
	else{
		node* temp=(*front_element);
		(*front_element)=(*front_element)->next;
		free(temp);
		return;	
	}
}

pratik* front(node** front_element, node** rear_element){
	if((*front_element)==NULL){
		return NULL;
	}
	else{
		return (*front_element)->context;
	}
}	
