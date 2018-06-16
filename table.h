#ifndef _TABLE_H_
#define _TABLE_H_

#include<vector>
#include<string>

#include"constant.h"

using namespace std;

class Table
{
private:
	string tableName;
	string primary;
	int recordLength;

	//col information
	int colNum;
	vector<string> colNameList;
	vector<short> colTypeList;
	vector<char> colUniqueList;
	vector<int> colStartPosList; // in one record

public:
	Table(const char* nameAndPrimary); //constructor

	const char* getTableName() const;
	const char* getPrimary() const;
	int getColNum();
	int getRecordLenth();

	const char* getColName(int id);
	int getId(const char* colName);
	short getType(const char* colName);
	char getUnique(const char* colName);

	//return type of column
	short getColValue(const char* colName, const char* recordIn, char* valueOut);

	//return -1 if valid, else return column id of not unique column
	int checkSameForUnique(const char* newRecord, const char* existRecord); 

	//return true if success
	bool recordToVec(const char* record, vector<char*>* vec);
	bool vecToRecord(const  vector<char*>* vec, char* record);
	


};

#endif
