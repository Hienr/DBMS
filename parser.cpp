#include "parser.h"
#include <ctype.h>
#include "stokenizer.h"
#include "w_exceptions.h"
#include "w_parser_constants.h"
#include "w_table_constants.h"
#include "fnc_state_machine.h"
#include <string.h>

/* type definitions */
typedef MMap<string, string> mmap_ss;
typedef Map<string, int> map_keywords;


Parser::Parser(char input[]){
    const bool debug = false, print = false;

    _input.clear();

    /* initialize table */
    init_table(_table);
    make_table();
    if (debug) print_table(_table);

//    toLower(input);
    if (debug) cout << "toLower() >> " << input << endl;

    fill_vector(input);
    if (debug) cout << "fillVector() >> " << _input << endl;

    fill_keywords();
    if (debug) cout << "\nkeywords() >> \n" << _keywords << endl;

    if (!build_parseTree())
        throw (INVALID_COMMAND);

    if (debug || print) cout << _parseTree << endl;
}

mmap_ss Parser::get_parseTree(){
    return _parseTree;
}

map_keywords Parser::get_keywords(){
    return _keywords;
}

void Parser::fill_vector(char input[]){
    const bool debug = false;
    if (debug) "|| fill_vector\n";

    STokenizer stk(input);

    Token token;
    stk >> token;

    while(stk.more()){
        if (token.type() != UNKNOWN){
            if (debug) cout << "fill_vector() : pushing token [" << token << "] "
                            << endl;

            _input.push_back(token);
        }
        token = Token();
        stk >> token;
    }

    if (debug) cout << _input << endl;
}

void Parser::fill_keywords(){
    const bool debug = false;
    if (debug) cout << "|| fill_keywords\n";

    for (int i = 0; i < _input.size(); i++){
        if (debug) cout << "fill_keywords() : inserting " << _input[i] << endl;
        insert_tokenType(_input[i]);
    }

    if (debug) cout << _keywords << endl;
}

void Parser::insert_tokenType(Token t){
    const bool debug = false;
    if (debug) cout << "|| inserting_tokenType\n";

    /* taking a token, and mapping it to a specific value is hard coded,
     * emulates a switch(){} */

    /* SQL commands include:
     * create table TABLE_NAME fields FIELD_NAMES...
     * insert int TABLE_NAME values VALUES....
     * select * from TABLE_NAME */

    string token_str = t.token_str();

    if (token_str == "select")          /** SELECT COMMAND **/
        _keywords[token_str] = SELECT;
    else if (token_str == "*")
        _keywords[token_str] = ASTERISK;
    else if (token_str == "from")
        _keywords[token_str] = FROM;
    else if (token_str == "where")
        _keywords[token_str] = WHERE;
    else if (token_str == "insert")     /** INSERT COMMAND **/
        _keywords[token_str] = INSERT;
    else if (token_str == "into")
        _keywords[token_str] = INTO;
    else if (token_str == "values")
        _keywords[token_str] = VALUES;
    else if(token_str == "create" || token_str == "make")
        _keywords[token_str] = CREATE;  /** CREATE COMMAND **/
    else if (token_str == "table")
        _keywords[token_str] = TABLE;
    else if(token_str == "fields")
        _keywords[token_str] = FIELDS;
    else if (token_str == "batch")
        _keywords[token_str] = BATCH;
    else if (isrelationaloperator(token_str))
        _keywords[token_str] = RELATIONAL_OPERATOR;
    else if (token_str == "and")
        _keywords[token_str] = AND;
    else if (token_str == "or")
        _keywords[token_str] = OR;
    else if (token_str == ",")
        _keywords[token_str] = COMMA;
    else if (token_str == " ")
        _keywords[token_str] = BLANK;
    else if (token_str == "\"")
        _keywords[token_str] = QUOTE;
    else {
        _keywords[token_str] = SYMBOL;
    }



    if (debug) cout << "|| inserted : " << _keywords[token_str] << endl;
}

