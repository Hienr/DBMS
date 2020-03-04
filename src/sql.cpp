#include "sql.h"

SQL::SQL(){

    //base constructor
    print_description();
    const bool debug = false;
    batch_mode = false;

    map_commands();
    map_possibilities();
    map_exceptions();

    /* resession tables previously recorded */
    resession();

    _command_count = 0;

    start_recorder();
    if (debug) cout << "SQL Constructor Fired >> Mode: Interactive\n";
}

SQL::SQL(const char fileName[]){

    //Batch file constructor
    const bool debug = false, drop = false;
    batch_mode = true;

    map_commands();
    map_possibilities();
    map_exceptions();

    /* resession tables previously used */
    resession();

    _command_count = 0;

    if (debug) cout << "SQL Constructor Fired >> Mode: Batch\n";

    start_recorder();

    run_batch(fileName);

    if (drop) drop_table(fileName);

    /* after batch for now run interactive */
    run();
}

void SQL::run(){

                        /* RUN INSTANCE OF PROGRAM */

    const int MAX_BUFFER = 100000;
    const bool debug = false, print = false;

    MMStringString_td command_tree;
    char command[MAX_BUFFER];

    while (true){
        do {

            command_tree = MMStringString_td();
            command[0] = '\0';

                        /* ATTEMPT GETTING INPUT */

            if (!run_input(command))
                assert(true && "! run_input failed critically !\n");

            if (debug) cout << "Command : " << command << endl;

                        /* ATTEMPT TO PARSE INPUT */

        } while (!parse(command, command_tree));    //loop until able to parse

    //successful parses of a command will be saved
    _command = command;
    _parsed_command = command_tree;

                        /* EXECUTE A PARSED INPUT */

    //if the command requested a batch, offload control
    if (is_batch()){
        execute_batch();
    }
    else if (!execute())        //Else EXECUTE ONE PARSED COMMAND
        assert(true && "||| Failed Execution");


    if (print) cout << "Parser() : \n" << command_tree << endl;
    }
}

void SQL::run_batch(const char fileName[]){

                        /* RUNS AN ENTIRE BATCH FILE */

    const int MAX_BUFFER = 100000;
    const bool debug = false, print = false;
    ifstream in;

    batch_mode = true;

    MMStringString_td command_tree;
    char command[MAX_BUFFER];

    if (!file_exists(fileName))
        throw(INVALID_BATCH_FILE);

    open_file(in, fileName);

    if (debug) cout << "||| Batch: Opening file f: " << fileName << endl;

    while (true){

                        /* RUN COMMAND LINE BY LINE */

        try {
            do {

                command_tree = MMStringString_td();
                command[0] = '\0';

                        /* RUNS ONE LINE OF INPUT */

                if (!run_input(command, in))        //attempt getting an input
                    assert(true && "! run_input failed critically !\n");

                if (debug) cout << "Command : " << command << endl;

            } while (!parse(command, command_tree));    //loop until able to parse
        }
        catch (SQL_exceptions e) {
            if (e == STREAM_EOF)
                throw (STREAM_EOF);
        }

        //successful parses of a command will be saved
        _command = command;
        _parsed_command = command_tree;

                        /* EXECUTE COMMAND */

        if (!execute())
            assert(true);


        if (print) cout << "Parser() : \n" << command_tree << endl;

        _command.clear();
        _parsed_command.clear();

    }
}

bool SQL::run_input(char command[], istream &in){
    const bool debug = false;
    bool process = false;

    while (!process){

        if (!batch_mode) cout << "Command : ";

        command[0] = '\n';                              //clear command buffer

        try {                                           //try an input
            if (get_input(command, in))
                process = true;                         //non echo type
            else {
                cout << command << endl;
                _recorder << command << endl;           //echo command
            }
        }
        catch (SQL_exceptions e) {                      //echos will be caught
                                                        //and echoed
            if (debug) cout << "Exceptions caught\n";

            cout << get_errorMsg(command, e) << endl;
            if (e == STREAM_EOF)
                throw(STREAM_EOF);
        }
        /* if command was type echo, will re-ask user for input */
    }

    /* a parseable command will be passed on */
    return true;
}

