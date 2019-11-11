/* Fill in your Name and GNumber in the following two comment fields
 * Name: Rushil Nandan Dubey	
 * GNumber: G01203932
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"

/* Schedule Count
 * - Return the number of Processes in the list.
 */
int schedule_count(Process *list) {
	Process *walker=list;
	int count=0;
	//If the list is empty return 0.
	if(list==NULL){
		return 0;
	}	
	//Keep on iterating the lsit till it will encounter a NULL and keep on incrementing the count.
	while(walker!=NULL){
		count++;
		walker=walker->next;
	}
 
/* Complete this Function */
  return count;
}

/* Schedule Insert
 * - Insert the Process into the List with the Following Constraints
 *   list is a pointer to the head pointer for a singly linked list.
 *   It may begin as NULL
 *   1) Insert in the order of Ascending PIDs
 *   (eg. 1, 13, 14, 20, 32, 55)
 * Remember to set list to the new head if it changes!
 */
void schedule_insert(Process **list, Process *node) {
	Process *walker=*list;
	//When insertion is done and the list is empty.
	if(*list==NULL){
		*list=node;
		node->next= NULL;
	
	}
	
	//When insertion is done before the head and there is only one element in the list
	else if(node->pid < (*list)->pid){
		node->next=*list;
		*list=node;	//Head is updated gloabally
	}	
	else{	
		while(walker != NULL){
			//When insertion is done in between.
			if(walker->next != NULL && walker->next->pid > node->pid && node->pid > walker->pid){
				
				node->next = walker->next;
				walker->next = node;
			}
			//When insertion is done at the end.
			if(walker->next == NULL && node->pid >walker->pid){
				walker->next=node;
				node->next=NULL;
			}
			walker=walker->next;
		}
	}
	return;
  
}

/* Schedule Terminate
 * - Unallocate the Memory for the given Node
 */
void schedule_terminate(Process *node) {
	free(node); //Free the memory of the given node.
	return;
  
 
}

/* Schedule Generate
 * - Allocate the Memory and Create the Process from the given Variables
 *   Remember to initialize all values, including next!
 */
Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) {
	Process *new_node=malloc(sizeof(Process));
	//If the memory has not been allocated to the new_node return NULL.
	if(new_node == NULL){
		return NULL;
	}
	strcpy(new_node->name,name);
	new_node->pid=pid;
	new_node->time_remaining = time_remaining;
	new_node->time_last_run = time_last_run;
      	new_node->next=NULL;	
/* Complete this Function */
  return new_node;
}

/* Schedule Select
 * - Select the next Process to be run using the following conditions:
 *   1) The Process with the lowest time_remaining should run next.
 *   - If there are any ties, select the Process with the lowest PID
 *   2) If there is a Process that has not run in >= TIME_STARVATION, then
 *      select that one instead.
 *      (Use the function clock_get_time() to get the current time)
*   - If there are any ties, select the Process with the lowest PID
 *   3) If the list is NULL, return NULL
 * - Remove the selected Process from the list
 * - Return the selected Process
 */
Process *schedule_select(Process **list) {
	Process *walker=*list;
	Process *temp=malloc(sizeof(Process)); //Allocate memory to the temporary node.
	temp=*list;
	//if the list is NULL return 0
	if(*list==NULL){
		return 0;
	}
	//Iterate the walker till it will encouter a null.
	while(walker != NULL){
		//When wait time of a process is greater than the starvation time then selection is done on the basis of this
		if( (clock_get_time() - walker->time_last_run) >= TIME_STARVATION){
			//When more than one processes are there in the starvation select the one with lowest pid.
			if((clock_get_time() - temp->time_last_run) >= TIME_STARVATION){
				if(temp->pid > walker->pid)	
				temp=walker;
			}
			else{
				temp=walker;
			}

		}	
		//When no process is in the starvation state the selection is done on the basis of time remaining of the processes.
		if( (clock_get_time() - temp->time_last_run) < TIME_STARVATION && (clock_get_time() - walker->time_last_run) < TIME_STARVATION){
			//Select the process with lowest time remaining.
			if(walker->time_remaining < temp->time_remaining){
				temp=walker;
			}
			//If two or more processes have the same time remaining the selection is done on the bais of pid.
			if(walker->time_remaining == temp->time_remaining){
				//One with the lowest pid is selected.
				if(temp->pid > walker->pid){
				
					temp=walker;
				}
				
			}
		}
			//Increment the walker from one node to the other
			walker=walker->next;
	}
	//Again assigning the walker to the head of that list, in order to remove the selected node.
	walker=*list;
	//Iterarte the walker till will encounter a NULL.
	while(walker != NULL){
		//When the selected node is the head node then update the head globally to the next node. 
		if((*list)->next !=NULL && temp == walker){
			*list = (*list)->next;
			break;
		}
		//When there is only one node in the list then update the list to NULL.
		if((*list)->next == NULL && temp == walker){
			*list=NULL;
			break;
		}
		//When the selected node is somewhere in the middle of the node.
		if(temp->next !=NULL && walker->next == temp){

			walker->next=temp->next;
			break;
		}
		//When the selected node is the last node of the list.
		if(temp->next == NULL && walker->next == temp){
			walker->next=NULL;
			break;
		}
		//Increment the walker from one node to the other.
		walker=walker->next;
	}
	
	
//return the selcted node.
  return temp;
}	
