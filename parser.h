#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "token.h"
#include "mmap.h"
#include "map.h"
#include "fnc_state_machine.h"

using namespace std;

class Parser{

    /* type definitions */
    typedef MMap<string, string> mmap_ss;
    typedef Map<string, int> map_keywords;
    typedef Map<int, string> map_states;

    public:
        Parser(char input[]);

        mmap_ss get_parseTree();                //returns a Multimap of
                                                //a input in tree form

        map_keywords get_keywords();            //returns a Map of keywords
                                                //given

        friend ostream& operator <<(ostream& outs, Parser& print_me){
            return print_me.print_parser(outs);
        }

    private:

        //-----------------------------------------------------------------//
        vector<Token> _input;                   //users input of tokens
        map_keywords _keywords;                 //map for lookup for tokens
        mmap_ss _parseTree;                     //multimap of parsed commands
        int _table[PARSER_ROWS][PARSER_COLUMNS];
                                                //adjacency matrix
        map_states _concatenation_states;       //map of states to concat.
        //------------------------------------------------------------------//

        //i/o helper
        void toLower(char input_str[]);         //converts string toLower

        //filers
        void fill_vector(char string[]);        //fill the input vector
        void fill_keywords();                   //maps keywords
        void insert_tokenType(Token t);         //insert token's type only
                                                //into keywords map
        int get_column(Token t);                //get a tokens column value in
                                                //the state machine

        bool build_parseTree();                 //builds a parse tree from str
                                                //fnc will return false if
                                                //parse tree has failed to build

        bool insert_key(int &last_key, int column, string token_str,
                        string &quoted_token);
                                                //inserts a key into the
                                                //parsed tree, will return
                                                //false if failed to insert

        //checks string for certain characteristics
        bool isrelationaloperator(string s);
        bool isvalue(string s);

        void make_table();                      //builds an adjancency matrix

        void map_concatenation_states();        //maps constant values

        ostream& print_parser(ostream& out = cout);
};

#endif // PARSER_H
