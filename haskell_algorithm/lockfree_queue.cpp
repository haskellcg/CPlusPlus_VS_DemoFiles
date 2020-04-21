#include "stdafx.h"
#include "lockfree_queue.h"
#include <Windows.h>
#include <intrin.h>
#include <iostream>
using namespace std;

lockfree_queue::lockfree_queue():m_pHead(NULL), m_pTail(NULL)
{
	lockfree_node *pSentinel = new lockfree_node;
	m_pHead = m_pTail = pSentinel;
}

lockfree_queue::~lockfree_queue()
{
	lockfree_node *pNode = m_pHead->m_pNext;
	lockfree_node *pNext = NULL;
	while (NULL != pNode){
		pNext = pNode->m_pNext;
		delete pNode;
		pNode = pNext;
	}
	delete m_pHead;
	m_pHead = m_pTail = NULL;
}

bool lockfree_queue::empty()
{
	return (NULL == m_pHead->m_pNext);
}

void lockfree_queue::enqueue(const string &strData)
{
	lockfree_node *pNewNode = new lockfree_node;
	pNewNode->m_strData = strData;

	lockfree_node *pTailBk = m_pTail;
	lockfree_node *pTailCAS = pTailBk;
	do{
		while (NULL != pTailCAS->m_pNext){
			pTailCAS = pTailCAS->m_pNext;
		}
		if ((lockfree_node *)InterlockedCompareExchangePointer((PVOID *)&(pTailCAS->m_pNext), (PVOID)pNewNode, (PVOID)NULL) != pTailCAS->m_pNext){
			InterlockedCompareExchangePointer((PVOID *)&m_pTail, (PVOID)pNewNode, (PVOID)pTailBk);
			break;
		}
	}while (true);
}

string lockfree_queue::dequeue()
{
	lockfree_node *pHeadBk = m_pHead;
	lockfree_node *pHeadNext = pHeadBk->m_pNext;
	do{
		if (NULL != pHeadNext){
			if ((lockfree_node *)InterlockedCompareExchangePointer((PVOID *)&m_pHead, (PVOID)pHeadNext, (PVOID)pHeadBk) != m_pHead){
				pHeadBk->m_pNext = NULL;
				delete pHeadBk;
				break;
			}
		} else {
			return string();
		}
	}while (true);
	return m_pHead->m_strData;
}

lockfree_queue g_queue;

DWORD WINAPI pruducer(LPVOID param)
{
	char *pData = (char *)param;
	for (int i = 0; i < 500; ++i){
		g_queue.enqueue(pData);
		//Sleep(10);
	}
	return 0;
}

DWORD WINAPI comsumer(LPVOID param)
{
	Sleep(3000);
	while (true){
		while (!g_queue.empty()){
			cout << g_queue.dequeue() << "\t";
		}
		//Sleep(200);
		//cout << ".";
	}
}

void lockfree_queue_test()
{
	//lockfree_queue myqueue;
	//for (int i = 0; i < 10; ++i){
	//	myqueue.enqueue("AB");
	//}

	//while (!myqueue.empty()){
	//	cout << myqueue.dequeue() << endl;
	//}

	char *pData_1 = "A";
	char *pData_2 = "B";
	char *pData_3 = "C";
	HANDLE handleProducer_1 = CreateThread(NULL, 0, &pruducer, pData_1, 0, NULL);
	HANDLE handleProducer_2 = CreateThread(NULL, 0, &pruducer, pData_2, 0, NULL);
	HANDLE handleProducer_3 = CreateThread(NULL, 0, &pruducer, pData_3, 0, NULL);
	HANDLE handleConsumer_1 = CreateThread(NULL, 0, &comsumer, NULL, 0, NULL);
	HANDLE handleConsumer_2 = CreateThread(NULL, 0, &comsumer, NULL, 0, NULL);

	WaitForSingleObject(handleConsumer_1, INFINITE);
}