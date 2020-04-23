#ifndef Z_WORK_REPORT_H
#define Z_WORK_REPORT_H
/*
    Data Structures Used:
        - Maps/Multimaps based on a B+ Tree
        - State Machines
        - Stacks/Queues based on linked lists

    Features:
        -Not Implemented:

        -Implemented:

        /* Program features the SQL class that offers two distinct modes :
         * Single-line commands/Batch Processing
         *
         * Single-Line commands will simply ask user for a command and
         * perform said command.
         * Batch processing will take a file name on disk and perform the
         * commands given.
         *
         * Program will ask user for an input, generate a parsed version,
         * and interpret the parsed command to perform a function.
         *
         * Functions of the database include SQL-like commands such as
         * (and limited to) :
         *
         * [1] create|make table <TABLENAME> fields <FIELDNAMES. . .>
         * [2] insert into <TABLENAME> values <VALUES. . .>
         * [3] select <FIELDNAMES...|*> from <TABLENAME> where <EXPRESSION>
         * [4] batch <FILENAME>
         *
         * an EXPRESSION can include : <FIELDNAME> <RELATIONALOP> <VALUE>
         *                              <LOGICAL OPERATOR>
         * <VALUE> : A string of alphanumeric characters, or a string of alphanumeric
                    characters and spaces enclosed by double quotation marks:
                    "Jean Luise", Finch, 1923
         * <RELATIONALOP> : =, <, >, <=, >=
         * <LOGICAL OPERATOR>: and, or
         * <FILENAME> Any string that must include an extension eg: (.bin)
         *
         * Program implements a state machine to interpret commands. State
         * machines will be implemented for a Tokenizer to simply return
         * tokens. As well as a parser to interpret tokens as apart of
         * a valid SQL command.
         *
         * Program implements Maps/Multimaps as the primary data structure
         * used to map certain values, and database indexing.
         *
         * Program implements Stacks/Queues to evaluate expressions given from
         * a ( select * from table where <EXPRESSION> ). A seperate RPN/Set
         * class will manage evaluations of expressions. Set will take in
         * a string, and call on RPN to get a postfix expression. Set will
         * then evaluate the postfix expression.
         *
         * Program uses binary files for disk storage of database records.
         * Program uses text files to record name of fields, tables, or other
         * information that needs to be remembered from session to session.
         *
         * Program will be error checking any command given such as
         * invalid input/commands. After each processing of a command, program
         * will echo back: command, table, command #, and possible error msg.
         *
         * Program saves all echoed outputs to command line to a txt file
         * on disk as well.


        -Partly implemented:

    Bugs     :  features that are implemented are bug-free.

*/
#endif // Z_WORK_REPORT_H
