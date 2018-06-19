#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include <string>
#include "constant.h"

class FileManager
{
	string filename;

	Block* currentBlock;

	int fixedLength;
	int offSet;
	int availableID;

	int recordNum;
	int recordOfEachBlock;

	int id;

public:
	FileManager(const char* FileName);
	~FileManager();


	static void createFile(const char* filename);
	static void removeFile(const char* filename);
	void addValue(const char* value);
	void removeValue(int id);
	void updateFirstBlock();
	void findCurrentBlock(int ID);
	void removeValue(const char* value);
	int getNextValue(char* data);
};






















#endif