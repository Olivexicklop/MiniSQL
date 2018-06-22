#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <string>

#define BLOCK_SIZE 4096
#define MAX_BLOCK_NUM 100
#define FIXED_LENGTH 124
#define DATA_LENGTH 114

#define EQUAL "="
#define GREATER ">"
#define LESS "<"
#define EQUAL_GREATER ">="
#define EQUAL_LESS "<="

#define COND_EQ 0
#define COND_NE 1
#define COND_LT 2
#define COND_GT 3
#define COND_LE 4
#define COND_GE 5

using namespace std;

struct Block
{
	string filename;
	int ID;

	bool dir;
	bool pin;

	char content[BLOCK_SIZE];
};



// Max length of table/index/column name
#define MAX_NAME_LENGTH 31

// Data types
#define TYPE_NULL 0
#define TYPE_CHAR 255
#define TYPE_INT 256
#define TYPE_FLOAT 257

//unque
#define UNIQUE 1
#define NOT_UNIQUE 0
#define ERROR_UNIQUE -1










#endif