bool SQL::execute(ostream& outs){

                    /* EXECUTES SINGLE COMMAND */

    const bool debug = false;

    bool postProcess = true;

    /* For a command to be executed
     * determine command, table name, and the need to resession */

    /* Determine the command from the parseTree */
    int command = get_command();
    if (debug) cout << "||| Executing a " << _command << endl;

    /* Determining the name of the table */
    string tableName = get_table();
    if (debug) cout << "||| Grabbing Table Name : " << tableName << endl;

    Table table;
    try{

                /* PROCESSES A COMMAND FOR A TABLE */

        /* Process command and receive the resulting table */
        table = process(command, tableName);
        if (debug) cout << "||| Retrieved table\n";
    }
    catch (SQL_exceptions e){
        /* Failed commands will bounce an error message */

        if (e == UNABLE_TO_PROCESS){
            cout << _mapped_exceptions[UNABLE_TO_PROCESS];
            _recorder << _mapped_exceptions[UNABLE_TO_PROCESS];
        }
        if (e == UNABLE_TO_EVAL){
            cout << _mapped_exceptions[UNABLE_TO_EVAL];
            _recorder << _mapped_exceptions[UNABLE_TO_EVAL];
        }
        if (e == FILE_DNE){
            cout << _mapped_exceptions[FILE_DNE];
            _recorder << _mapped_exceptions[FILE_DNE];
        }
        if (e == FIELDS_MISMATCH){
            cout << _mapped_exceptions[FIELDS_MISMATCH];
            _recorder << _mapped_exceptions[FIELDS_MISMATCH];
        }

        //reset table status
        table = Table();
        postProcess = false;
    }

    /* Only allow post processing if a command was successful.
     * Post processing involves printing the results, recording results if
     * needed to, dropping a table if needed to, incrementing the count of
     * commands currently in session */

    if (postProcess){
        /* show results */
        if (debug) cout << setw(30) << "R E S U L T S : \n";
        print_results(_command, table, outs);

        /* record results if needed */
        if (_recorder_on) print_results(_command, table, _recorder);
        if (debug) cout << "||| Logging. . .\n";

        if (should_drop()){
            if (debug) cout << "||| Drop table is being invoked\n";

            table.drop();
        }
        else {
            if (!_mapped_tables.contains(table.get_table_name() + ".bin"))
                record_table(table);
            if (debug) cout << "||| Recording retrieved table/n";
        }
        /* increment command count */
        _command_count++;
        if (debug) cout << "||| Incrementing command count for this session\n";

    }
    return true;
}

void SQL::execute_batch(){

                        /* BATCH PROGRAM WILL BEGIN NOW */
    try {

        if (debug) cout << "||| isBatch! \n";

                        /* ATTEMPT TO OPEN A BATCH FILE */

        string batch_file = build_batch_file_name();
        if (debug) cout << "||| Built batch file name\n";

                        /* OFFLOAD CONTROL TO RUN_BATCH() */

        run_batch(batch_file.c_str());
    }
    catch (SQL_exceptions e) {

                        /* ERROR HANDLING */

        if (e == INVALID_BATCH_FILE){
            cout << _mapped_exceptions[INVALID_BATCH_FILE];
            _recorder << _mapped_exceptions[INVALID_BATCH_FILE];
        }
        else if (e == STREAM_EOF){
            _recorder << _mapped_exceptions[STREAM_EOF];
            batch_mode = false;
        }
    }

}

