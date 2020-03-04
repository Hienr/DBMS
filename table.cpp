#include "table.h"
#include "record.h"
#include <string>

using namespace std;

Table::Table(){ ; }

Table::Table(string tableName){
    /* Constructor for pre-existing tables. With the pre-existing table we can
     * repopulate the MAP of indices */

    load_existing(tableName);
}

Table::Table(string tableName, vector<string> field_names){
    /* Constructor to create a new table */

    create_table(tableName, field_names);
}

void Table::create_table(string tableName, vector<string> field_names){
    const bool debug = false, print = false;

    _tableName = tableName;         //get name of the table

    /* generate blank files/tables */
    write_fields(field_names);
    generate_bin();

    /* create blank table */
    fill_current_fields(field_names);
    _indices_list.resize(_current_fields.size());

    /* MAP ALL KEYWORDS FOR FUTURE USE */
    map_fields();

    if (debug) cout << "map_fields() : " << _map << endl;

    if (print) print_maps();
}

void Table::insert_into(vector<string> fields){
    const bool debug = false;

    if (fields.size() != _current_fields.size())
        throw (FIELDS_MISMATCH);

    if (debug) cout << "insert_into() : fields : " << fields << endl;

    Record record(fields);
    fstream ff;

    /* insert record from vector into the file */
    open_fileRW(ff, (get_binFile_onDisk()).c_str());
    long pos = record.write(ff);

    /* populate indices vector */
    for (int i = 0; i < _indices_list.size(); i++){
        _indices_list[i][fields[i]]
                += pos;
    }

    ff.close();

    if (debug) print_current_indices("insertion attempt : ");
}

void Table::load_existing(string tableName){
    const bool debug = false, print = false;

    /* set table name */
    _tableName = tableName;

    /* if file does not exist, must throw exception immediately */
    //assert(file_exists(get_fieldFile_onDisk().c_str()));
    if (!file_exists(get_fieldFile_onDisk().c_str()))
            throw (FILE_DNE);

    /* because the file exists, open it again for read */
    ifstream in;
    in.open(get_fieldFile_onDisk());

    /* FILL VECTOR OF FIELDS for a tracker of the current fields */
    fill_current_fields_from_file(in);
    _indices_list.resize(_current_fields.size());
    in.close();

    /* MAP ALL KEYWORDS FOR FUTURE USE */
    map_fields();

    if (debug) cout << "vector<string> _current_fields : " << _current_fields
                    << endl;

    if (debug) cout << "map_fields() : " << _map << endl;

    /* Reindex */
    reindex();

    if (print) print_current_indices("reindex attempt : ");
}

//                       debug helper
void Table::print_maps(){
    for (int i = 0; i < _current_fields.size(); i++){
        cout << setw(30) << _current_fields[i] << endl
             <<_indices_list[i] << endl;
    }
}

void Table::print_current_indices(string msg){
    cout << msg << endl;
    print_maps();
    cout << endl;
}

void Table::print_record_vec(vector<Record> records){
    for (int i = 0; i < records.size(); i++)
        cout << records[i] << endl;
}

void Table::print_records(vector<long> recnos){
    fstream ff;
    Record record;

    open_fileRW(ff, get_binFile_onDisk().c_str());

    for (int i = 0; i < recnos.size(); i++){
        record.read(ff, recnos[i]);

        cout << "\n[" << i << "] : \n" << record;
    }

}

//                        ---------

Table Table::select_all(){
    const bool debug = false;

    Record record;
    fstream ff;

    /* open the file */
    open_fileRW(ff, (get_binFile_onDisk()).c_str());
    int i = 0;

    while (!ff.eof()){
        record.read(ff, i);

        if (ff.eof()) break;

        i++;
        if (debug) cout << record << endl;
    }

    //return the table
    return Table(_tableName);
}

Table Table::select_all(vector<string> conditions){
    return select(_current_fields, conditions);
}

Table Table::select(vector<string> field_names){
    const bool debug = false, print = false;

    Record full_record;
    fstream ff;
    vector<string> new_record;
    vector<Record> records;

    /* open the original bin */
    open_fileRW(ff, (get_binFile_onDisk().c_str()));
    int i = 0;

    /* create new table with these specs */
    Table filtered_table(get_selectionFile(_tableName, field_names),field_names);


    /* check field names */
    if (!do_fields_match(filtered_table.get_field_names(), field_names))
        throw (FIELDS_MISMATCH);

    while (!ff.eof()){
        full_record.read(ff, i);

        if (debug) cout << full_record << endl;

        if (ff.eof()) break;

        /* grab filtered records */
        for (int j = 0; j < field_names.size(); j++){
            new_record.push_back(full_record.get_field_data(_map[field_names[j]]));

            if (debug) cout << "select() : record: " << new_record << endl;
        }

        filtered_table.insert_into(new_record);
        new_record.clear();
        i++;
    }

    if (print) filtered_table.print_current_indices("selected fields :\n");
    return filtered_table;
}

