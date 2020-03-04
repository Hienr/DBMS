/*
 * Author: Raymond Hien
 * Project: Final Project - Relational Database
 * Purpose: Relational database program that will respond to SQL-like commands.
 */

#include <iostream>
#include "sql.h"

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                   Relational Database System
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Implementation of a database that will respond to SQL-like
 * commands. They include :
 * >> CREATE|MAKE TABLE <TABLENAME> FIELDS <FIELDNAME(S) . . .>
 * >> INSERT INTO <TABLE NAME> VALUES <VALUE(S) . . . >
 * >> SELECT *|<FIELD(S) . . .> FROM <TABLENAME> WHERE <CONDITION>

 * <CONDITION> include : <FIELDNAME> <RELATIONAL_OPERATOR> <VALUE>
 * chain conditions together with a <LOGICAL OPERATOR>
 * <RELATIONAL OPERATOR> : = < > <= >=
 * <LOGICAL OPERATOR> : AND OR
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int main()
{
    //SQL program will be instantiated
    SQL _program;

    //Runs the SQL
    _program.run();
    return 0;
}