Table SQL::process(int command, string tableName){
    /* Will process a single command and expect to return a TABLE with
     * the given specs */

    const bool debug = false;
    if (debug) cout << "||| Processing " << command << endl;

    /* instantiate a table object that will be acted upon */
    Table table;

    switch (command){

        //                                      Invoke Create
        case SQL_CREATE :
        case SQL_MAKE :
            if (debug) cout << "||| >> SQL_Create invoked\n";

            /* if this table is not one that needs to be resessioned - create */
            if (_mapped_tables.contains(tableName + ".bin"))
                table = _mapped_tables[tableName];
            else {
                table = Table(tableName, get_fields());
            }
            /* if table does not need to be resessioned, it already exists */

            break;

        //                                      Invoke Insert
        case SQL_INSERT :
            if (debug) cout << "||| >> SQL_Insert invoked\n";

            /* insert into original bin */
            if (_mapped_tables.contains(tableName + ".bin")){
                table = Table(tableName);

                /* insert into the table object */
                table.insert_into(get_values());
            }
            else {
                throw(UNABLE_TO_PROCESS);   //table needs to EXIST for insertion
            }

            break;

        //                                      Invoke Select
        case SQL_SELECT :

            table = Table(tableName);

            if (is_select_all()){
                if (is_select_where()){         //Case 1 Select All from Where
                    if (debug) cout << "||| SQL_Select * from where invoked.\n";

                    table = table.select_all(get_where());

                }
                else /* !is_select_where() */ { //Case 2 Selct All
                    if (debug) cout << "||| SQL_Select all invoked\n";

                    table = table.select_all();
                }
            }
            else /* !is_selct_all() */{       //Case 3 Select Fields from Where
                if (is_select_where()){
                    if (debug) cout << "|||SQL_Select <fields> from where invoked"
                                    << endl;

                    table = table.select(get_fields(), get_where());

                }
                else /* !is_select_where() */ {//Case 4 Select Fields from
                    if (debug) cout << "SQL_Select <fields> from invoked\n";

                    table = table.select(get_fields());
                }
            }
        }

    return table;
}

void SQL::resession(){

                        /* RESESSION PREVIOUS TABLES */

    const bool debug = false;

    if (debug) cout << "||| Resessioning\n";

    if (file_exists(FILE_TABLERECORD.c_str()))
        map_tables();                           //map existing tables
    else {
        ofstream create_new_file;
        create_new_file.open(FILE_TABLERECORD);
        create_new_file.close();
    }

    if (debug) cout << "||| Resessioned Tables :\n"
                    << _mapped_tables << endl;
}

bool SQL::parse(char input[], MMStringString_td &command_tree){
    /* expect to build a parse tree */

    const bool debug = false;
    const string src(input);

    try {
        if (debug) cout << "||| Parsing command : " << input << endl;
        Parser parse(input);
        command_tree = parse.get_parseTree();

    } catch (SQL_exceptions e) {
        //bounce error message if parse failed

        if (debug) cout << "parse() caught |" << src <<"|"<< endl;

        cout << get_errorMsg(src.c_str(), e) << endl;

        return false;
    }
    return true;
}

int SQL::get_command_count(){
    return _command_count;
}

string SQL::append_select_files(vector<string> fields){
    string append_with;
    for (size_t i = 0; i < fields.size(); i++)
        append_with += fields[i];
    return append_with;
}

string SQL::build_batch_file_name(){
    string batch_file = "";
    vector<string> batch_file_parts = _parsed_command["Batch"];

    for (int i = 0; i < batch_file_parts.size();i++)
        batch_file.append(batch_file_parts[i]);

    if (!verify_batch_file(batch_file))
        throw (INVALID_BATCH_FILE);

    return batch_file;

}

bool SQL::verify_batch_file(string str){
    const bool debug = false;

    size_t found = str.find('.');
    if (found != string::npos){
        if (debug) cout << "||| Found DOT position in batch file.\n";
        return true;
    }
    return false;
}

bool SQL::get_input(char input[], istream& in) throw(SQL_exceptions){
    //ensure input string is parseable

    const bool debug = false;

    if (in.eof()) throw (STREAM_EOF);

    string input_str = "";
    getline(in, input_str);
    strcpy(input, input_str.c_str());

    //if the command type was a terminate, end
    //if the command type was not a command, echo back to user
    //NONCOMMANDs can be any line that begins with
    int command_type = validate_input(input_str);
    if (debug) cout << "Command Type of " << input_str << " to be "
                    << command_type << endl;

    if (batch_mode){
        if (command_type == NONCOMMAND)
            return false;
    }
    else {
        if (command_type == NONCOMMAND)
            return false;
        else if (command_type == TERMINATE)
            exit(1);
    }
    return true;
}

