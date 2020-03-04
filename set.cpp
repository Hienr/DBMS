#include "set.h"

Set::Set(vector<string> _expression, mmap_indices indices, keyword_map keymap){
    const bool debug = false;

    if (debug) cout << "Set Constructor Fired.\n";
    convert_to_queue(_expression);                  //convert vector to queue
    if (debug) cout << "Set() : infix :\n" << _expression << endl;

    get_postfix();                              //get RPN
    if (debug) cout << "Set() : postfix :\n" << _postfixQ << endl;

    _indices = indices;
    if (debug) cout << "Set() : indices :\n" << _indices << endl;

    _keymap = keymap;
    if (debug) cout << "Set() : keymap :\n" << _keymap << endl;
}

vector<long> Set::eval(){
    const bool debug = false;
    if (debug){
        cout << string(75,'*') << endl << setw(30) << " Eval Trace " << setw(30)
             << endl << string(75,'*') << endl;
    }

    /* a where query will have a general pattern
     * where [FIELD] [RELATIONAL OPERATOR] [VALUE] [LOGICAL OPERATOR
     * so we need three stacks to evaluate the query. */

    Stack<string> field_stack;
    Stack<string> value_stack;
    Stack<vector<long >> vec_stack;

    while(!_postfixQ.empty()){

        if (debug) cout << "evaluating " << _postfixQ.front() << endl;

        /* CASE 1: FRONT OF THE RPN QUEUE IS AN OPERAND */
        if (isOperand(_postfixQ.front())){

            if (debug) cout << _postfixQ.front() << " is an operand\n";

            /* CASE 1A: Front of the RPN Queue is contained in the mapped
             * fields. This can only mean the token must be a field name */
            if (_keymap.contains(_postfixQ.front()))
                field_stack.push(_postfixQ.pop());
            else {
            /* CASE 1B: Front of the RPN Queue is a value */
                value_stack.push(_postfixQ.pop());
            }

            if (debug) cout << _keymap << endl;

            if (debug)
                            cout << "f s :: " << field_stack
                            << endl
                            << "v s :: " << value_stack << endl;
        }
        else if (!isRelational(_postfixQ.front())){

        /* Case 2: Is an Operator
         * call an operator on the field and key at the top of the stack
         * to find the matching indices
         */

            string op = _postfixQ.pop(),
                   field = field_stack.pop(),
                   key = value_stack.pop();

            if (debug) cout << "Popped op " << op << endl
                            << "Popped field " << field << endl
                            << "Popped key " << key << endl;

            vector<long> list;

            if (op == "=")
                list = equal(field, key);
            else if (op == ">")
                list = greaterThan(field, key);
            else if (op == "<")
                list = lessThan(field, key);
            else if (op == ">=")
                list = greaterThanEqual(field, key);
            else if (op == "<=")
                list = lessThanEqual(field, key);

            /* push vector of needed indices onto the vector stack */
            vec_stack.push(list);

            if (debug) cout << "vs : "  << vec_stack << endl;
        }
        else{

            /* CASE 3 : Combine Vectors
             * Because of the pattern, AND/OR must occur after a
             * [field] [relational operator] [value] [AND/OR]
             * the vector stack cannot be empty when combining the vectors */

            assert(!vec_stack.empty());

            string token_str = _postfixQ.pop();

            if (debug) cout << "eval() : performing on " << token_str << endl;

            vector<long> v1, v2;

            if (debug) cout << "vector stack < " << vec_stack << endl;

            v1 = vec_stack.pop();                   //get two vectors
            v2 = vec_stack.pop();

            /* [and] will find the intersection of two vectors
             * i.e all the indices that match
             * [or] will find the union of two vectors
             * i.e merge them in sorted order */

            if (debug) cout << "Sorting the two vectors " << endl;
            std::sort(v1.begin(),v1.end());
            std::sort(v2.begin(),v2.end());

            if (debug) cout << "Performing a " << token_str << " on\n" <<
                            v1 << endl << v2 << endl;

            if (token_str == "and")                 //determine which op
                vec_stack.push(intersect(v1, v2));
            else
                vec_stack.push(merge(v1,v2));

            if (debug)
                            cout << "f s :: " << field_stack
                            << endl
                            << "v s :: " << value_stack << endl;
        }
    }

    /* Final Step: when all stacks have been popped, the resulting vector
     * will be in the vec_stack */

    //field stack & value stack MUST BE EMPTY for evaluations to be completed
    if (!field_stack.empty() && !value_stack.empty())
        throw (UNABLE_TO_EVAL);
    //assert(field_stack.empty() && value_stack.empty() && "EVAL NOT EMPTY");

    if (debug){
        cout << string(75,'*') << endl << setw(30) << " TRACE END " << setw(30)
             << endl << string(75,'*') << endl;
    }

    if (debug) cout << "Evaluated Vector : " << vec_stack.top() << endl;

    return vec_stack.pop();
}

void Set::convert_to_queue(vector<string> expression){
    const bool debug = false;
    string token;

    for (int i = 0; i < expression.size(); i++){
        token = expression[i];

        _expression.push(token);

        if (debug) cout << "Queue Status : " << _expression << endl;
    }
}

