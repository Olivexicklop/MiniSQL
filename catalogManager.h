#ifndef _CATALOG_MANAGER_H_
#define _CATALOG_MANAGER_H_

#include <vector>
#include <string>
#include <unordered_map>

// #include "file/heapFile.h"
#include "table.h"
#include "index.h"
#include "heapFile.h"

using namespace std;

class CatalogManager
{
private:
	unordered_map<string, Table*> tableMap;
	unordered_map<string, int> tableIdMap;

	unordered_map<string, Index*> indexMap;
	unordered_map<string, int> indexIdMap;

	HeapFile* tablesFile; //tableName + primary

	HeapFile* indicesFile; //indexName + tableName + colName

public:
	
	CatalogManager(); // Constructor
	~CatalogManager();	// Destructor

	

	Table* getTable(const char* tableName) const;

	bool createTable(
		const char* tableName, const char* primary,
		const vector<string>* colName, const vector<short>* colType, vector<char>* colUnique
	);     // Return true if success

	
	bool dropTable(const char* tableName); // Return true if success

	


	Index* getIndex(const char* indexName) const;

	void getIndexByTableName(const char* tableName, vector<Index*>* vec);

	Index* getIndexByTableCol(const char* tableName, const char* colName);

	bool createIndex(const char* indexName, const char* tableName, const char* colName);//  Return true if success

	bool dropIndex(const char* indexName);  //  Return true if success


};




#endif