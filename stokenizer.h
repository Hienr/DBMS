#ifndef STOKENIZER_H
#define STOKENIZER_H
#include "w_stk_constants.h"
#include "w_table_constants.h"
#include "token.h"
#include <iostream>
#include <string.h>

class STokenizer
{
    public:
        STokenizer();
        STokenizer(char str[]);
        STokenizer(string str);
        bool done();            //true: there are no more tokens
        bool more();            //true: there are more tokens

        //extract one token (very similar to the way cin >> works)
        friend STokenizer& operator >> (STokenizer& s, Token& t){
            const bool debug = false;

            string temp = "";
            //Check first char of every token to give it a type
            if (s.get_token(INITIAL_STATE, temp)){
                if (debug) cout << "STK >> " << temp << endl;
                if (isdigit(temp[ZERO]))
                    t.set_type(NUMBER);
                else if (isalpha(temp[ZERO]))
                    t.set_type(ALPHA);
                else if (s.isoperator(temp[ZERO]))
                    t.set_type(OPERATOR);
                else if (temp[ZERO] == '"')
                    t.set_type(QUOTATION);
                else if ((ispunct(temp[ZERO])) && (temp.length() == ONE_CHAR))
                    t.set_type(PUNCT);
                else if (isspace(temp[ZERO]))
                    t.set_type(SPACE);
                else
                    t.set_type(UNKNOWN);
                t.set_string(temp); //Set the string to token
            }
            else {
                //If given empty string.
                s._pos++;
            }
            return s;
        }

        //set a new string as the input string
        void set_string(char str[]);

        //Prints the current table stored.
        void print_table();

    private:
        //create table for all the tokens we will recognize
        //                      (e.g. doubles, words, etc.)
        void make_table(int _table[][STK_COLUMNS]);

        //extract the longest string that match
        //     one of the acceptable token types
        bool get_token(int start_state, string& token);
        //---------------------------------
        char _buffer[MAX_BUFFER];       //input string
        int _pos;                       //current position in the string
        int _table[STK_ROWS][STK_COLUMNS];

        bool isoperator(char c);
};
#endif // STOKENIZER_H
