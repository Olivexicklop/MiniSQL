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
}

void BufferManager::removeBlock(BlockNode* node)
{
	writeBlock(node->block->filename.c_str(), node->block->ID);
	filenameToBlock.erase(node->block->filename + "-" + to_string(node->block->ID));
	delete node;
	blockNum--;
}


Block* BufferManager::loadBlock(const char* filename, int ID)
{
	Block* block = new Block;
	FILE* file = fopen(("data/" + string(filename) + ".mdf").c_str(),"rb+");
	fseek(file, ID*BLOCK_SIZE, SEEK_SET);
	fread(block->content, BLOCK_SIZE, 1, file);
	fclose(file);

	BlockNode* node = new BlockNode;
	node->insert(head);
	blockNum++;
	filenameToBlock[string(filename) + "-" + to_string(ID)] = node;

	return block;
}

void BufferManager::writeBlock(const char* filename, int ID)
{
	Block* block = filenameToBlock[string(filename) + "-" + to_string(ID)]->block;
	FILE* file = fopen(("data/" + string(filename) + ".mdf").c_str(),"rb+");
	fseek(file, ID*BLOCK_SIZE, SEEK_SET);
	fwrite(block->content, BLOCK_SIZE, 1, file);
	fclose(file);
}