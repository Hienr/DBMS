#ifndef RECORDER_H
#define RECORDER_H
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <cassert>
#include "fnc_array.h"

using namespace std;

class Record{
    public:
        Record();
        Record(vector<string> input);

        long write(fstream &outs);
        long read(fstream &ins, long recno);

        friend ostream& operator<<(ostream& outs,
                                   const Record& r){
            r.print_record(outs);
            return outs;
        }

        string get_field_data(int field);

        vector<string> vectorize_fields();

        void print_record(ostream& outs = cout) const;

    private:
        static const int MAX_CHARACTERS = 50, MAX_FIELDS = 10;

        long recno;
        char record[MAX_FIELDS][MAX_CHARACTERS];
};
#endif // RECORDER_H
