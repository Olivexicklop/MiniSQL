#ifndef MINISQL_H
#define MINISQL_H

//#include "stdafx.h"
//#include "API.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <list>
#include <vector>
//#include "Interpreter.h"
#include "recordManager.h"
#include "fileManager.h"
#include "bufferManager.h"
#include "catalogManager.h"

using namespace std;

//enum{PRIMARY, UNIQUE};
//enum{INT,FLOAT,CHAR};
#define UNIQUE 1
#define PRIMARY 2
#define INT 1
#define FLOAT 2
#define CHAR 3

//	用于描述表中一个属性在表中的具体信息
struct Attribute
{
	string attr_name;
	int attr_type;	//属性的数据类型，分别为CHAR, FLOAT, INT
	int attr_key_type;//属性的主键类型，分别为PRIMARY, UNIQUE, Empty=0
	int attr_len; 	//属性所存放的数据的长度，如果不是CHAR，则长度标记为1?
	int attr_id;    //属性在表中是第几个
};

//	用于描述表的信息
//struct Table
//{
//    string table_name;  //表名
//    int attr_count;        //表中属性的总个数
//    Attribute attrs[32];    //表的所有属性列表, 最多32个属性
//
//    int searchAttrId(string att_name) {
//        for (int i = 0; i < attr_count; i++)
//        {
//            if (attrs[i].attr_name == att_name)
//            {
//                return i;
//            }
//        }
//        return -1;
//    }
//};

//	用于描述判断条件的信息
struct Condition
{
	string attr_name;	//条件所对应的属性名
	string op_type;		//条件所用到的操作符，分别为"<>", "=", ">=", "<=", "<", ">"
	string cmp_value;	//条件所需要进行比较的值
};
//改为vector
typedef vector<Condition> Condition_list;
//typedef Condition_list condList;
typedef Condition_list condList;


//	用于描述索引信息
//struct Index
//{
//    string index_name;
//    string table_name;
//    string attr_name;    //索引所对应的属性
//};

class minisql
{
	

public:
	//minisql();
	//~minisql();

	static BufferManager* bufferManager;

	static RecordManager* recordManager;
    
    static CatalogManager* catalogManager;
	
};


#endif
