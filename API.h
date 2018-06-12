#ifndef API_H
#define API_H 

#include "minisql.h"
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

typedef Condition_list condList;

class API {
public:
	API(){};
	//CatalogManager myCatalog;
	//IndexManager myIndex;
	//RecordManager myRecord;
	void createTable(const string &SQL);
	void createIndex(const string &SQL);
    void dropTable(string &tableName);
    void dropIndex(string &indexName);
	void insertTuple(const string &SQL);
	void selectTuple_nowhere(string &tableName);
	void selectTuple(string &tableName, const string &SQL);
	void deleteTuple_nowhere(string &tableName);
	void deleteTuple(string &tableName, const string &SQL);

private:
	vector<string> split(const string &SQL, const string &pattern);
	void drawResult(const Table &table, vector<string>select_value);
};

#endif