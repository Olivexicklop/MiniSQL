#include<iostream>
#include<cstdio>

#include "general.h"
#include "constant.h"

using namespace std;
//
//int General:: getTypeSize(short type)
//{
//    if (type == TYPE_NULL) return 0;
//    else if (type == TYPE_INT || type == TYPE_FLOAT) return 4;
//    else if (type < TYPE_CHAR) return type + 1;   //²»Ì«¶®
//
//    else
//    {
//        cerr << "ERROR: [Utils::getTypeSize] Unknown type " << type << "! " << endl;
//        cerr << "Please use type 'char', 'int' or 'float'(MiniSQL only supports these three data types)" << endl;
//        return 0;
//    }
//}
int General::getTypeSize(short type)
{
    if (type == TYPE_NULL)
        return 0;
    else if (type < TYPE_CHAR)
        return type + 1;
    else if (type == TYPE_INT)
        return 4;
    else if (type == TYPE_FLOAT)
        return 4;
    else
    {
        cerr << "ERROR: [Utils::getTypeSize] Unknown type " << type << "!" << endl;
        return 0;
    }
}

char* General::getDataFromStr(const char* s, int type)
{
    char* key = NULL;
    int size = getTypeSize(type);
    
    if (type <= TYPE_CHAR)
    {
        int len = strlen(s);
        if (len > type)
        {
            cerr << "ERROR: [Utils::getDataFromStr] Expecting char(" << type << "), but found char(" << len << ")." << endl;
            return NULL;
        }
        
        key = new char[size]();
        memcpy(key, s, min(size, len + 1));
    }
    else if (type == TYPE_INT)
    {
        int value;
        if (sscanf(s, "%d", &value) < 1)
        {
            cerr << "ERROR: [Utils::getDataFromStr] Expecting int, but found '" << s << "'." << endl;
            return NULL;
        }
        
        key = new char[size]();
        memcpy(key, &value, size);
    }
    else if (type == TYPE_FLOAT)
    {
        float value;
        if (sscanf(s, "%f", &value) < 1)
        {
            cerr << "ERROR: [Utils::getDataFromStr] Expecting float, but found '" << s << "'." << endl;
            return NULL;
        }
        
        key = new char[size]();
        memcpy(key, &value, size);
    }
    
    return key;
}
