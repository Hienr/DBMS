#ifndef TABLE_H
#define TABLE_H
#include "map.h"
#include "mmap.h"
#include "record.h"
#include "parser.h"
#include "fnc_file.h"
#include "filesignature.h"
#include "set.h"
#include "w_exceptions.h"
#include <vector>

class Table{
    /* type definitions */
    typedef vector<MMap<string, long> > mmap_indices;
    typedef Map<string, int> keyword_map;

    public:

        /* Constructors
         * [1] to load an existing table
         * [2] to create a new table */

        Table();
        Table(string tableName);
        Table(string tableName, vector<string> field_names);

        // [1] CREATE TABLE <TABLE NAME> FIELDS <FIELDS...>
        // [2] MAKE TABLE <TABLE NAME> FIELDS <FIELDS...>
        void create_table(string tableName, vector<string> field_names);

        // [1] INSERT INTO <TABLE NAME> VALUES <VALUES...>
        void insert_into(vector<string> values);

        //ctor call to load pre-existing table
        void load_existing(string tableName);

        /* [1] SELECT * FROM <TABLE NAME>
         * [2] SELECT * FROM <FIELDS . . . > WHERE <QUERY>
         * [3] SELECT <FIELDS ... > FROM <TABLE NAME>
         * [4] SELECT <FIELDS . . .> FROM <TABLE NAME> WHERE <QUERY> */
        Table select_all();
        Table select_all(vector<string> conditions);
        Table select(vector<string> field_names);
        Table select(vector<string> field_names, vector<string> conditions);

        void drop();                //clean up the temporary tables

        string get_table_name();
        vector<string> get_field_names();

        void print_table(ostream& outs = cout);
        friend ostream& operator<<(ostream& outs, Table print_me){
            print_me.print_table(outs);
            return outs;
        }

        /* debug */
        void print_maps();
        void print_current_indices(string msg);
        void print_record_vec(vector<Record> records);
        void print_records(vector<long> recnos);

    private:
        mmap_indices _indices_list;
        keyword_map _map;
        vector<string> _current_fields;
        string _tableName;

        //index list helpers
        bool index(fstream &ff, int record, long &pos);
        void reindex();

        //returns a name of a file on DISK
        string get_binFile_onDisk();
        string get_fieldFile_onDisk();

        //returns a generated file name WITHOUT EXTENSION
        string get_selectionFile(string current, vector<string> fields);
        string get_whereFile(string current, vector<string> fields,
                             vector<string> expression);

        //_keyword  map helper
        void map_fields();
        int get_index(string find_me);

        //_current_fields vector helper
        void fill_current_fields(vector<string> fields);
        void fill_current_fields_from_file(ifstream &in);

        //writers
        void write_fields(vector<string> field_names);
        void generate_bin();

        //validators
        bool do_fields_match(vector<string> src, vector<string> fields);
};
#endif // TABLE_H
