#include <cstring>
#include <iostream>

#include "fileManager.h"
#include "minisql.h"

using namespace std;

FileManager::FileManager(const char* FileName)
{
	filename = FileName;

	BufferManager* Manager = minisql::bufferManager;
	currentBlock = Manager->getCurrentBlock(filename.c_str(), 0);
	availableID = *(reinterpret_cast<int*>(currentBlock->content+FIXED_LENGTH));
	//fixedLength = 128;
}

FileManager::~FileManager()
{

}


void FileManager::createFile(const char* filename)
{
	//length += 2;

	FILE* file = fopen(("/Users/olive/c++ project/miniSQL/miniSQL/data/" + string(filename) + ".mdf").c_str(),"wb");

	char data[BLOCK_SIZE] = {0};
	//memcpy(data, content, 4);
	//memset(data + 4, 0, 4);
	int id = -1;
	memcpy(data + FIXED_LENGTH, &id, 4);
	fwrite(data, BLOCK_SIZE, 1, file);
	cout << (file == NULL) << endl;
	cout << filename << endl;
	fclose(file);
	

}

void FileManager::removeFile(const char* filename)
{

}

void FileManager::removeValue(int id)
{
	findCurrentBlock(id);

	
	memcpy(currentBlock->content + FIXED_LENGTH, &availableID, 4);

	char status = 'N';
	memcpy(currentBlock->content + FIXED_LENGTH - 1, &status,1);
	availableID = id;
	updateFirstBlock();
	//dirty;
}

void FileManager::addValue(const char* value)
{
	if (availableID >= 0) 
	{
		findCurrentBlock(availableID);
		availableID = *(reinterpret_cast<int*>(currentBlock->content+FIXED_LENGTH));
	}
	else 
	{
		findCurrentBlock(recordNum);
		recordNum++;
	}

	memcpy(currentBlock->content + offSet, value, DATA_LENGTH);
	char status = 'Y';
	memcpy(currentBlock->content + FIXED_LENGTH - 1, &status,1);

	updateFirstBlock();
	//dirty

}

void FileManager::updateFirstBlock()
{
	BufferManager* Manager = minisql::bufferManager;
	currentBlock = Manager->getCurrentBlock(filename.c_str(), 0);
	memcpy(currentBlock->content + FIXED_LENGTH, &availableID, 4);

	//dirty;
}

void FileManager::findCurrentBlock(int ID)
{
	BufferManager* Manager = minisql::bufferManager;
	currentBlock = Manager->getCurrentBlock(filename.c_str(), ID / recordOfEachBlock + 1);
	offSet = (availableID % recordOfEachBlock) * fixedLength;
}

void FileManager::removeValue(const char* value)
{

}

int FileManager::getNextValue(char* data)
{
	bool found = false;
	while (id < recordNum && !found)
	{
		findCurrentBlock(id);
		if (*(currentBlock->content + offSet + FIXED_LENGTH - 1) == 'Y') found = true;
	}

	memcpy(data, currentBlock->content + offSet, DATA_LENGTH);
	return id;
}





