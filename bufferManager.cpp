#include "bufferManager.h"

using namespace std;

BufferManager::BufferManager()
{
	blockNum = 0;
	head = new BlockNode();
	tail = new BlockNode();
	head->pre = head->next = tail;
	tail->pre = tail->next = head;
}

BufferManager::~BufferManager()
{
	blockNum = 0;

	while (head->next != tail) removeBlock(head->next);
	delete head;
	delete tail;
}


void BufferManager::removeBlock(BlockNode* node)
{
	writeBlock(node->block->filename.c_str(), node->block->ID);
	filenameToBlock.erase(node->block->filename + "-" + to_string(node->block->ID));
	delete node;
	blockNum--;
}


void BufferManager::writeBlock(const char* filename, int ID)
{
	Block* block = filenameToBlock[string(filename) + "-" + to_string(ID)]->block;
	FILE* file = fopen(("data/" + string(filename) + ".mdf").c_str(),"rb+");
	fseek(file, ID*BLOCK_SIZE, SEEK_SET);
	fwrite(block->content, BLOCK_SIZE, 1, file);
	fclose(file);
}


Block* BufferManager::loadBlock(const char* filename, int ID)
{
	Block* block = new Block;
	FILE* file = fopen(("data/" + string(filename) + ".mdf").c_str(),"rb+");
	fseek(file, ID*BLOCK_SIZE, SEEK_SET);
	fread(block->content, BLOCK_SIZE, 1, file);
	fclose(file);

	BlockNode* node = new BlockNode;
	node->block = block;
	node->insert(head);
	blockNum++;
	filenameToBlock[string(filename) + "-" + to_string(ID)] = node;

	return block;
}

Block* BufferManager::getCurrentBlock(const char* filename, int ID)
{
	string blockName = string(filename) + "-" + to_string(ID);
	if (filenameToBlock.find(blockName) != filenameToBlock.end())
	{
		BlockNode* node = filenameToBlock[blockName];
		node->remove();
		node->insert(head);
		return node->block;
	}

	if (blockNum == MAX_BLOCK_NUM)
	{
		BlockNode* newBlock = new BlockNode();

		////
	}

	return loadBlock(filename, ID);
}
