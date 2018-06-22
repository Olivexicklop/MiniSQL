#include <cstdio>
#include <iostream>
#include "tokenizer.h"

using namespace std;

// Token type
const int Tokenizer::INVALID = -1;
const int Tokenizer::START = 0;
const int Tokenizer::END = 1;
const int Tokenizer::FIGURE = 2;
const int Tokenizer::NUMBER = 3;
const int Tokenizer::STRING_SINGLE = 4;
const int Tokenizer::STRING_DOUBLE = 5;
const int Tokenizer::SYMBOL = 6;
const int Tokenizer::OPERATOR = 7;

// Read all tokens in SQL statement. Return number of semicolons read
int Tokenizer::getTokens(const char* sql, vector<string>* tokens, vector<int>* type)
{
    int endCount = 0;
    int state = START;
    int cursor = 0;
    string token = "";

    while (true)
    {
        bool moveCursor = true;
        char c = sql[cursor];
        if (state != STRING_SINGLE && state != STRING_DOUBLE && c >= 'A' && c <= 'Z')
            c += 'a' - 'A';

        if (state == START)
        {
            if (c == ';')
            {
                state = END;
                moveCursor = false;
            }
            else if ((c >= 'a' && c <= 'z') || c == '_')
            {
                state = FIGURE;
                moveCursor = false;
            }
            else if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')
            {
                state = NUMBER;
                moveCursor = false;
            }
            else if (c == '\'')
                state = STRING_SINGLE;
            else if (c == '"')
                state = STRING_DOUBLE;
            else if (c == ',' || c == '(' || c == ')' || c == '*')
            {
                state = SYMBOL;
                moveCursor = false;
            }
            else if (c == '=' || c == '<' || c == '>')
            {
                state = OPERATOR;
                moveCursor = false;
            }
            else if (c == ' ' || c == '\n' || c == '\t') {}
            else if (c == 0)
                break;
            else
            {
                cerr << "ERROR: [Tokenizer::getTokens] Unknown character '" << c << "'!" << endl;
                tokens->push_back(token);
                type->push_back(INVALID);
                token.clear();
            }
        }
        else if (state == END)
        {
            token += c;

            tokens->push_back(token);
            type->push_back(END);
            token.clear();
            endCount++;

            state = START;
        }
        else if (state == FIGURE)
        {
            if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
                token += c;
            else
            {
                tokens->push_back(token);
                type->push_back(FIGURE);
                token.clear();

                state = START;
                moveCursor = false;
            }
        }
        else if (state == NUMBER)
        {
            if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')
                token += c;
            else
            {
                tokens->push_back(token);
                type->push_back(NUMBER);
                token.clear();

                state = START;
                moveCursor = false;
            }
        }
        else if (state == STRING_SINGLE || state == STRING_DOUBLE)
        {
            if ((c == '\'' && state == STRING_SINGLE) || (c == '"' && state == STRING_DOUBLE))
            {
                tokens->push_back(token);
                type->push_back(state);
                token.clear();

                state = START;
            }
            else if (c == '\n' || c == 0)
            {
                cerr << "ERROR: [Tokenizer::getTokens] New line symbol reached when reading string!" << endl;
                tokens->push_back(token);
                type->push_back(INVALID);
                token.clear();
                state = START;
            }
            else
                token += c;
        }
        else if (state == SYMBOL)
        {
            token += c;

            tokens->push_back(token);
            type->push_back(SYMBOL);
            token.clear();

            state = START;
        }
        else if (state == OPERATOR)
        {
            if (c == '=' || c == '<' || c == '>')
                token += c;
            else
            {
                tokens->push_back(token);
                type->push_back(OPERATOR);
                token.clear();

                state = START;
                moveCursor = false;
            }
        }

        if (moveCursor)
            cursor++;
    }

    return endCount;
}