int SQL::validate_input(string s){
    const bool debug = false;
    if (debug) cout << "Validating " << s[0] << endl;

    int command_no = _mapped_possibilities[s[0]];           //check if possible

    return command_no;
}

void SQL::record_table(Table t){
    //writes a bin file to disk of a new table created

    const bool debug = false;

    ofstream recorder(FILE_TABLERECORD, std::ofstream::app);

    if (debug) cout << "Recording \n" << t << endl;

    FileSignature fs(t.get_table_name());
    string name = fs.get_file(".bin");

    //mapping table
    _mapped_tables[name] = t;
    if (debug) cout << "Current Table Map : " << _mapped_tables << endl;

    //write to file
    recorder << (name) << endl;
    if (debug) cout << "Wrote " << fs.get_file(".bin") << endl;
}

bool SQL::should_drop(){
    //lets program know if there is a necessity to drop a current table

    const bool debug = false;
    const int SELECT = 3;

    if (debug) cout << "Parse Tree before drop : \n" << _parsed_command << endl;

    //SQL shall only drop a table if the command was a SELECTION
    /* eg: select <FIELDNAMES ...> from <TABLENAME>
     *     select * from <TABLENAME> where
     *     and any where queries made */
    if (get_command() == SELECT){
        if (get_where().size() > 0){
            if (debug) cout << "||| Parse Tree contains an expression\n"
                            << get_where() << endl;
            return true;
        }
        else if (get_fields().at(0) != "*"){
            if (debug) cout << "||| Parse Tree contains a !*\n"
                            << get_fields() << endl;
            return true;
        }
    }

    return false;
}

//debug to delete table
void SQL::drop_table(string tableName){
    _mapped_tables.erase(tableName);
}

                        /* MAP COMMONLY USED VALUES */

void SQL::map_commands(){
    const bool debug = false;

    const int NUM_COMMANDS = 4;
    const vector<string> commands = {"create","make","insert","select"};

    for (int i = 0; i < NUM_COMMANDS; i++)
        _mapped_commads[commands[i]] = i;

    if (debug) cout << "||| Mapped Commands : \n" << _mapped_commads << endl;
}

void SQL::map_possibilities(){
    const bool debug = false;

    _mapped_possibilities['/'] = NONCOMMAND;
    _mapped_possibilities[' '] = NONCOMMAND;
//    _mapped_possibilities['d'] = DEBUG;
    _mapped_possibilities['x'] = TERMINATE;

    if (debug) cout << "||| Possible Commands : \n"
                    << _mapped_possibilities << endl;
}

void SQL::map_exceptions(){
    const bool debug = false;

    _mapped_exceptions[UNABLE_TO_PROCESS] =
            "||| Could not process an insertion to a table that does\nnot exist";
    _mapped_exceptions[UNABLE_TO_EVAL] = "||| Could not evaluate the conditions\n";
    _mapped_exceptions[FILE_DNE] = "||| A file did not exist when attemping to access\n";
    _mapped_exceptions[FIELDS_MISMATCH] = "||| There was a mismatch in fields\n";
    _mapped_exceptions[INVALID_INPUT] = "||| Invalid Input was inputted\n";
    _mapped_exceptions[INVALID_COMMAND] = "||| Invalid Command was inputted\n";
    _mapped_exceptions[ECHO_INPUT] = "||| Echoing command\n";
    _mapped_exceptions[INVALID_BATCH_FILE] = "||| Invalid batch file.\n";
    _mapped_exceptions[STREAM_EOF] = "||| File reached end of file\n";

    if (debug) cout << "Mapped Exceptions : \n" << _mapped_exceptions << endl;
}

                        /* MAP TABLES FROM A RESESSION */

void SQL::map_tables(){
    const bool debug = false;

    ifstream in(FILE_TABLERECORD);

    while (!in.eof()){
        string fileName = "";
        getline(in, fileName);

        if (in.eof()) break;

        if (debug) cout << "Retrieving table " << fileName << endl;

        string table_name = FileSignature::remove_ext(fileName, ".bin");

        if (file_exists(((fileName).c_str()))){
            Table t(table_name);

            _mapped_tables[fileName] = t;

            if (debug) cout << "Resessioned " << fileName << endl;
        }
    }

    in.close();
}