vector<long> Set::merge(vector<long> v1, vector<long> v2){
    const bool debug = false;
    if (debug) cout << "taking the union of 2 vector \n"
                    << v1 << endl << v2 << endl;

    vector<long> v3;

    std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                   inserter(v3, v3.begin()));

    if (debug) cout << "union : " << v3 << endl;

    return v3;
}

vector<long> Set::intersect(vector<long> v1, vector<long> v2){
    const bool debug = false;

    if (debug) cout << "intersect of "
                    << "vector 1 : " << v1 << endl
                    << "vector 2 : " << v2 << endl;

    vector<long> return_vec;

    std::set_intersection(v1.begin(), v1.end(),
                          v2.begin(), v2.end(),
                          std::back_inserter(return_vec));

    if (debug) cout << "set_intersected vector : " << return_vec << endl;
    return return_vec;
}

void Set::get_postfix(){
    const bool debug = false;

    if (debug) cout << "infix :: " << _expression << endl;

    RPN converter(_expression);             //get an rpn version of our queue
    _postfixQ = converter.get_postfix();

    if (debug) cout << "postfix() : " << _postfixQ << endl;
}

vector<long> Set::equal(string field, string key){
    /* returns the indices that are equal to field/key */
    const bool debug = false;

    vector<long> return_vec = _indices[_keymap[field]][key];

    if (debug) cout << "equal() : " << return_vec << endl;

    return return_vec;
}

vector<long> Set::lessThan(string field, string key){
    /* returns the indices that are lessThan to field/key */
    const bool debug = false;

    if (debug) cout << "lessThan()\n";

    vector<long> return_vec;

    /* i.e. all keys less than 15
     * begin()
     * v
     * 1 2 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
     * ================================
     *                                ^
     *                              lower_bound()
     */

    for(MMap<string, long>::Iterator it = _indices[_keymap[field]].begin();
        it != _indices[_keymap[field]].lower_bound(key)
        && it != _indices[_keymap[field]].end(); it++){
        MPair<string, long> mpair = *it;

        if (debug) cout << mpair.value_list << endl;

        return_vec = merge(return_vec, mpair.value_list);
    }

    return return_vec;
}

vector<long> Set::greaterThan(string field, string key){
    /* returns indices greater than field/key */
    const bool debug = false;

    if (debug) cout << "greater than " << key << endl;

    vector<long> return_vec;

    /* i.e. all keys greater than 15
     *                            upper_bound()
     *                                    v
     * 1 2 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
     *                                    ============================
     *                                                               ^
     *                                                              end()
     */

    for(MMap<string, long>::Iterator it = _indices[_keymap[field]].upper_bound(key);
        it != _indices[_keymap[field]].end(); it++){
        MPair<string, long> mpair = *it;

        if (debug) cout << mpair.key << endl;

        return_vec = merge(return_vec, mpair.value_list);
    }

    return return_vec;
}

vector<long> Set::lessThanEqual(string field, string key){
    /* returns indices less than or equal to field/key */
    const bool debug = false;

    if (debug) cout << "lessThanEqual()" << endl;

    vector<long> return_vec;

    /* i.e. all keys less than/= 15
     * begin()
     * v
     * 1 2 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
     * ================================
     *                                ^
     *                              lower_bound()
     */
    for(MMap<string, long>::Iterator it = _indices[_keymap[field]].begin();
        it != _indices[_keymap[field]].upper_bound(key)
        && it != _indices[_keymap[field]].end(); it++){
        MPair<string, long> mpair = *it;

        return_vec = merge(return_vec, mpair.value_list);
    }

    return return_vec;
}

vector<long> Set::greaterThanEqual(string field, string key){
    /* returns indices greater than or equal to field/key */

    vector<long> return_vec;
    /* i.e. all keys greater than 15
     *                            upper_bound()
     *                                    v
     * 1 2 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
     *                                    ============================
     *                                                               ^
     *                                                              end()
     */
    for(MMap<string, long>::Iterator it = _indices[_keymap[field]].lower_bound(key);
        it != _indices[_keymap[field]].end(); it++){
        MPair<string, long> mpair = *it;

        return_vec = merge(return_vec, mpair.value_list);
    }

    return return_vec;
}

bool Set::isOperand(string t){
    if (!isRelational(t))
        if(!isOperator(t))
            return true;
    return false;
}

bool Set::isRelational(string t){
    if (t == "and")
        return true;
    else if (t == "or")
        return true;
    return false;
}

bool Set::isOperator(string t){
    const bool debug = false;

    switch(t[0]){
        case '=' :
        case '>' :
        case '<' :
            if (debug) cout << t << " was found to be an operator\n";
            return true;
        default : return false;
    }
}

void Set::print_records(vector<long> indices, string binfile){
    fstream ff;
    Record record;

    open_fileRW(ff, binfile.c_str());

    for (int i = 0; i < indices.size(); i++){
        record.read(ff, indices[i]);

        cout << "\n[" << i << "] : \n" << record;
    }

}
