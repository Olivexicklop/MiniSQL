#include <cstring>
#include <iostream>
#include <string>

#include "constant.h"
#include "utils.h"
#include "interpreter.h"
//#include "index/bpTree.h"
#include "minisql.h"

using namespace std;

// Managers
BufferManager* MiniSQL::bufferManager = NULL;
CatalogManager* MiniSQL::catalogManager = NULL;
RecordManager* MiniSQL::recordManager = NULL;
IndexManager* MiniSQL::indexManager = NULL;

// Init mini SQL system
void MiniSQL::init()
{
    // Check if catalog/tables.mdb exists
    if (!Utils::fileExists("catalog/tables"))
        HeapFile::createFile("catalog/tables", MAX_NAME_LENGTH*2);

    // Check if catalog/indices.mdb exists
    if (!Utils::fileExists("catalog/indices"))
        HeapFile::createFile("catalog/indices", MAX_NAME_LENGTH*3);

    // Init managers
    bufferManager = new BufferManager();
    catalogManager = new CatalogManager();
    recordManager = new RecordManager();
    indexManager = new IndexManager();
}

// Clean up managers
void MiniSQL::cleanUp()
{
    delete bufferManager;
    delete catalogManager;
    delete recordManager;
    delete indexManager;
}

// Get buffer manager
BufferManager* MiniSQL::getBufferManager()
{
    return bufferManager;
}

// Get catalog manager
CatalogManager* MiniSQL::getCatalogManager()
{
    return catalogManager;
}

// Get record manager
RecordManager* MiniSQL::getRecordManager()
{
    return recordManager;
}

// Get index manager
IndexManager* MiniSQL::getIndexManager()
{
    return indexManager;
}

// Main function
int main()
{
    //MiniSQL::init();
    Interpreter* interpreter = new Interpreter();
    
    string sql;

	cout << endl;
	cout << "\t\t***********************************************" << endl;
	cout << "\t\t             »¶Ó­Ê¹ÓÃ MiniSQL !" << endl;
	cout << "\t\t               Version (1.0)  " << endl;
	cout << "\t\t               ÇëÊäÈëSQLÓï¾ä " << endl;
	cout << "\t\t***********************************************" << endl;
	cout << endl << endl;

    while (!interpreter->isExiting())
    {
        if (interpreter->tokenVecEmpty())
            cout << endl << "minisql> ";
        else
            cout << "    ...> ";

        getline(cin, sql);
        interpreter->execute(sql.c_str());
    }
    
    delete interpreter;
    MiniSQL::cleanUp();
    return 0;
}