void SQL::start_recorder(){
    const bool debug = false;
    string session_file = get_current_date() + "_" + get_current_time() +
            " session.txt";
    //naming convention for session texts will be date - time

    if (debug) cout << "||| Recorder started onto : " << session_file << endl;
    _recorder.open(session_file);
}

//get file names - AUTO APPENDS of extensions
string SQL::get_selectionFile(string current, vector<string> fields){
    FileSignature fs(current, fields);
    return  fs.get_file(".bin");
}

string SQL::get_whereFile(string current, vector<string> fields,
                            vector<string> expression){
    FileSignature fs(current, fields, expression);
    string s = fs.get_file(".bin");
    return s;
}

//error message helper
string SQL::get_errorMsg(const char command[], SQL_exceptions e){
    const bool debug = false;

    if (debug) cout << command << " " << e << endl;

    if (e == ECHO_INPUT)
        return command;
    if (e == STREAM_EOF)
        return _mapped_exceptions[e];
    else if (command[0] != '\0'){
        return _mapped_exceptions[e] + " : " + command;
    }
}

                        /* PARSE TREE FILTERS */

int SQL::get_command(){
    const bool debug = false;

    int const COMMAND = 0;

    string command = _parsed_command["Command"][COMMAND];
    if (debug) cout << "get_command() << " << command << endl;

    return _mapped_commads[command];
}

bool SQL::get_table(string tbl_name){
    FileSignature fs(tbl_name);

    if (file_exists(fs.get_file(".bin").c_str())){

        _mapped_tables[tbl_name] = Table(tbl_name);
        return true;
    }
    else {
        return false;
    }
}

vector<string> SQL::get_fields(){
    const bool debug = false;

    if (debug) cout << "||| Returning Fields : \n" << _parsed_command["Fields"];
    return _parsed_command["Fields"];
}

vector<string> SQL::get_values(){
    return _parsed_command["Values"];
}

vector<string> SQL::get_where(){
    const bool debug = false;

    return _parsed_command["Expression"];
}

bool SQL::is_batch(){
    bool perform_batch = (_parsed_command.find("Batch").is_null())
            ? false : true;

    return perform_batch;
}

string SQL::get_table(){
    int const NAME = 0;

    return _parsed_command["Table Name"][NAME];
}

bool SQL::is_select_all(){
    bool isSelectAll = (get_fields()[0] == "*") ? true : false;
    return isSelectAll;
}

bool SQL::is_select_where(){
    const bool debug = false;

    if (debug) cout << "Current Parse Tree : \n" << _parsed_command << endl;

    bool isSelectWhere = (!_parsed_command["Expression"].empty())
            ? true : false;

    return isSelectWhere;
}

                            /* FORMATING */

void SQL::print_results(string command, Table table, ostream &outs){
    outs << "[" << _command_count << "] : " << command << endl
         << table << endl;
}

void SQL::print_description(){
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
         << setw(47) << " Relational Database System \n"
         << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
         << " * Implementation of a database that will respond to SQL-like\n"
         << " * commands. They include : \n"
         << " * >> CREATE|MAKE TABLE <TABLENAME> FIELDS <FIELDNAME(S) . . .>\n"
         << " * >> INSERT INTO <TABLE NAME> VALUES <VALUE(S) . . . >\n"
         << " * >> SELECT *|<FIELD(S) . . .> FROM <TABLENAME> WHERE <CONDITION>"
         << endl << endl
         << " * <CONDITION> include : <FIELDNAME> <RELATIONAL_OPERATOR> <VALUE>"
         << endl
         << " * chain conditions together with a <LOGICAL OPERATOR>\n"
         << " * <RELATIONAL OPERATOR> : = < > <= >=\n"
         << " * <LOGICAL OPERATOR> : AND OR" << endl
         << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";
}
