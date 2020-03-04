#ifndef PARSER_CONSTANTS_H
#define PARSER_CONSTANTS_H
#include <vector>
#include <string>

using namespace std;

/* Constants used for parser */

enum ParserTable { PARSER_ROWS = 40, PARSER_COLUMNS = 40 };

enum Keys { Z,
            /* COMMANDS */
                CREATE, MAKE,       //1..2
                INSERT,             //3
                SELECT,             //4
            /* CREATE COMMANDS */
                TABLE,              //5
                FIELDS,             //6
            /* INSERT COMMANDS */
                INTO, VALUES,       //7..8
            /* SELECT COMMANDS */   //9..10..11
                ASTERISK, FROM, WHERE,
            /* OPERATORS */         //12
                RELATIONAL_OPERATOR,
            /* LOGICAL_OPERATORS */
                AND, OR,            //13..14
            /* NON COMMANDS */
                QUOTE, COMMA, BLANK, SYMBOL, BATCH
                                    //15..16..17..18
};

#endif // PARSER_CONSTANTS_H
