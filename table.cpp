#include<cstdio>
#include<iostream>

#include"table.h"
#include"general.h"
#include"heapFile.h"

using namespace std;

// warning : create table_file before make an object of Table 
Table::Table(const char* nameAndPrimary)
{
	tableName = nameAndPrimary;
	primary = nameAndPrimary + MAX_NAME_LENGTH;


	//load column information from file -- colNum, name, type, unique
	HeapFile* colFile = new HeapFile(("catalog/table_" + string(tableName)).c_str());
	int i;

	int colNum = colFile->getRecordCount();
	char colInfo[MAX_NAME_LENGTH + 3]; //MAX_NAME_LENGTH for name, 2 for type, 1 for unique
	for (i = 0; i < colNum; i++) {
		colFile->getNextRecord(colInfo);

		char nameInfo[MAX_NAME_LENGTH];
		memcpy(nameInfo, colInfo, MAX_NAME_LENGTH);
		colNameList.push_back(nameInfo);

		char typeInfo[2];
		memcpy(typeInfo, colInfo + MAX_NAME_LENGTH, 2);
		colTypeList.push_back((short)*typeInfo);

		char uniqueInfo = colInfo[MAX_NAME_LENGTH + 2];
		colUniqueList.push_back(uniqueInfo);
	}

	delete colFile;


	//update column information -- startPos, recordLength
	colStartPosList.push_back(0);
	for (i = 1; i < colNum; i++) {
		colStartPosList.push_back(colStartPosList[i - 1] + General::getTypeSize(colTypeList[i - 1]));
	}

	recordLength = colStartPosList[colNum - 1] + General::getTypeSize(colTypeList[colNum - 1]);


}


const char* Table:: getTableName() const
{
	return tableName.c_str();
}

const char* Table::getPrimary() const
{
	return primary.c_str();
}

int Table::getColNum()
{
	return colNum;
}

int Table::getRecordLenth()
{
	return recordLength;
}

const char* Table::getColName(int id)
{
	if (id >= colNum) {
		cerr << "ERROR: [Table::getColName] Column id " << id << " is too large!" << endl;
		return NULL;
	}

	return colNameList[id].c_str();
}

int Table::getId(const char* colName)
{
	int id;
	for (id = 0; id < colNum; id++) {
		if (colNameList[id] == colName)
			return id;
	}
	return -1; //No such colName
}

short Table::getType(const char* colName)
{
	int id = getId(colName);

	if (id > 0) {
		return colTypeList[id];
	}

	cerr << "ERROR: [Table::getType] Table `" << tableName << "` has no column named `" << colName << "`!" << endl;
	return TYPE_NULL;
}

char Table::getUnique(const char* colName)
{
	int id = getId(colName);

	if (id > 0) {
		return colUniqueList[id];
	}

	cerr << "ERROR: [Table::getUnique] Table `" << tableName << "` has no column named `" << colName << "`!" << endl;
	return ERROR_UNIQUE;

}


//return type of column
short Table::getColValue(const char* colName, const char* recordIn, char* valueOut)
{
	int id = getId(colName);

	if (id < 0) {
		cerr << "ERROR: [Table::getColValue] Table `" << tableName << "` has no column named `" << colName << "`!" << endl;
		return TYPE_NULL;
	}

	memcpy(valueOut, recordIn + colStartPosList[id], General::getTypeSize(colTypeList[id]));
	return colTypeList[id];
}

//return -1 if valid, else return column id of not unique column
int Table:: checkSameForUnique(const char* newRecord, const char* existRecord)
{
	int id;
	int pos;

	for (id = 0; id < colNum; id++) {
		if (!colUniqueList[id]) continue;

		for (pos = colStartPosList[id]; pos < colStartPosList[id] + General::getTypeSize(colTypeList[id]); pos++) {
			if (newRecord[pos] != existRecord[pos]) break;
		}

		if (pos >= colStartPosList[id] + General::getTypeSize(colTypeList[id]))  return id;
		
	}

	return -1;
}


//return true if success
bool Table::recordToVec(const char* record, vector<char*>* vec)
{
	int id;
	int len;

	for (id = 0; id < colNum; id++) {
		len = General::getTypeSize(colTypeList[id]);
		char* value = new char[len];
		memcpy(value, record + colStartPosList[id], len);
		vec->push_back(value);
		delete[] value;
	}
	return true;
}

//return true if success
bool Table::vecToRecord(const  vector<char*>* vec, char* record)
{
	if ((int)vec->size() != colNum)
	{
		cerr << "ERROR: [Table::vecToRecord] Value number mismatch. Expecting " << colNum << " values, but found " << vec->size() << " values." << endl;
		return false;
	}

	int id;
	for (id = 0; id < colNum; id++) {
		char* value = General::getDataFromStr(vec->at(id), colTypeList[id]);
		int len = General::getTypeSize(colTypeList[id]);

		if (value == NULL) return false;
		
		memcpy(record + colStartPosList[id], value, len);
	}
}