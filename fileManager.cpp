#include <cstring>
#include <iostream>

#include "fileManager.h"
#include "minisql.h"

using namespace std;

FileManager::FileManager(const char* FILENAME)
{
	FileName = FILENAME;

	BufferManager* Manager = minisql::bufferManager;
	currentBlock = Manager->getCurrentBlock(FileName.c_str(), 0);
	availableID = *(reinterpret_cast<int*>(currentBlock->content+FIXED_LENGTH));
    recordNum = *(reinterpret_cast<int*>(currentBlock->content+DATA_LENGTH));
	fixedLength = 128;
    ids = -1;
    recordOfEachBlock = 10;
}

FileManager::~FileManager()
{

}


void FileManager::createFile(const char* filename)
{
	//length += 2;

	FILE* file = fopen(("/Users/olive/c++ project/MiniSQL Project/MiniSQL Project/tables/" + string(filename) + ".mdf").c_str(),"wb");

	char data[BLOCK_SIZE] = {0};
	//memcpy(data, content, 4);
	//memset(data + 4, 0, 4);
	int id = -1;
    memset(data + DATA_LENGTH, 0, 4);
	memset(data + FIXED_LENGTH, 0xFF, 4);
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

int FileManager::addValue(const char* value)
{
    int idToReturn;
	if (availableID >= 0) 
	{
		findCurrentBlock(availableID);
		availableID = *(reinterpret_cast<int*>(currentBlock->content+FIXED_LENGTH));
        idToReturn = availableID;
	}
	else 
	{
		findCurrentBlock(recordNum);
		recordNum++;
        idToReturn = recordNum;
	}

	memcpy(currentBlock->content + offSet, value, DATA_LENGTH);
	char status = 'Y';
	memcpy(currentBlock->content + FIXED_LENGTH - 1, &status,1);

	updateFirstBlock();
	//dirty
    return idToReturn;

}

void FileManager::updateFirstBlock()
{
	BufferManager* Manager = minisql::bufferManager;
	currentBlock = Manager->getCurrentBlock(FileName.c_str(), 0);
    memcpy(currentBlock->content + DATA_LENGTH, &recordNum, 4);
	memcpy(currentBlock->content + FIXED_LENGTH, &availableID, 4);

	//dirty;
}

void FileManager::findCurrentBlock(int ID)
{
	BufferManager* Manager = minisql::bufferManager;
	currentBlock = Manager->getCurrentBlock(FileName.c_str(), ID / recordOfEachBlock);
    ids = ID;
	offSet = (ids % recordOfEachBlock) * fixedLength;
}

void FileManager::removeValue(const char* value)
{

}

int FileManager::getNextValue(char* data)
{
	bool found = false;
//    while (ids < recordNum && !found)
//    {
//        findCurrentBlock(ids);
//        if (*(currentBlock->content + offSet + FIXED_LENGTH - 1) == 'Y') found = true;
//    }

	memcpy(data, currentBlock->content + offSet, DATA_LENGTH);
	return ids;
}





