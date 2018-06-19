#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define BLOCK_SIZE 4096
#define MAX_BLOCK_NUM 100
#define FIXED_LENGTH 124
#define DATA_LENGTH 114

#define EQUAL "="
#define GREATER ">"
#define LESS "<"
#define EQUAL_GREATER ">="
#define EQUAL_LESS "<="

#include <string>

using namespace std;


struct Block
{
	string filename;
	int ID;

	bool dir;
	bool pin;

	char content[BLOCK_SIZE];
};

// struct availableIDNode
// {
// 	int firstAvailableID;

// 	struct availableIDList* next;
// 	struct availableIDList* pre;

// 	void insert(availableIDNode* node)
// 	{
// 		pre = node;
// 		next = node->next;
// 		node->next->pre = this;
// 		node->next = this;
// 	}

// 	void remove()
// 	{
// 		pre->next = next;
// 		next->pre = pre;
// 	}

// 	~availableIDNode()
// 	{
// 		remove();
// 	}
// };










#endif