#ifndef _CATALOG_MANAGER_H_
#define _CATALOG_MANAGER_H_

#include <vector>
#include <string>
#include <unordered_map>

// #include "file/heapFile.h"
#include "table.h"
#include "index.h"
#include "fileManager.h"

using namespace std;

class CatalogManager
{
private:
	unordered_map<string, Table*> tableMap;
	unordered_map<string, int> tableIdMap;

	unordered_map<string, Index*> indexMap;
	unordered_map<string, int> indexIdMap;

	FileManager* tablesFile; //tableName + primary

	FileManager* indicesFile; //indexName + tableName + colName


	void getIndexByTableName(const char* tableName, vector<Index*>* vec);

public:
	
	CatalogManager(); // Constructor
	~CatalogManager();	// Destructor

	

	Table* getTable(const char* tableName) const;

	bool createTable(
		const char* tableName, const char* primary,
		const vector<string>* colName, const vector<short>* colType, vector<char>* colUnique
	);     // Return true if success

	
    // include dropping related indices. Return true if success
	bool dropTable(const char* tableName); 
	
	Index* getIndex(const char* indexName) const;

	Index* getIndexByTableCol(const char* tableName, const char* colName);

	bool createIndex(const char* indexName, const char* tableName, const char* colName);//  Return true if success

	bool dropIndex(const char* indexName);  //  Return true if success


};




#endif