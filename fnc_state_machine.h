#ifndef STATE_MACHINE_FNC_H
#define STATE_MACHINE_FNC_H
#include "w_stk_constants.h"
#include "w_parser_constants.h"

/****************************************************************************/
//                          String Tokenizer                                //
/****************************************************************************/

//Fill all cells of the array with -1
void init_table(int _table[][STK_COLUMNS]);

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][STK_COLUMNS], int state);

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][STK_COLUMNS], int state);

//true if state is a success state
bool is_success(int _table[][STK_COLUMNS], int state);

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][STK_COLUMNS], int from, int to,
                int state);

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][STK_COLUMNS], const char columns[],
                int state);

//Mark this row and column
void mark_cell(int row, int table[][STK_COLUMNS], int column, int state);

//This can realistically be used on a small table
void print_table(int _table[][STK_COLUMNS]);

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos);


/****************************************************************************/
//                          PARSER TABLE STATE MACHINE                      //
/****************************************************************************/

//Fill all cells of the array with -1
void init_table(int _table[][PARSER_COLUMNS]);

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][PARSER_COLUMNS], int state);

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][PARSER_COLUMNS], int state);

//true if state is a success state
bool is_success(int _table[][PARSER_COLUMNS], int state);

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][PARSER_COLUMNS], int from, int to,
                int state);

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][PARSER_COLUMNS], const char columns[],
                int state);

//Mark this row and column
void mark_cell(int row, int table[][PARSER_COLUMNS], int column, int state);

//This can realistically be used on a small table
void print_table(int _table[][PARSER_COLUMNS]);

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos);


#endif // STATE_MACHINE_FNC_H
