#include <iostream>
#include "utility.h"

string get_current_date(){
    time_t t = time(nullptr);   // get time now
    struct tm * now = localtime( & t );

    string cd = to_string(now->tm_year + 1900);
    cd += ("-" + to_string(now->tm_mon + 1));
    cd += ("-" + to_string(now->tm_mday));

    return cd;

}

string get_current_time(){
    time_t t = time(nullptr);
    struct tm * now = localtime ( & t );

    string ct = to_string(now->tm_hour);
    ct += ("h" + to_string(now->tm_min));
    ct += ("m" + to_string(now->tm_sec));

    return ct;
}

int random::objectCount = 0;

random::random(){
    objectCount++;
    if (objectCount == 1){
        if (debug) cout << "random number generator seeded.\n";

        srand(time(0));
    }
}

int random::getNext(int lo, int hi){
    int random_number = rand()%(hi - lo + 1) + lo;
    return random_number;
}
