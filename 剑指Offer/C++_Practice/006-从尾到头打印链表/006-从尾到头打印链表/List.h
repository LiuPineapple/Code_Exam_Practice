#pragma once
#include<iostream>
using namespace std;
class ListNode
{
public:
	int m_nValue;
	ListNode * m_pNext;
	ListNode(int value)
	{
		m_nValue = value;
		m_pNext = NULL;
	}
};

void PrintList(ListNode* pHead)
{
	ListNode* pNode = pHead;
	while (pNode !=nullptr)
	{
		printf("%d\t", pNode->m_nValue);
		pNode = pNode->m_pNext;
	}
	cout << endl;
}

ListNode * CreateListNode(int value)
{
	return new ListNode(value);
}
void ConnectListNodes(ListNode* pnode1, ListNode* pnode2)
{
	pnode1->m_pNext = pnode2;
}

void DestroyList(ListNode* pNode)
{
	//ListNode* pnode = pNode->m_pNext;
	//while (pnode != nullptr)
	//{
	//	ListNode* pnode = pnode->m_pNext;
	//	delete &pNode;
	//}
	pNode->m_pNext = NULL;
}