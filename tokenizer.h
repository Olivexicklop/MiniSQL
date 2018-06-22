#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <vector>
#include <string>

using namespace std;

class Tokenizer
{
public:

    // Token type
    static const int INVALID;
    static const int START;
    static const int END;
    static const int FIGURE;
    static const int NUMBER;
    static const int STRING_SINGLE;
    static const int STRING_DOUBLE;
    static const int SYMBOL;
    static const int OPERATOR;

    // Read all tokens in SQL statement. Return number of semicolons read
    int getTokens(const char* sql, vector<string>* tokens, vector<int>* type);
};

#endif
