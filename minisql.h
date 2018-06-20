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

//	������������һ�������ڱ��еľ�����Ϣ
struct Attribute
{
	string attr_name;
	int attr_type;	//���Ե��������ͣ��ֱ�ΪCHAR, FLOAT, INT
	int attr_key_type;//���Ե��������ͣ��ֱ�ΪPRIMARY, UNIQUE, Empty=0
	int attr_len; 	//��������ŵ����ݵĳ��ȣ��������CHAR���򳤶ȱ��Ϊ1?
	int attr_id;    //�����ڱ����ǵڼ���
};

//	�������������Ϣ
//struct Table
//{
//    string table_name;  //����
//    int attr_count;        //�������Ե��ܸ���
//    Attribute attrs[32];    //������������б�, ���32������
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

//	���������ж���������Ϣ
struct Condition
{
	string attr_name;	//��������Ӧ��������
	string op_type;		//�������õ��Ĳ��������ֱ�Ϊ"<>", "=", ">=", "<=", "<", ">"
	string cmp_value;	//��������Ҫ���бȽϵ�ֵ
};
//��Ϊvector
typedef vector<Condition> Condition_list;
//typedef Condition_list condList;
typedef Condition_list condList;


//	��������������Ϣ
//struct Index
//{
//    string index_name;
//    string table_name;
//    string attr_name;    //��������Ӧ������
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