Table Table::select(vector<string> field_names, vector<string> conditions){
    const bool debug = false, print = false;

    Set set(conditions, _indices_list, _map);

    //evaluate query to get indices of where to look
    vector<long> records = set.eval();
    vector<string> new_record;

    if (debug) cout << "Evaluated Vector : " << records << endl;
    if (debug) print_records(records);

    Record full_record;
    fstream ff;
    int i = 0;

    open_fileRW(ff, get_binFile_onDisk().c_str());

    /* create new table with these specs */
    Table filtered_table(get_whereFile(_tableName, field_names, conditions),
                         field_names);

    //check if fields match
    if (!do_fields_match(filtered_table.get_field_names(), field_names))
        throw (FIELDS_MISMATCH);

    while ((!ff.eof()) && (i < records.size())){
        //read a record from the relevant indices

        full_record.read(ff, records[i]);


        if (ff.eof()) break;

        /* grab filtered records */
        for (int j = 0; j < field_names.size(); j++)
            new_record.push_back(full_record.get_field_data(_map[field_names[j]]));

        // create new filtered table
        filtered_table.insert_into(new_record);
        new_record.clear();
        i++;
    }

    ff.close();

    if (print) filtered_table.print_current_indices("selected fields :\n");

    return filtered_table;
}

void Table::drop(){
    /* call this to delete a select table on disk */
    const bool debug = false;

    if (debug) cout << "dropping " << get_binFile_onDisk() << endl;


    if(file_exists(get_binFile_onDisk().c_str())){
        if (debug) cout << get_binFile_onDisk() << " existed. DROPPING\n";

        if (remove(get_binFile_onDisk().c_str()) != 0)
            assert(true && "bin clean up failed\n");
    }
    if(file_exists(get_fieldFile_onDisk().c_str())){
        if (debug) cout << get_fieldFile_onDisk() << " existed. DROPPING\n";

        if (remove(get_fieldFile_onDisk().c_str()) != 0)
            assert(true && "field clean up failed\n");
    }
}

string Table::get_table_name(){
   return _tableName;
}

vector<string> Table::get_field_names(){
    return _current_fields;
}

void Table::print_table(ostream& outs){
    Record record;
    fstream ff;
    int i = 0, count = 1;

    open_fileRW(ff, get_binFile_onDisk().c_str());

    cout << endl;

    outs << string(75,'-') << endl;

    for (int i = 0; i < _current_fields.size(); i++){
        outs << setw(14) << _current_fields[i];
    }
    outs << endl << string(75,'-') << endl << setw(5);

    while(!ff.eof()){
        record.read(ff, i);

        if (ff.eof()) break;

        outs << count << "." << record << endl;
        count++;
        i++;
    }
    outs << endl << string(75,'-') << endl;
}

bool Table::index(fstream& ff, int recordnum, long &pos){
    const bool debug = false;

    /* indexes ONE record */

    Record record;

    pos += record.read(ff, recordnum);

    if (ff.eof())
        return false;

    vector<string> field_data = record.vectorize_fields();
    for (int i = 0; i < _indices_list.size(); i++)
        _indices_list[i][field_data[i]] += (long) recordnum;
    return true;
}

void Table::reindex(){
    const bool debug = false;
    /* indexes MULTIPLE records */

    Record record;
    fstream ff;
    long pos = 0;
    /* read a chunk of the records */

    open_fileRW(ff, get_binFile_onDisk().c_str());
    int i = 0;

    /* fill vector of multimaps with indices */
    while (index(ff,i, pos))
        i++;

    ff.close();
}

int Table::get_index(string find_me){
    return _map[find_me];
}

void Table::fill_current_fields(vector<string> fields){
    const bool debug = false;

    for (int i = 0; i < fields.size(); i++)
        _current_fields.push_back(fields[i]);

    if (debug) cout << "fill_current_fields() : " << _current_fields << endl;
}

void Table::fill_current_fields_from_file(ifstream& in){
    const bool debug = false;

    int fields;
    in >> fields;
    in.ignore();

    string field;

    for(int i = 0; i < fields; i++){
        getline(in, field);
        _current_fields.push_back(field);
    }

    if (debug) cout << "fill_current_fields_from_file() : " << _current_fields
                    << endl;
}

void Table::write_fields(vector<string> field_names){
    ofstream of;

    of.open(get_fieldFile_onDisk());
    write_vector(of, field_names);
}

void Table::generate_bin(){
    fstream ff;

    open_fileRW(ff, get_binFile_onDisk().c_str());
}

bool Table::do_fields_match(vector<string> src, vector<string> fields){
    if (fields[0] == "*")
        return true;

    for (int i = 0; i < fields.size(); i++)
        if (!contains(src, fields[i]))
            return false;
    return true;

}

string Table::get_binFile_onDisk(){
    FileSignature fs(_tableName);
    string s = fs.get_file(".bin");
    return s;
}

string Table::get_fieldFile_onDisk(){
    FileSignature fs(_tableName);
    return fs.get_file("_fields.txt");
}

// Do not append .bin for selectionFile and whereFile

string Table::get_selectionFile(string current, vector<string> fields){
    FileSignature fs(current, fields);
    return  fs.get_file("");
}

string Table::get_whereFile(string current, vector<string> fields,
                            vector<string> expression){
    FileSignature fs(current, fields, expression);
    return fs.get_file("");
}

void Table::map_fields(){
    const bool debug = false;

    for (int i = 0; i < _current_fields.size(); i++)
       _map[_current_fields[i]] = i;

    if (debug) cout << _map << endl;
}
