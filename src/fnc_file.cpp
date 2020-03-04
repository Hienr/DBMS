#include "fnc_file.h"

void write_vector(ofstream& out, vector<string> field_names){
    out << field_names.size() << endl;

    for (int i = 0; i < field_names.size(); i++)
        out << field_names[i] << endl;
}

bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_file(ifstream& f, string fileName) throw(string){
    if (file_exists(fileName.c_str()))
        f.open(fileName);
    else {
        throw ("File did not exist\n");
    }
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary
                | std::fstream::app);
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }
}

void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary | std::fstream::app );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }
}

void save_list(string fileName, Record list[], int count){
    fstream f;
    open_fileRW(f, fileName.c_str());
    for (int i= 0; i<count; i++){
        list[i].write(f);
    }
    f.close();
}

void save_list(string fileName, vector<Record> list){
    fstream f;
    open_fileRW(f, fileName.c_str());
    for (int i = 0; i < list.size(); i++)
        list[i].write(f);
    f.close();
}
