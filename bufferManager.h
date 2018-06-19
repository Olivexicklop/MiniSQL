#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__

#include <string>
#include <unordered_map>
#include "constant.h"

using namespace std;

struct BlockNode
{
	Block* block;
	BlockNode* pre;
	BlockNode* next;

	void insert(BlockNode* node)
	{
		pre = node;
		next = node->next;
		node->next->pre = this;
		node->next = this;
	}

	void remove()
	{
		pre->next = next;
		next->pre = pre;
	}

	~BlockNode()
	{
		remove();
	}
};


class BufferManager
{
	int blockNum;
	BlockNode* head;
	BlockNode* tail;
	unordered_map<string, BlockNode*> filenameToBlock;

	Block* loadBlock(const char* filename, int ID);
	void writeBlock(const char* filename, int ID);
	void removeBlock(BlockNode* node);


public:
	BufferManager();
	~BufferManager();

	Block* getCurrentBlock(const char* filename, int ID);
};










































#endif