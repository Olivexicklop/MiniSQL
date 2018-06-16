#include<iostream>
#include<cstdio>

#include "general.h"
#include "constant.h"

using namespace std;

int General:: getTypeSize(short type)
{
	if (type == TYPE_NULL) return 0;
	else if (type == TYPE_INT || type == TYPE_FLOAT) return 4;
	else if (type < TYPE_CHAR) return type + 1;   //²»Ì«¶®

	else
	{
		cerr << "ERROR: [Utils::getTypeSize] Unknown type " << type << "! " << endl;
		cerr << "Please use type 'char', 'int' or 'float'(MiniSQL only supports these three data types)" << endl;
		return 0;
	}
}