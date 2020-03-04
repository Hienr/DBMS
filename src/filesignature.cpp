#include "filesignature.h"

/* All three constructors take a name or name + vector or name + vector + vector
 * Mashes every name, and element in each vector into one file name */
FileSignature::FileSignature(string name){
    is_long = false;
    _src = name;
    clean_quotes();

    _buffer.push(_src);

    map_unknowns();
}

FileSignature::FileSignature(string name, vector<string> v1){
    is_long = false;
    _src = name;
    clean_quotes();

    _buffer.push(_src);

    push_into_queue(v1);

    map_unknowns();
}

FileSignature::FileSignature(string name, vector<string> v1, vector<string> v2){
    is_long = false;
    _src = name;
    clean_quotes();

    _buffer.push(_src);

    push_into_queue(v1);
    push_into_queue(v2);

    map_unknowns();
}

string FileSignature::get_file(string ext){
    const bool debug = false;
    /* will return an appropriate file name by mashing all elements in the
     * vector, string name, and extension */

    string file_name = "";

    while (!_buffer.empty() && file_name.size() < MAX - ext.size()){
        string token = _buffer.pop();

        if (token == _src)
            file_name += token;
        else if (_unknowns.contains(token)){
            file_name += "_" + _unknowns[token];
        }
        else{
            file_name += "_" + token;
        }
    }

    if (debug) cout << "About to assign a file name of "
                    << file_name << ext << endl;

    return file_name += ext;
}

string FileSignature::remove_ext(string str, string ext){
    //remove an extension from a file name
    size_t fpos = str.find(ext);
    if (fpos != string::npos)
        return str.substr(0, fpos);
    return str;
}

void FileSignature::clean_quotes(){
    //clean " marks
    const bool debug = true;

    size_t quote_pos = _src.find('\"');
    while (quote_pos != -1){
        _src.erase(quote_pos, 1);

        quote_pos = _src.find('\"');
    }
}

void FileSignature::push_into_queue(vector<string> v){
    for (int i = 0; i < v.size(); i++)
        _buffer.push(v[i]);
}

void FileSignature::map_unknowns(){
    assert(unknowns.size() == knowns.size() && "Size Mismatch when mapping\n");

    for (int i = 0; i < unknowns.size(); i++)
        _unknowns[unknowns[i]] = knowns[i];
}
