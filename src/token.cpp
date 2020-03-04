#include "token.h"
#include "w_stk_constants.h"

Token::Token(){
    _token = "";            //Initialize the token to a blank string.
    _type = NONE;           //Initialize the integer to -1 to not match a type.
}

Token::Token(string str, int type){
    //Assign private member variables to passed variables.
    _token = str;
    _type = type;
}

int Token::type(){
    return _type;
}

string Token::token_str(){
    return _token;
}

string Token::type_string(){
    switch(_type){
        case NUMBER: return "NUMBER";
        case ALPHA: return "ALPHA";
        case PUNCT: return "PUNCT";
        case SPACE:   return "SPACE";
        case QUOTATION: return "Q\"MARK";
        case OPERATOR: return "OPERATOR";
        default:  return "UNKNOWN";
    }
}

void Token::set_type(int type){
    _type = type;
}

void Token::set_string(string str){
    _token = str;
}
