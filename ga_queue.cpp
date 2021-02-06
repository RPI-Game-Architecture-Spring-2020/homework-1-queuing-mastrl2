/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_queue.h"
#include <mutex>

ga_queue::ga_queue(int node_count)
{
	// TODO:
	// Initialize the queue.
	// For extra credit, preallocate 'node_count' elements (instead of
	// allocating on push).
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	/*node = new node() # Allocate a free node
	node– > next.ptr = NULL # Make it the only node in the linked list
	Q– > Head = Q– > Tail = node # Both Head and Tail point to it
	Q– > H lock = Q– > T lock = FREE # Locks are initially free*/
	
	node* h = new node();

	HEAD = new node();
	TAIL = new node();

	HEAD->pnt = h;
	TAIL->pnt = h;

	maxNodes = node_count;
}

ga_queue::~ga_queue()
{
	// TODO:
	// Free any resources held by the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	void* info = 0;
	bool popped = true;
	while (popped == true) {
		popped = pop(&info);
	}
	free(HEAD->pnt);
	free(HEAD);
	free(TAIL);
}

void ga_queue::push(void* data)
{
	// TODO:
	// Push 'data' onto the queue in a thread-safe manner.
	// If you preallocated 'node_count' elements, and if the queue is full when
	// this function is called, you must block until another thread pops an
	// element off the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	/*
	* node = new node() # Allocate a new node from the free list
	node–>value = value # Copy enqueued value into node
	node–>next.ptr = NULL # Set next pointer of node to NULL
	lock(&Q–>T lock) # Acquire T lock in order to access Tail
	Q–>Tail–>next = node # Link node at the end of the linked list
	Q–>Tail = node # Swing Tail to node
	unlock(&Q–>T lock) # Release T lock
	*/

	node* temp = new node();
	temp->value = data;
	node* last = TAIL->pnt;

	T_LOCK.lock();

	last->pnt = temp;
	TAIL->pnt = temp;

	T_LOCK.unlock();
}

bool ga_queue::pop(void** data)
{
	// TODO:
	// Pop one element off the queue in a thread-safe manner and place it in
	// the memory pointed to by 'data'.
	// If the queue is empty when this function is called, return false.
	// Otherwise return true.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	/*
	* lock(&Q–>H lock) # Acquire H lock in order to access Head
	node = Q–>Head # Read Head
	new head = node–>next # Read next pointer
	if new head == NULL # Is queue empty?
	unlock(&Q–>H lock) # Release H lock before return
	return FALSE # Queue was empty
	endif
	*pvalue = new head–>value # Queue not empty. Read value before release
	Q–>Head = new head # Swing Head to next node
	unlock(&Q–>H lock) # Release H lock
	free(node) # Free node
	return TRUE # Queue was not empty, dequeue succeeded
	*/

	H_LOCK.lock();
	node* temp = HEAD->pnt->pnt;
	//node* newHead = HEAD->pnt->pnt->pnt;
	if (temp == nullptr) {
		H_LOCK.unlock();
		return false;
	}
	*data = temp->value;
	//void* val = temp->data;
	HEAD->pnt->pnt = temp->pnt;
	H_LOCK.unlock();
	//void * value 
	free(temp);

	return true;
}

int ga_queue::get_count() const
{
	// TODO:
	// Get the number of elements currently in the queue.
	int count = 0;
	//h = HEAD;
	node* curr = HEAD->pnt;
	while (curr->pnt != nullptr) {
		count += 1;
		//node* next = curr->pnt;
		curr = curr->pnt;
	}
	

	return count;
}
