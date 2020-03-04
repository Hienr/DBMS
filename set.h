/* Set Class
 * Class will take in an expression, a table of indices, and the map to the
 * field names. This will help evaluate an SQL Where Query
 */
#ifndef SET_H
#define SET_H
#include <vector>
#include <iostream>
#include <algorithm>
#include "token.h"
#include "stokenizer.h"
#include "fnc_array.h"
#include "rpn.h"
#include "record.h"
#include "fnc_file.h"
#include "mmap.h"
#include "w_exceptions.h"

using namespace std;

class Set{
    /* type definitions */
    typedef vector<MMap<string, long> > mmap_indices;
    typedef Map<string, int> keyword_map;

    public:
        /* Constructor */
        Set(vector<string> _expression,
            mmap_indices indices, keyword_map keymap);

        /* returns the indices to an evaluated where query */
        vector<long> eval();

    private:
        mmap_indices _indices;
        keyword_map _keymap;

        Queue<string> _expression;               //expression is in infix
        Queue<string> _postfixQ;                 //postfix expression

        //operations to help evaluate query
        vector<long> merge(vector<long> v1, vector<long> v2);
        vector<long> intersect(vector<long> v1, vector<long> v2);
        vector<long> equal(string field, string key);
        vector<long> lessThan(string field, string key);
        vector<long> greaterThan(string field, string key);
        vector<long> lessThanEqual(string field, string key);
        vector<long> greaterThanEqual(string field, string key);

        vector<long> ordered_merge(vector<long> v1, vector<long> v2);

        //token checker
        bool isRelational(string t);
        bool isOperand(string t);
        bool isOperator(string t);

        void convert_to_queue(vector<string> expression);
        void get_postfix();

        /* debug */
        void print_records(vector<long> indices, string binfile);
};

#endif // SET_H
