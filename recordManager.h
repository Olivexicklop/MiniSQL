#ifndef _RECORD_MANAGER_H
#define _RECORD_MANAGER_H


#include <vector>
#include <string>
#include "constant.h"


class RecordManager
{
	//int fixedLength;

public:
	//RecordManager();
	//~RecordManager();
	

	bool createTable(const char* tableName);
	bool dropTable(const char* tableName);
	void insert(const char* tableName, const char* value);
	int select(const char* tableName);
	int Delete(const char* tableName);

	int select(const char* tableName, string column, string Operator, const char* operand);
	int filter(const char* tableName, const char* data, string column, string Operator, const char* operand);
	bool intCmp(const char* data, string Operator, const char* operand);
	int remove(const char* tableName);
};








#endif