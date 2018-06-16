#include "recordManager.h"
#include "fileManager.h"

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

int RecordManager::select(const char* tableName)
{
	FileManager* file = new FileManager(("record/" + string(tableName)).c_str());

	int sum = 0, id = 0;


	while (id < file->recordNum)
	{
		if (check())
		{
			sum++;
		}
		id++;
	}
	return sum;
}

int RecordManager::Delete(const char* tableName)
{
	FileManager* file = new FileManager(("record/" + string(tableName)).c_str());

	int sum = 0, id = 0;


	while (id < file->recordNum)
	{
		if (check())
		{
			sum++;
		}
		id++;
	}
	return sum;
}

