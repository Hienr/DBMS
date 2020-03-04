#ifndef SQL_H
#define SQL_H
#include<iostream>
#include <ctime>
#include "parser.h"
#include <string.h>
#include "fnc_file.h"
#include "table.h"
#include "w_exceptions.h"

enum possibe_input { COMMAND, NONCOMMAND, DEBUG };
enum command_list { SQL_CREATE, SQL_MAKE, SQL_INSERT, SQL_SELECT, SQL_EXIT };

using namespace std;
class SQL{

    const string FILE_TABLERECORD = "table_record.txt";

    /* type definitions */
    typedef MMap<string, string> MMStringString_td;
    typedef Map<string, int> KeywordMap_td;
    typedef Map<char, int> PossibilitiesMap_td;
    typedef Map<int, string> ExceptionMap_td;
    typedef Map<string, Table> TableMap_td;

    public:
        /* SQL Constructor will either run normally, or batch mode */
        SQL();
        SQL(const char fileName[]);

        void run();                                 //Powers on the database
    private:
        void run_batch(const char fileName[]);      //Powers on batch mode

        bool run_input(char command[], istream &in = cin);

        //------------------------- Private Data ----------------------------//

        //Current Data
        string _command;                            //Current line of command
        MMStringString_td _parsed_command;          //Current parsed command
        int _command_count;

        //Data Stored
        TableMap_td _mapped_tables;                 //Tables stored

        //Maps
        KeywordMap_td _mapped_commads;              //Command Map
        PossibilitiesMap_td _mapped_possibilities;  //Possible commands
        ExceptionMap_td _mapped_exceptions;         //Exceptions Map

        //Recorder
        ofstream _recorder;
        bool _recorder_on;

        bool batch_mode, prompts = true;

        //------------------------------------------------------------------//

        bool execute(ostream& outs = cout);         //Executes parse tree state
        void execute_batch();                       //Executes a batch state

        Table process(int command, string tableName);
                                                    //Processes a command

        void resession();            //Check the need to create

        bool parse(char input[], MMStringString_td &command_tree);
                                                    //Parse a command

        bool get_input(char input[], istream &in = cin) throw (SQL_exceptions);
                                                    //Get user input
        int validate_input(string s);               //Validate input

        void record_table(Table t);
        bool should_drop();
        bool should_record();
        void drop_table(string tableName);

        //construct
        void map_commands();
        void map_possibilities();
        void map_exceptions();
        void map_tables();

        void start_recorder();

        //exceptions
        string get_errorMsg(const char command[], SQL_exceptions e);

        /* parse tree filters */
        int get_command();
        vector<string> get_fields();
        vector<string> get_values();
        vector<string> get_where();
        bool is_batch();
        string get_table();
        bool is_select_all();
        bool is_select_where();

        int get_command_count();

        //file name helpers (in order to maintain filename uniformity)
        string get_selectionFile(string current, vector<string> fields);
        string get_whereFile(string current, vector<string> fields,
                                    vector<string> expression);
        string append_select_files(vector<string> fields);
        string build_batch_file_name();
        bool verify_batch_file(string str);

        //searches disk for table
        bool get_table(string file_name);

        //formatting
        void print_description();
        void print_results(string command, Table table, ostream &outs = cout);
};

#endif // SQL_H
