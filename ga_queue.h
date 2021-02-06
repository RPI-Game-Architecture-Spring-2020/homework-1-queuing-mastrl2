#pragma once
#include <mutex>
/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

/*
** Thread-safe queue.
** https://www.research.ibm.com/people/m/michael/podc-1996.pdf
*/

class node
{
public:
	node() { pnt = nullptr; value = nullptr; }
	void changeNext(node* pnter) { pnt = pnter; }
	void* next() { return pnt; }


	node* pnt;
	void* value;
};


class ga_queue
{
public:
	ga_queue(int node_count);
	~ga_queue();

	void push(void* data);
	bool pop(void** data);

	int get_count() const;


	node* HEAD;
	node* TAIL;

	//if false, the lock is free
	std::mutex H_LOCK;
	std::mutex T_LOCK;
	int maxNodes;
};