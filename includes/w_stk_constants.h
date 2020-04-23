#ifndef STK_CONSTANTS_H
#define STK_CONSTANTS_H
#include <string>

/* Constants for the STokenizer class */

enum Token_Type
{   ALPHA, NUMBER, PUNCT, SPACE, QUOTATION, OPERATOR, UNKNOWN, NONE = -1};

enum Initializations
{ EMPTY = 0, ZERO = 0, ONE_CHAR = 1, MAX_BUFFER = 1000,
    STK_ROWS = 17, STK_COLUMNS = 256 };

enum State
{ INITIAL_STATE = 0, STATE = STK_COLUMNS - STK_COLUMNS };
#endif // STK_CONSTANTS_H
