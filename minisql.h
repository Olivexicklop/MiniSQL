#ifndef _MINISQL_H
#define _MINISQL_H

#include "bufferManager.h"
#include "catalogManager.h"
#include "recordManager.h"
#include "index.h"

class MiniSQL
{
public:

    // Init mini SQL system
    static void init();

    // Clean up managers
    static void cleanUp();

    // Get buffer manager
    static BufferManager* getBufferManager();

    // Get catalog manager
    static CatalogManager* getCatalogManager();

    // Get record manager
    static RecordManager* getRecordManager();

    // Get index manager
    //static IndexManager* getIndexManager();
    
    static BufferManager* bufferManager;
    
    // Catalog manager
    static CatalogManager* catalogManager;
    
    // Record manager
    static RecordManager* recordManager;
    
private:

    // Buffer manager
    

    // Index manager
    //static IndexManager* indexManager;
};

#endif