int Parser::get_column(Token t) {
    const bool debug = false;

    string token_str = t.token_str();
    if (_keywords.contains(Pair<string, int>(token_str, t.type()))){
        if (debug) cout << "get_columns() : Returning " << _keywords[token_str]
                        << " from " << token_str << endl;
        return _keywords[token_str];
    }
    else{
        if (debug) cout << "get_columns() : Returning SYMBOL for " << token_str
                        << endl;
        return SYMBOL;
    }
}

bool Parser::build_parseTree(){
    const bool debug = false;
    if (debug) cout << "|| build_parseTree\n";

    int state = 0, last_state = 0, last_key = 0, column = 0;
    string quoted_token = "";

    /* Loop through the vector */
    for (int i = 0; i < _input.size(); i++){
        if (debug) cout << "|| vector at " << i << " :: " << _input[i] << endl;

        /* at each iteration get the next token */
        Token token = _input[i];

        /* set strings and a value for the grabbed token */
        string token_str = token.token_str();
        int token_type = token.type();


        //see if token_str is in the keyword map, if not it's a symbol, otherwise,
        //reutn keywords[token_str]
        column = get_column(Token(token_str, token.type()));

        /* record previous state */
        last_state = state;
        /* determine the token's state */
        state = _table[state][column];

        /* if token's state was a FAIL, end */
        if (state == FAIL){
            if (debug) cout << token_str << " failed.\n";
            if (debug) assert(true && "Fail State Reached. CHECK COMMAND.\n");

            if (token_str == " "){
                /* last state is recorded in order to provide a false-positive
                 * state, its previous state to keep going */

                state = last_state;
            }
            else{
                if (debug) cout << "Error: Invalid Command\n" << endl;

                throw (INVALID_COMMAND);
                return false;
            }
        }

        if(!insert_key(last_key, column, token_str, quoted_token))
            if (debug) cout << "|| " << token_str << " failed to insert.\n";
    }

    if (debug) cout << state <<endl << " @ " << _table[state][0] << endl;
    if (_table[state][0] == 1){
        if (debug) cout << "build_parseTree() : true " << endl;
        return true;
    }
    else {
        if (debug) cout << "build_parseTree() : false" << endl;
        return false;
    }
}

bool Parser::insert_key(int &last_key, int column, string token_str,
                        string &quoted_token){
    const bool debug = false;

    if (debug) cout << "|| Determining " << token_str << " with v: " << column
                    << endl;

    switch (column){
        case COMMA :
        case BLANK :
            if (last_key == QUOTE)
                quoted_token += token_str;
            break;                              //end immediately if blank
        case SELECT:
        case INSERT:
        case CREATE:
        case MAKE :
            _parseTree["Command"] += token_str;
            last_key = column;
            break;
        case BATCH :
        case FROM :
        case FIELDS :
        case TABLE :
        case INTO:
        case VALUES:
        case WHERE:
            last_key = column;
            break;
        case ASTERISK :
            _parseTree["Fields"] += token_str;
            break;
        case RELATIONAL_OPERATOR :
        case AND :
        case OR :
            _parseTree["Expression"] += token_str;
            last_key = column;
            break;
        case QUOTE :
            if (last_key == QUOTE && _parseTree["Command"][0] == "insert"){
                //if quote add concatenation
                _parseTree["Values"] += quoted_token;
                quoted_token.clear();               //clear quoted token
                last_key = VALUES;                  //reset last key
            }
            else if (last_key == QUOTE && _parseTree["Command"][0] == "select"){
                _parseTree["Expression"] += quoted_token;

                quoted_token.clear();               //clear quoted token
                last_key = VALUES;                  //reset last key
            }
            else
                last_key = column;
            break;
        case SYMBOL:
            switch (last_key){
                case SELECT:
                case FIELDS:
                    _parseTree["Fields"] += token_str;
                    break;
                case WHERE:
                case RELATIONAL_OPERATOR :
                case AND :
                case OR:
                    _parseTree["Expression"] += token_str;
                    break;
                case TABLE:
                case FROM:
                case INTO:
                    _parseTree["Table Name"] += token_str;
                    break;
                case VALUES:
                    _parseTree["Values"] += token_str;
                    break;
                case BATCH:
                    _parseTree["Batch"] += token_str;
                    break;
                case QUOTE:
                    quoted_token += token_str;
                    break;
                default :
                    if (debug) cout << token_str << " symbol was not recognized\n";
            }
            break;
    default :
                if (debug) cout << token_str << " was not inserted\n";
                return false;
    }
    return true;
}

