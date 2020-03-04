#include "record.h"
#include <iomanip>
Record::Record(){
    recno = -1;
    recno = 0;

    memset(&record, 0, sizeof(record));
}

Record::Record(vector<string> input){
    const bool debug = false;

    memset(&record, 0, sizeof(record));

    if (debug) cout << "Record(input)\n";
    recno = 0;
    for (int i = 0; i < input.size(); i++){
        strcpy(record[i], input[i].c_str());

        if (debug) cout << "record[i] : " << record[i] << endl;
    }

    if (debug) cout << "Constructor succesfully fired.\n";
}

long Record::write(fstream& outs){
    //write to end of the file
    long pos = 0;
    outs.seekg(pos, outs.end);
    pos = outs.tellp();

    outs.write(&record[0][0], sizeof(record));

    return pos / sizeof(record);            //return a record #
//    return pos;
}

long Record::read(fstream& ins, long recno){
    long pos = recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);

    ins.read(&record[0][0], sizeof(record));

    return ins.gcount();                    //returns position
}

string Record::get_field_data(int field){
    const bool debug = false;

    if (debug) cout << record[field] << endl;
    return record[field];
}

vector<string> Record::vectorize_fields(){
    const bool debug = false;

    vector<string> field_data;

    for (int i = 0; i < sizeof(record) / sizeof(record[0]); i++){

        field_data.push_back(record[i]);
    }

    if (debug) cout << field_data << endl;

    return field_data;                      //return a vector form of a record
}

void Record::print_record(ostream& outs) const{
    for (int i = 0; i < sizeof(record) / sizeof(record[0]); i++)
        outs << setw(13) << record[i] << setw(5);
}
