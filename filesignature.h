#ifndef FILESIGNATURE_H
#define FILESIGNATURE_H
#include <string>
#include <vector>
#include "queue.h"
#include "map.h"

/* FILE SIGNATURE class keeps file name management uniform to prevent
 * mismanged file names */

using namespace std;

static const int MAX = 80;
static const int FILENAME_CHARLIMIT = 80;

static vector<string> unknowns = {"\\", "/", ":", "*", "?", "\"", "<", ">", "|",
                                  "<=",">="};
static vector<string> knowns = {"forwardS", "backwardS", "colon", "star",
                                "question", "quote", "lessThan", "greaterThan",
                                "line","lessThanEqual","greaterThanEqual"};

class FileSignature{
    public :
        FileSignature(string name);
        FileSignature(string name, vector<string> v1);
        FileSignature(string name, vector<string> v1, vector<string> v2);

        string get_file(string ext);
        static string remove_ext(string str, string ext);

    private :
        string _src;
        Queue<string> _buffer;
        Map<string, string> _unknowns;
        bool is_long;

        void clean_quotes();                    //prevent " in file names

        void push_into_queue(vector<string> v);

        void map_unknowns();
};

#endif // FILESIGNATURE_H
