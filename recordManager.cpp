#include "recordManager.h"
#include "fileManager.h"
#include "minisql.h"

using namespace std;





bool RecordManager::createTable(const char* tableName)
{
	FileManager::createFile(("record/" + string(tableName)).c_str());
	return true;
}

bool RecordManager::dropTable(const char* tableName)
{
	FileManager::removeFile(("record/" + string(tableName)).c_str());
	return true;
}

void RecordManager::insert(const char* tableName, const char* value)
{
	FileManager* file = new FileManager(("record/" + string(tableName)).c_str());
	file->addValue(value);
	delete file;
}

int RecordManager::select(const char* tableName, 
	string column, string Operator, const char* operand)
{
	FileManager* file = new FileManager(("record/" + string(tableName)).c_str());
	CatalogManager* Manager = minisql::catalogManager;

	int sum = 0, id = 0;

	char data[DATA_LENGTH];

	while ((id = file->getNextValue(data)) >= 0)
	{
		if (filter(tableName,data,column,Operator,operand))
		{
			sum++;
		}
	}

	delete file;
	return sum;
}

// int RecordManager::Delete(const char* tableName)
// {
// 	FileManager* file = new FileManager(("record/" + string(tableName)).c_str());

// 	int sum = 0, id = 0;


// 	while (id < file->recordNum)
// 	{
// 		if (filter())
// 		{
// 			sum++;
// 		}
// 		id++;
// 	}
// 	return sum;
// }

int RecordManager::filter(const char* tableName, const char* data, 
	string column, string Operator, const char* operand)
{
	int type;

	CatalogManager* Manager = minisql::catalogManager;
	Table* table = Manager->getTable(tableName);

	char dataOut[FIXED_LENGTH];
	type = table->getColValue(column.c_str(), data, dataOut);
	if (type == TYPE_INT)
	{
		return intCmp(dataOut, Operator, operand);
	}
    return 000;
}

bool RecordManager::intCmp(const char* data, string Operator, const char* operand)
{
	const int a = *(reinterpret_cast<const int*>(data));
	const int b = atoi(operand);

	if (Operator == EQUAL) return a == b;
	if (Operator == GREATER) return a > b;
	if (Operator == LESS) return a < b;
	if (Operator == EQUAL_GREATER) return a >= b;
	if (Operator == EQUAL_LESS) return a <= b;
	return false;
}

