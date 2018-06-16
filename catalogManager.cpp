#include<iostream>
#include<cstring>
#include <unordered_set>

#include "catalogManager.h"
#include"constant.h"
#include"general.h"

CatalogManager::CatalogManager()
{
	//open file
	tablesFile = new HeapFile("catalog/tables");
	indicesFile = new HeapFile("catalog/indices");

	//load file information to map
	char tableInfo[MAX_NAME_LENGTH * 2];
	char indexInfo[MAX_NAME_LENGTH * 3];

	int id;
	while((id = tablesFile->getNextRecord(tableInfo)) >= 0){
		Table* table = new Table(tableInfo);
		tableMap[table->getTableName()] = table;
		tableIdMap[table->getTableName()] = id;
	}

	while ((id = indicesFile->getNextRecord(indexInfo)) >= 0) {
		Index* index = new Index(indexInfo);
		indexMap[index->getIndexName] = index;
		indexIdMap[index->getIndexName()] = id;
	}

}

CatalogManager::~CatalogManager()
{
	for (auto table : tableMap)
		delete table.second;
	for (auto index : indexMap)
		delete index.second;

	
	delete tablesFile;
	delete indicesFile;
}

Table* CatalogManager::getTable(const char* tableName) const
{
	if (tableMap.find(tableName) == tableMap.end())
	{
		cerr << "ERROR: [CatalogManager::getTable] Table `" << tableName << "` does not exist!" << endl;
		return NULL;
	}
	 
	return tableMap.at(tableName);
}

bool CatalogManager:: createTable(
	const char* tableName, const char* primary,
	const vector<string>* colName, const vector<short>* colType, vector<char>* colUnique
)
{
	//checks
	if (tableMap.find(tableName) != tableMap.end()) {
		cerr << "ERROR: [CatalogManager::createTable] Table `" << tableName << "` already exists!" << endl;
		return false;
	}

	//make sure every column is different named
	unordered_set<string> colNameSet;
	for (auto eachName : *colName)
	{
		if (colNameSet.find(eachName) != colNameSet.end())
		{
			cerr << "ERROR: [CatalogManager::createTable] Duplicate column name `" << eachName << "`!" << endl;
			return false;
		}
		colNameSet.insert(eachName);
	}


	// set primary key to unique
	int colNum = colName->size();
	int i;
	for (i = 0; i < colNum; i++) {
		if (strcmp(primary, (colName->at(i)).c_str()) == 0) {
			colUnique->at(i) = UNIQUE;
			break;
		}
	}
	if (i >= colNum) { // primary not in colName
		cerr << "ERROR: [CatalogManager::createTable] Cannot find primary key name `" << primary << "` in column names!" << endl;
		return false;
	}

	//All checks are done, now begin create
	//write to tablesFile
	char tableInfo[MAX_NAME_LENGTH * 2];
	memcpy(tableInfo, tableName, MAX_NAME_LENGTH);
	memcpy(tableInfo + MAX_NAME_LENGTH, primary, MAX_NAME_LENGTH);
	int id = tablesFile->addRecord(tableInfo);


	//create file to store column information before create Table
	HeapFile::createFile(("catalog/table_" + string(tableName)).c_str(), MAX_NAME_LENGTH + 3);
	HeapFile* colFile = new HeapFile(("catalog/table_" + string(tableName)).c_str());

	for (i = 0; i < colNum; i++) {
		char col[MAX_NAME_LENGTH + 3];
		memcpy(col, (colName->at(i)).c_str, MAX_NAME_LENGTH);
		memcpy(col + MAX_NAME_LENGTH, &(colType->at(i)), 2);
		memcpy(col + MAX_NAME_LENGTH + 2, &(colUnique->at(i)), 1);
		tablesFile->addRecord(col);
	}


	//record table to map
	tableIdMap[tableName] = id;
	tableMap[tableName] = new Table(tableInfo);

	return true;

}


