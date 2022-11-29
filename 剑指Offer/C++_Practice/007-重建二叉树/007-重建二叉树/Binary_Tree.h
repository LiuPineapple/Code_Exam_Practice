#pragma once
#include<iostream>
using namespace std;
#include<queue>

class BinaryTreeNode {
public:
	int m_nValue;
	BinaryTreeNode* m_pLeft;
	BinaryTreeNode* m_pRight;
};

//广度优先遍历即层序遍历打印
void PrintTree(BinaryTreeNode* root)
{
	if (root == nullptr)
	{
		throw std::exception("Invalid Input.\n");
	}
	queue<BinaryTreeNode *> qu;
	qu.push(root);
	while (!qu.empty())
	{
		BinaryTreeNode * node = qu.front();
		qu.pop();
		cout << node->m_nValue << ' ';
		if (node->m_pLeft != nullptr)//if(node->m_pLeft)也可
		{
			qu.push(node->m_pLeft);
		}
		if (node->m_pRight != nullptr)
		{
			qu.push(node->m_pRight);
		}
	}
	cout << endl;
}
void DestroyTree(BinaryTreeNode* root)
{
	delete root;
}