#ifndef _GENERAL_H_
#define _GENERAL_H_

class General {
public:
	static int getTypeSize(short type);
	static char* getDataFromStr(const char* s, short type);
}
#endif