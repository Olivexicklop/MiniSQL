#ifndef _INDEX_H_
#define _INDEX_H_

#include<string>

using namespace std;

class Index {
private:
	string indexName;
	string tableName;
	string colName;

public:
	Index(const char* data); //Constructor:data contains indexName, tableName and colName
	const char* getIndexName() const;
	const char* getTableName() const;
	const char* getColName() const;
};

#endif