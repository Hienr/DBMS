#include "fnc_state_machine.h"
#include "w_stk_constants.h"
#include "w_table_constants.h"
#include <iostream>

/****************************************************************************/
//                          String Tokenizer                                //
/****************************************************************************/

void init_table(int _table[][STK_COLUMNS]){
    //Run through 2D array and fill each slot with -1.
    for (unsigned int i = 0; i < STK_ROWS; i++)
        for (unsigned int j = 0; j < STK_COLUMNS; j++)
            _table[i][j] = FAIL;
}

void mark_success(int _table[][STK_COLUMNS], int state){
    //Mark 1 for success in COLUMN 0 of ROW = state
    _table[state][STK_COLUMNS-STK_COLUMNS] = 1;
}

void mark_fail(int _table[][STK_COLUMNS], int state){
    //Mark 0 for failure in COLUMN 0 of row = state
    _table[state][STK_COLUMNS-STK_COLUMNS] = 0;
}

bool is_success(int _table[][STK_COLUMNS], int state){
    //Return ROW 0 of row = state; it will return 1 or 0 for T/F.
    return _table[state][STK_COLUMNS-STK_COLUMNS];
}

void mark_cells(int row, int _table[][STK_COLUMNS], int from, int to,
                int state){
    //Run through the 2D array from 'from' and stop at 'to'.
    //Set every block it went through to a state.
    for (unsigned int j = (unsigned int)from; j <= to; j++)
        _table[row][j] = state;
}

void mark_cells(int row, int _table[][STK_COLUMNS], const char columns[],
                int state){
    //Given a character array, cast it to receive integer values.
    //Given the integer value of the character move to that column to mark.
    //Continue marking as long as the C-String does not hit NULL('\0').
    for (unsigned int i = 0; columns[i] != '\0'; i++)
        _table[row][int(columns[i])] = state;
}

void mark_cell(int row, int _table[][STK_COLUMNS], int column, int state){
    //Mark an individual cell given the row and column.
    _table[row][column] = state;
}

void print_table(int _table[][STK_COLUMNS]){
    //Run through the 2D array and print each item.
    for (unsigned int i = 0; i < STK_ROWS; i++)
    {
        for (unsigned int j = 0; j < STK_COLUMNS; j++)
            std::cout << _table[i][j];
        std::cout << std::endl;
    }
}

/****************************************************************************/
//                          PARSER TABLE STATE MACHINE                      //
/****************************************************************************/

void init_table(int _table[][PARSER_COLUMNS]){
    //Run through 2D array and fill each slot with -1.
    for (unsigned int i = 0; i < PARSER_ROWS; i++)
        for (unsigned int j = 0; j < PARSER_COLUMNS; j++)
            _table[i][j] = FAIL;
}

void mark_success(int _table[][PARSER_COLUMNS], int state){
    //Mark 1 for success in COLUMN 0 of ROW = state
    _table[state][PARSER_COLUMNS-PARSER_COLUMNS] = 1;
}

void mark_fail(int _table[][PARSER_COLUMNS], int state){
    //Mark 0 for failure in COLUMN 0 of row = state
    _table[state][PARSER_COLUMNS-PARSER_COLUMNS] = 0;
}

bool is_success(int _table[][PARSER_COLUMNS], int state){
    //Return ROW 0 of row = state; it will return 1 or 0 for T/F.
    return _table[state][PARSER_COLUMNS-PARSER_COLUMNS];
}

void mark_cells(int row, int _table[][PARSER_COLUMNS], int from, int to,
                int state){
    //Run through the 2D array from 'from' and stop at 'to'.
    //Set every block it went through to a state.
    for (unsigned int j = (unsigned int)from; j <= to; j++)
        _table[row][j] = state;
}

void mark_cells(int row, int _table[][PARSER_COLUMNS], const char columns[],
                int state){
    //Given a character array, cast it to receive integer values.
    //Given the integer value of the character move to that column to mark.
    //Continue marking as long as the C-String does not hit NULL('\0').
    for (unsigned int i = 0; columns[i] != '\0'; i++)
        _table[row][int(columns[i])] = state;
}

void mark_cell(int row, int _table[][PARSER_COLUMNS], int column, int state){
    //Mark an individual cell given the row and column.
    _table[row][column] = state;
}

void print_table(int _table[][PARSER_COLUMNS]){
    //Run through the 2D array and print each item.
    for (unsigned int i = 0; i < PARSER_ROWS; i++)
    {
        for (unsigned int j = 0; j < PARSER_COLUMNS; j++)
            std::cout << _table[i][j];
        std::cout << std::endl;
    }
}
