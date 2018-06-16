#include <cstring>
#include "constant.h"
#include "fileManager.h"

using namespace std;

FileManager::FileManager(const char* FileName)
{
	filename = FileName;
	fixedLength = 128;
	availableID = 0;

}

FileManager::~FileManager()
{

}


void FileManager::createFile(const char* filename)
{
	//length += 2;

	FILE* file = fopen(("data/" + string(filename) + ".mdf").c_str(),"wb");

	char data[BLOCK_SIZE] = {0};
	//memcpy(data, content, 4);
	//memset(data + 4, 0, 4);
	//memset(data + 8, -1, 4);
	fwrite(data, BLOCK_SIZE, 1, file);
	fclose(file);
}

void FileManager::removeFile(const char* filename)
{

}

void FileManager::addValue(const char* value)
{
	offSet = availableID * fixedLength;
	memcpy(currentBlock->content + offSet, value, fixedLength);

	recordNum++;
}



