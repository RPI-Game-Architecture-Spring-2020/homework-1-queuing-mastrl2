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
	
	//Starting head node
	node* h = new node();

	HEAD = h;
	TAIL = h;

	maxNodes = node_count;
}

ga_queue::~ga_queue()
{
	// TODO:
	// Free any resources held by the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	//Pops everything off the queue which destroys most of the queue
	void* info = 0;
	bool popped = true;
	while (popped == true) {
		popped = pop(&info);
	}
	//Lastly destroys the head node
	free(HEAD);
}

void ga_queue::push(void* data)
{
	// TODO:
	// Push 'data' onto the queue in a thread-safe manner.
	// If you preallocated 'node_count' elements, and if the queue is full when
	// this function is called, you must block until another thread pops an
	// element off the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	//Creates new node and adds value to it
	node* temp = new node();
	temp->value = data;

	T_LOCK.lock();

	//Resets where the tail points
	TAIL->pnt = temp;
	TAIL = temp;

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

	//Sets the first value node in the queue as the new head
	H_LOCK.lock();
	node* temp = HEAD;
	node* newHead = HEAD->pnt;
	if (newHead == nullptr) {
		H_LOCK.unlock();
		return false;
	}
	//Setting up the new head and setting data to the removed value
	*data = newHead->value;
	HEAD = newHead;
	H_LOCK.unlock();

	free(temp);

	return true;
}

int ga_queue::get_count() const
{
	//Counts up the number of nodes by checking if the next node is a nullpntr
	int count = 0;
	node* curr = HEAD;
	while (curr->pnt != nullptr) {
		count += 1;
		curr = curr->pnt;
	}
	return count;
}
