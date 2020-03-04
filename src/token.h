#ifndef TOKEN_H
#define TOKEN_H
#include <string>

using namespace std;

class Token
{
    public:
        Token();
        Token(string str, int type);
        friend ostream& operator <<(ostream& outs, const Token& t){
            outs << t._token;
            return outs;
        }
        int type();                 //Returns the type of token
        string type_string();       //Returns the token type
        string token_str();         //Returns the token

        void set_string(string str);    //Set _token
        void set_type(int type);        //Set _type
    private:
        string _token;              //Will store a string token
        int _type;                  //Will store the type of the string
};
#endif // TOKEN_H