bool Parser::isrelationaloperator(string s){
    switch (s[0]){
        case '<':
        case '>':
        case '=':
            return true;
        default : return false;
    }
}

bool Parser::isvalue(string s){
    bool isValue = ((s[0] == '\"') && (s[s.back()] == '\"')) ? true : false;
    return isValue;
}

void Parser::make_table(){
    /* Success/Nonsuccess states will be MARKED */

    mark_fail(_table, ROW0); //Row 0 will fail if no other tokens is inputted

    /* ROW 0 */ //Dictates which machine to enter
    mark_cell(ROW0, _table, CREATE, GOSTATE1);
    mark_cell(ROW0, _table, MAKE, GOSTATE1);
    mark_cell(ROW0, _table, SELECT, GOSTATE11);
    mark_cell(ROW0, _table, INSERT, GOSTATE25);
    mark_cell(ROW0, _table, BATCH, GOSTATE30);

    // --------------------- CREATE COMMAND MACHINE -------------------------//
    /* NONSUCCESS STATES until a completed command is INPUTTED */
    mark_fail(_table, ROW1);
    mark_fail(_table, ROW2);
    mark_fail(_table, ROW3);
    mark_fail(_table, ROW4);
    mark_success(_table, ROW5);
    mark_fail(_table,ROW6);
    // ---------------------------------------------------------------------//

    /* ROW 1 */
    mark_cell(ROW1, _table, TABLE, GOSTATE2);
    /* ROW 2 */
    mark_cell(ROW2, _table, SYMBOL, GOSTATE3);
    /* ROW 3 */
    mark_cell(ROW3, _table, FIELDS, GOSTATE4);
    /* ROW 4 */
    mark_cell(ROW4, _table, SYMBOL, GOSTATE5);

    /* ROW 5 */
    mark_cell(ROW5, _table, SYMBOL, GOSTATE5);
    mark_cell(ROW5, _table, COMMA, GOSTATE6);

    /* ROW 6 */
    mark_cell(ROW6, _table, SYMBOL, GOSTATE5);
    /* ROW 7 */
    /* ROW 8 */
    /* ROW 9 */
    /* ROW 10 */

    // ---------------------- SELECT COMMAND MACHINE ------------------//
    /* NONSUCCESS STATES until a completed command is INPUTTED */
    mark_fail(_table, ROW11);               //select *
    mark_fail(_table, ROW12);               //select * from
    mark_fail(_table, ROW13);               //select * from <TABLENAME>
    mark_fail(_table, ROW14);
    mark_fail(_table, ROW15);
    mark_success(_table, ROW16);
    mark_fail(_table, ROW17);
    mark_fail(_table, ROW18);
    mark_fail(_table, ROW19);
    mark_success(_table, ROW20);
    mark_fail(_table, ROW21);
    mark_fail(_table, ROW22);
    // ---------------------------------------------------------------------//
    /* ROW 11 */
    mark_cell(ROW11, _table, SYMBOL, GOSTATE12);
    mark_cell(ROW11, _table, ASTERISK, GOSTATE14);
    /* ROW 12 */
    mark_cell(ROW12, _table, SYMBOL, GOSTATE12);
    mark_cell(ROW12, _table, COMMA, GOSTATE13);
    mark_cell(ROW12, _table, FROM, GOSTATE15);

    /* ROW 13 COMMA LOOP */
    mark_cell(ROW13, _table, SYMBOL, GOSTATE12);

    /* ROW 14 ASTERISK */
    mark_cell(ROW14, _table, FROM, GOSTATE15);

    /* ROW 15 */
    mark_cell(ROW15, _table, SYMBOL, GOSTATE16);

    /* ROW 16 */
    mark_cell(ROW16, _table, WHERE, GOSTATE17);

    /* ROW 17 */
    mark_cell(ROW17, _table, SYMBOL, GOSTATE18);

    /* ROW 18 */
    mark_cell(ROW18, _table, RELATIONAL_OPERATOR, GOSTATE19);

    /* ROW 19 */
    mark_cell(ROW19, _table, SYMBOL, GOSTATE20);
    mark_cell(ROW19, _table, QUOTE, GOSTATE21);

    /* ROW 20 SUCCESS STATE */
    mark_cell(ROW20, _table, SYMBOL, GOSTATE20);

    /* only exit point is a logical operator to restart loop */
    mark_cell(ROW20, _table, AND, GOSTATE17);
    mark_cell(ROW20, _table, OR, GOSTATE17);

    /* ROW 21 QUOTE STATE */
    mark_cell(ROW21, _table, SYMBOL, GOSTATE21);
    mark_cell(ROW21, _table, COMMA, GOSTATE21);
    mark_cell(ROW21, _table, BLANK, GOSTATE22);

    /* loop back to success state */
    mark_cell(ROW21, _table, QUOTE, GOSTATE20);

    /* ROW 22 COMMA STATE */
    mark_cell(ROW22, _table, SYMBOL, GOSTATE21);
    mark_cell(ROW22, _table, QUOTE, GOSTATE20);


    /* 23-24 Filler */

    // ---------------------- INSERT COMMAND MACHINE ------------------//
    /* NONSUCCESS STATES until a completed command is INPUTTED */
    mark_fail(_table, ROW25);
    mark_fail(_table, ROW26);
    mark_fail(_table, ROW27);
    mark_fail(_table, ROW28);
    mark_success(_table, ROW29);
    mark_fail(_table, ROW30);
    mark_fail(_table, ROW31);
    mark_fail(_table, ROW32);

    // -----------------------------------------------------------------//

    /* INSERT COMMAND MACHINE */
    /* ROW 25 */
    mark_cell(ROW25, _table, INTO, GOSTATE26);
    /* ROW 26 */
    mark_cell(ROW26, _table, SYMBOL, GOSTATE27);
    /* ROW 27 */
    mark_cell(ROW27, _table, VALUES, GOSTATE28);

    /* ROW 28 */
    mark_cell(ROW28, _table, SYMBOL, GOSTATE29);
    mark_cell(ROW28, _table, QUOTE, GOSTATE31);

    /* ROW 29 SUCCCESS STATE */
    mark_cell(ROW29, _table, SYMBOL, GOSTATE29);
    mark_cell(ROW29, _table, COMMA, GOSTATE30);
    mark_cell(ROW29, _table, QUOTE, GOSTATE31);

    /* ROW 30 COMMA STATE */
    mark_cell(ROW30, _table, SYMBOL, GOSTATE29);
    mark_cell(ROW30, _table, QUOTE, GOSTATE31);

    /* ROW 31 QUOTE STATE */
    mark_cell(ROW31, _table, SYMBOL, GOSTATE31);

    mark_cell(ROW31, _table, BLANK, GOSTATE32);
    mark_cell(ROW31, _table, COMMA, GOSTATE31);

    /* loop back to success state */
    mark_cell(ROW31, _table, QUOTE, GOSTATE29);

    /* ROW 32 BLANK STATE FROM QUOTE */
    mark_cell(ROW32, _table, BLANK, GOSTATE32);

    /* loop backward 1 step */
    mark_cell(ROW32, _table, SYMBOL, GOSTATE31);

    /* loop back to success state */
    mark_cell(ROW32, _table, QUOTE, GOSTATE29);

    /* 33-34 Filler */

    // ---------------------- BATCH COMMAND MACHINE ---------------------//
    /* NONSUCCESS STATES until a fileName is given */
    mark_fail(_table, ROW35);
    mark_success(_table, ROW36);
    // -----------------------------------------------------------------//

    /* ROW 30 */
    mark_cell(ROW35, _table, SYMBOL, GOSTATE36);

    /* ROW 31 IS SUCCESS, LEAVE. */

}

ostream& Parser::print_parser(ostream& out){
    out << _input;
    return out;
}

void Parser::toLower(char input_str[]){
    for (int i = 0; input_str[i]; i++)
        input_str[i] = tolower(input_str[i]);
}

void Parser::map_concatenation_states(){
    _concatenation_states[27] = "insert";

}