// include dropping related indices
bool CatalogManager::dropTable(const char* tableName)
{
	if (tableMap.find(tableName) == tableMap.end())
	{
		cerr << "ERROR: [CatalogManager::dropTable] Table `" << tableName << "` does not exist!" << endl;
		return false;
	}

	//drop all index
	vector<Index*>* indexVec;
	getIndexByTableName(tableName, indexVec);
	for (int i = 0; i < indexVec->size(); i++) {
		dropIndex(indexVec->at(i)->getIndexName());
	}

	//drop column information file
	General::dropFile(("catalog/table_" + string(tableName)).c_str());

	//delete table
	delete tableMap[tableName];

	//delete record from tablesFile
	tablesFile->deleteRecord(tableIdMap[tableName]);

	//delete pairs from table map
	unordered_map<string, Table*>::iterator iter1 = tableMap.find(tableName);
	if (iter1 != tableMap.end())
	{
		tableMap.erase(iter1);
	}

	unordered_map<string, int>::iterator iter2 = tableIdMap.find(tableName);
	if (iter2 != tableIdMap.end())
	{
		tableIdMap.erase(iter2);
	}

	return true;
}


Index* CatalogManager::getIndex(const char* indexName) const
{
	if (indexMap.find(indexName) == indexMap.end())
	{
		cerr << "ERROR: [CatalogManager::getIndex] Index `" << indexName << "` does not exist!" << endl;
		return NULL;
	}

	return indexMap.at(indexName);
}


Index* CatalogManager::getIndexByTableCol(const char* tableName, const char* colName)
{
	unordered_map<string, Index*>::iterator iter;
	for (iter = indexMap.begin(); iter != indexMap.end(); iter++) {
		if (strcmp(iter->second->getTableName, tableName) == 0 && strcmp(iter->second->getColName, colName) == 0) {
			return iter->second;
		}
	}

	cerr << "ERROR: [CatalogManager::getIndexByTableCol] Index with table `" << tableName << "` and column `" << colName << "` does not exist!" << endl;
	return NULL;
}


bool CatalogManager::createIndex(const char* indexName, const char* tableName, const char* colName)
{
	//check : if indexName exists already
	if (indexMap.find(indexName) != indexMap.end())
	{
		cerr << "ERROR: [CatalogManager::createIndex] Index `" << indexName << "` already exists!" << endl;
		return false;
	}

	//check : table exists?
	if (tableMap.find(tableName) == tableMap.end())
	{
		cerr << "ERROR: [CatalogManager::createIndex] Table `" << tableName << "` does not exist!" << endl;
		return false;
	}

	//check : column exists?
	Table* table = tableMap[tableName];
	int i;
	for (i = 0; i < table->getColNum(); i++) {
		if (strcmp(table->getColName(i), tableName) == 0) break;
	}

	if (i >= table->getColNum()) {
		cerr << "ERROR: [CatalogManager::createIndex] Table `" << tableName << "` does not have column named `"<< colName << "` !" << endl;
		return false;
	}

	//check : unique? already has index?
	if (table->getUnique(colName) != UNIQUE) {
		cerr << "ERROR: [CatalogManager::createIndex] Column `" << colName << "` is not unique"<< "` !" << endl;
		return false;
	}

	Index* index;
	if (( index = getIndexByTableCol(tableName, colName)) != NULL) {
		cerr << "ERROR: [CatalogManager::createIndex] Column `" << colName << "` already has a index named `"<< index->getIndexName << "` !" << endl;
		return false;
	}


	//All checks done. Now begin create
	//indicesFile & map
	char indexInfo[MAX_NAME_LENGTH * 3];
	memcpy(indexInfo, indexName, MAX_NAME_LENGTH);
	memcpy(indexInfo + MAX_NAME_LENGTH, tableName, MAX_NAME_LENGTH);
	memcpy(index + MAX_NAME_LENGTH * 2, colName, MAX_NAME_LENGTH);
	int id = indicesFile->addRecord(indexInfo);

	Index* newIndex = new Index(indexInfo);
	indexMap[indexName] = newIndex;
	indexIdMap[indexName] = id;

	return true;
}


bool CatalogManager::dropIndex(const char* indexName)
{
	if (indexMap.find(indexName) == indexMap.end())
	{
		cerr << "ERROR: [CatalogManager::dropIndex] Index `" << indexName << "` dose not exist!" << endl;
		return false;
	}

	delete indexMap[indexName];
	
	int id = indexIdMap[indexName];
	indicesFile->deleteRecord(id);

	indexIdMap.erase(indexName);
	indexMap.erase(indexName);

	return true;
}


void CatalogManager:: getIndexByTableName(const char* tableName, vector<Index*>* vec)
{
	unordered_map<string, Index*>::iterator iter;
	for (iter = indexMap.begin(); iter != indexMap.end(); iter++) {
		if (strcmp((iter->first).c_str, tableName) == 0)
			vec->push_back(iter->second);
	}
}