#include "stokenizer.h"
#include "fnc_state_machine.h"
#include <iostream>
#include "w_stk_constants.h"
#include <string.h>

using namespace std;

STokenizer::STokenizer(){
    _buffer[0] = '\0';
    _pos = ZERO;
    init_table(_table);
    make_table(_table);
}

STokenizer::STokenizer(char str[]){
    set_string(str);
    init_table(_table);
    make_table(_table);
}

STokenizer::STokenizer(string str){
    strcpy(_buffer, str.c_str());
    init_table(_table);
    make_table(_table);
}

bool STokenizer::done(){
    return !(_pos <= strlen(_buffer));
}

bool STokenizer::more(){
//    if (_buffer[_pos] == '\0')
//        return false;
    return (_pos <= strlen(_buffer));
}

void STokenizer::set_string(char str[]){
    strcpy(_buffer, str);
    _pos = ZERO;
}

void STokenizer::make_table(int (*_table)[STK_COLUMNS]){
    //Mark off Success/Nonsuccess states
    mark_fail(_table, ROW0);
    //ROW1-3 DOUBLES
    mark_success(_table, ROW1);
    mark_fail(_table, ROW2);
    mark_success(_table, ROW3);
    //ROW4 WORDS
    mark_success(_table, ROW4);
    mark_fail(_table, ROW5);
    mark_success(_table, ROW6);
    //ROW 7 PUNCTUATION
    mark_success(_table, ROW7);
    //ROW8 " MACHINE
    mark_success(_table, ROW8);

    //ROW11 OPERATOR MACHINE
    mark_success(_table, ROW11);
    mark_fail(_table, ROW12);

    //ROW 15 SPACES
    mark_fail(_table, ROW15);

    //0 STARTING STATE
    mark_cells(ROW0, _table, ' ', ' ', GOSTATE15);//ASCII SPACE --------> FAIL
    mark_cells(ROW0, _table, '!', '!', GOSTATE7);//ASCII 32-32 -------->SUCCESS
    mark_cells(ROW0, _table, '"', '"', GOSTATE8);//ASCII " ----------->SUCCESS
    mark_cells(ROW0, _table, '#', ')', GOSTATE7);//ASCII 35-41--------->SUCCESS
    mark_cells(ROW0, _table, '*', '+', GOSTATE11);//ASCII 42-43-------->SUCCESS
    mark_cell(ROW0, _table, ',', GOSTATE7);       //ASCII 44----------->SUCCESS
    mark_cell(ROW0, _table, '-', GOSTATE11);      //ASCII 45 ---------->SUCCESS
    mark_cell(ROW0, _table, '/', GOSTATE11);        //ASCII 47 -----> SUCCESS
    mark_cells(ROW0, _table, ':', ';', GOSTATE7);//ASCII 58-59 -------->SUCCESS
    mark_cells(ROW0, _table, '<', '>', GOSTATE11);//ASCII 60-62-------->SUCCESS
    mark_cells(ROW0, _table, '?', '@', GOSTATE7);//ASCII 63-64--------->SUCCESS
    mark_cells(ROW0, _table, '[', '\'', GOSTATE7);//ASCII 91-96 ------->SUCCESS
    mark_cells(ROW0, _table, '{', '~', GOSTATE7);  //ASCII 123-126 ---->SUCCESS
    mark_cells(ROW0, _table, MAJUSCULE, NBSP, GOSTATE14);//123-126 ----->SUCCESS
    mark_cells(ROW0, _table, 'A', 'Z', GOSTATE4);    //ASCII 65-90----->SUCCESS
    mark_cells(ROW0, _table, 'a', 'z', GOSTATE4);    //ASCII 97-122 --->SUCCESS
    mark_cells(ROW0, _table, '.','.', GOSTATE2);     //ASCII 46 ---->NONSUCCESS
    mark_cells(ROW0, _table, DIGITS, GOSTATE1);      //ASCII 48-57 ---->SUCCESS

    //Mark off table for DOUBLES---------------------------------------------//
    //-----------------------------------------------------------------------//
    //1 SUCCESS STATE IF DIGIT IS INPUTTED
    mark_cells(ROW1, _table, '.','.', GOSTATE2);     //ASCII 46 ---->NONSUCCESS
    mark_cells(ROW1, _table, DIGITS, GOSTATE1);      //ASCII 48-57 ---->SUCCESS

    //2 NONSUCCESS STATE IF DECIMAL IS INPUTTED
    mark_cells(ROW2, _table, DIGITS, GOSTATE3);      //ASCII 48-57 ---->SUCCESS

    //3 SUCCESS STATE IF DIGIT IS INPUTTED AFTER DECIMAL
    mark_cells(ROW3, _table, DIGITS, GOSTATE3);      //ASCII 48-57 ---->SUCCESS

    //Mark off table for WORDS-----------------------------------------------//
    //-----------------------------------------------------------------------//
    //4 SUCCESS STATE IF ALPHA IS INPUTTED
        //lowercase letters:
    mark_cells(ROW4, _table, 'A', 'Z', GOSTATE4);     //ASCII 65-90 ---->SUCCESS
        //UPPERCASE LETTERS:
    mark_cells(ROW4, _table, 'A', 'z', GOSTATE4);     //ASCII 97-122 --->SUCCESS
        //Apostrophe
    mark_cells(ROW4, _table, '\'', '\'', GOSTATE5);

    //5 NONSUCCESS STATE IF APOSTROPHE OR - IS INPUTTED
    //lowercase letters:
    mark_cells(ROW5, _table, 'A', 'Z', GOSTATE6);    //ASCII 65-90 ---->SUCCESS
        //UPPERCASE LETTERS:
    mark_cells(ROW5, _table, 'A', 'z', GOSTATE6);    //ASCII 97-122 --->SUCCESS

    //6 SUCCESS STATE IF ALPHA IS INPUTTED
    //lowercase letters:
    mark_cells(ROW6, _table, 'A', 'Z', GOSTATE6);    //ASCII 65-90 ---->SUCCESS
        //UPPERCASE LETTERS:
    mark_cells(ROW6, _table, 'A', 'z', GOSTATE6);    //ASCII 97-122 --->SUCCESS

    //Mark off table for PUNCTUATION-----------------------------------------//
    //-----------------------------------------------------------------------//
    mark_cells(ROW7, _table, '!', '!', GOSTATE7);//ASCII 32-47 -------->SUCCESS
    mark_cells(ROW7, _table, '#', ')', GOSTATE7);
    mark_cells(ROW7, _table, ':', '@', GOSTATE7);//ASCII 58-64 -------->SUCCESS
    mark_cells(ROW7, _table, '[', '\'', GOSTATE7);//ASCII 91-96 ------->SUCCESS
    mark_cells(ROW7, _table, '{', '~', GOSTATE7);//ASCII 123-126 ---->SUCCESS

    //" MACHINE -------------------------------------------------------------//
    //-----------------------------------------------------------------------//
    //SUCCESS STATE IF " WAS INPUTTED

    // ROW 8

    //OPERATOR MACHINE ------------------------------------------------------//
    //-----------------------------------------------------------------------//
    //SUCCESS STATE IF NO OPERATOR WAS GIVEN AFTER OPERATOR
    //FAIL IMMEDIATELY WHEN = IS GIVEN AFTER A > OR < OR =
    // eg: >= <= ==
    mark_cell(ROW11, _table, '=', GOSTATE12);

    //Mark off table for UNKNOWN---------------------------------------------//
    //-----------------------------------------------------------------------//
    mark_cells(ROW14, _table, MAJUSCULE, NBSP, GOSTATE14);

}

bool STokenizer::get_token(int start_state, string &token){
    int success_pos = _pos;                 //Declare Position Flag for success
    if (_buffer[0] != '\0')
    {
        for (unsigned int i = _pos; i < strlen(_buffer); i++)
        {
            //For loop to iterate through the c-string
            //Change state depending on the character in the buffer
            start_state =
                    _table
                    [start_state][(unsigned int)(unsigned char)(_buffer[i])];

            //If the state given is SUCCESS or NONSUCCESS
            if (start_state != FAIL)
            {
                //Add the character to the token, and record the position
                token += _buffer[i];
                success_pos = i;
            }
            else {
                //If FAIL break;
                break;
            }
        }
        _pos = ++success_pos;
        return true;

    }
    else
        return false;
}

bool STokenizer::isoperator(char c){
    switch (c){
        case '+' :
        case '-' :
        case '/' :
        case '*' :
        case '=' :
        case '<' :
        case '>' :
            return true;
        default : return false;
    }
}

void STokenizer::print_table(){
    ::print_table(_table);
}

