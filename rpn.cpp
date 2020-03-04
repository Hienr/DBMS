#include "rpn.h"

RPN::RPN(Queue<string> infix){
    const bool debug = false;

    if (debug) cout << "RPN() : infix >> " << infix << endl;
    _source = infix;
    build_postfix();

    if (debug) cout << _output << endl;
}

void RPN::build_postfix(){  //utilize simplified shunting yard
    const bool debug = false;

    /* the main goal of this function is to create a postfix for evaluations.
     * Will mainly be using a stack and queue -- the stack for operators,
     * and the queue for the final postfix */

    if (debug) cout << "||| building postfix with initial Q: "
                    << _source << endl
                    << "||| postfix : " << _output << endl;

    /* While the infix source is NOT empty pop one token.
     * If the token is an operand then it needs to be pushed into the output
     * queue as they serve no purpose when converting to postfix.
     *
     * If the token is an operator however it must do one of things :
     *      pushing an operator into the stack or output queue
     *  in order to figure this out, we check the top of the stack. If the
     *  stack is empty then simply push in the held token. If the stack
     * is not empty, we must either swap the tokens or push into queue.
     *
     * You may only push tokens into the stack as long as the token on
     * the top's precedence is >=
     *
     * At the end, any remaining operators are pushed into the output queue */

    while (!_source.empty()){
        string t = _source.pop();

        if (isOperand(t)){
            if (debug) cout << t << " was found to be an operand\n";
            _output.push(t);
        }
        if (isOperator(t)){
            if (debug) cout << t << " was found to be an operator\n";

            if (!_operators.empty()){
                string top_token = _operators.top();

                if (debug) cout << "comparing " << get_precedence(top_token)
                                << " to " << get_precedence(t);

                while ((!_operators.empty())
                       && (get_precedence(top_token) >= get_precedence(t))){

                    _output.push(_operators.pop());

                    if (!_operators.empty()){;
                        top_token = _operators.top();
                    }
                    if (debug && "Stack/Queue Tracker")
                                    cout << " \ns : " << _operators
                                    << endl
                                    << " \nq : " << _output
                                    << endl;
                }
            }

            if (debug) cout << t
                            << " was found to be an operator, so pushing\n";
            _operators.push(t);

        }
        if (debug && "Stack/Queue Tracker")
                        cout << " s : " << _operators
                        << endl
                        << " q : " << _output
                        << endl;
    }

    if (_source.empty()){
        while (!_operators.empty())
            _output.push(_operators.pop());
    }

    if (debug) cout << "||| built the postfix : " << _output << endl;
}

bool RPN::isOperand(string t){
    bool operand = (isOperator(t)) ? false : true;
    return operand;
}

bool RPN::isOperator(string token_str){
    const bool debug = false;

    if (token_str == "and"){
        if (debug) cout << token_str << " was found to be an operator\n";
        return true;
    }
    else if (token_str == "or"){
        if (debug) cout << token_str << " was found to be an operator\n";
        return true;
    }
    else{
        switch(token_str[0]){
            case '=' :
            case '>' :
            case '<' :
                if (debug) cout << token_str << " was found to be an operator\n";
                return true;
            default : return false;
        }
    }
}

int RPN::get_precedence(string token_str){
    const bool debug = false;

    if (debug) cout << "get_precedence()" << endl;
    /* ensure t is an operator in order to get precedence */
    assert(isOperator(token_str));

    if (token_str == "and")
        return AND_OP;
    else if (token_str == "or")
        return OR_OP;
    else
        return RELATIONAL_OP;
}


Queue<string> RPN::get_postfix(){
    return _output;
}
