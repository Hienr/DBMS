#ifndef RPN_H
#define RPN_H
#include "stack.h"
#include "queue.h"
#include "token.h"
#include "map.h"
#include "w_stk_constants.h"

/* shows precedence */
enum operators { NON, OR_OP, AND_OP, RELATIONAL_OP};

class RPN{
    public:
        RPN(Queue<string> infix);                   //must receive a queue
                                                    //of expression in infix

        Queue<string> get_postfix();                //retrieves a postfix

    private:
        //------------------------------------------------------------------//
        Queue<string> _source;
        Stack<string> _operators;
        Queue<string> _output;
        //------------------------------------------------------------------//

        void build_postfix();                       //builds a postfix

        //checks for certain characteristics
        bool isOperand(string t);
        bool isOperator(string token_str);
        int get_precedence(string token_str);
};

#endif // RPN_H
