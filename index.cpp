#include "index.h"
#include "constant.h"

Index::Index(const char* data)
{
	indexName = data;
	tableName = data + MAX_NAME_LENGTH;
	colName = data + MAX_NAME_LENGTH * 2;
}

const char* Index::getIndexName() const
{
	return indexName.c_str();
}

const char* Index::getTableName() const
{
	return tableName.c_str();
}

const char* Index::getColName() const
{
	return colName.c_str();
}