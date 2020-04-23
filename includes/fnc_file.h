#ifndef FNC_FILE_H
#define FNC_FILE_H
#include <fstream>
#include "record.h"
using namespace std;
//------------------------ File Functions ----------------------------------//

void write_vector(ofstream& out, vector<string> field_names);

void open_file(ifstream& f, string fileName) throw (string);

void open_fileW(fstream& f, const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);

bool file_exists(const char filename[]);

void save_list(string fileName, Record list[], int count);

void save_list(string fileName, vector<Record> list);
#endif // FNC_FILE_H
