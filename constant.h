#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define BLOCK_SIZE 4096

#include <string>

using namespace std;

struct Block
{
	string filename;
	int ID;

	bool dir;
	bool pin;

	char content[BLOCK_SIZE];
};










#endif