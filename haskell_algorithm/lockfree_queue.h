#ifndef __LOCKFREE_QUEUE_H__
#define __LOCKFREE_QUEUE_H__

#include <string>
using namespace std;

struct lockfree_node
{
	string m_strData;
	lockfree_node *m_pNext;
	lockfree_node():m_pNext(NULL)
	{}
};

struct lockfree_queue
{
	lockfree_node *m_pHead;
	lockfree_node *m_pTail;

	lockfree_queue();
	~lockfree_queue();

	bool empty();
	string dequeue();
	void enqueue(const string &strData);
};

void lockfree_queue_test();

#endif //__LOCKFREE_QUEUE_H__