#include<string>
#include<iostream>

#include"BTNode.h"
#include"bufferManager.h"

using namespace std;

// load from file
BTNode::BTNode(const char* fileName, int _blockId, int _keyLen)
{
	/********************************
	waiting for api\bufferManager
	********************************/
	BTreeFile = fileName;
	blockId = _blockId;
	keyLen = _keyLen;

	BufferManager* buffer; //wating
	Block* block = buffer->loadBlock(fileName, _blockId);
	char* nodeInfo = block->content;
	
	char* temp = new char[4];
	memcpy(temp, nodeInfo, 4);
	keyNum = (int)temp;

	keys.push_back(NULL);

	memcpy(temp, nodeInfo + 4, 4);
	idPtrs.push_back((int)temp);
	delete[] temp;

	int dist = 8;
	for (int i = 1; i <= keyNum; i++) {
		char* key = new char[keyLen];
		memcpy(key, nodeInfo + dist, keyLen);
		keys.push_back(key);
		delete[] key;
		dist += keyLen;
		
		char* ptr = new char[4];
		memcpy(ptr, nodeInfo + dist, 4);
		idPtrs.push_back((int)ptr);

		dist += 4;

		
	}

	deleted = dir = false;
	leaf = idPtrs[0] < 0 ;

}

//empty node
BTNode::BTNode(const char* fileName, int _blockId, int _keyLen, bool _leaf, int firstIdPtr)
{
	BTreeFile = fileName;
	blockId = _blockId;
	keyLen = _keyLen;

	keys.push_back(NULL);
	idPtrs.push_back(firstIdPtr);

	keyNum = 0;
	leaf = _leaf;
	dir = true;
	deleted = false;
}

BTNode::~BTNode()   //waiting
{
	if (dir && !deleted) { //save tp file
		BufferManager* buffer;
		Block* block = buffer->loadBlock(BTreeFile.c_str, blockId);
		char* nodeInfo = block->content;

		char* temp = new char[4];
		memcpy(nodeInfo, temp, 4);

		
	}
}

int BTNode::getKeyNum()
{
	return keyNum;
}

int BTNode::isLeaf()
{
	return leaf;
}

