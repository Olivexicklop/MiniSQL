#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include <string>


class FileManager
{
	string filename;

	Block* currentBlock;

	int fixedLength;
	int offSet;
	int availableID;

	int recordNum;

public:
	FileManager(const char* FileName);
	~FileManager();


	static void createFile(const char* filename);
	static void removeFile(const char* filename);
	void addValue(const char* value);
};






















#endif