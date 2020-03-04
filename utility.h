#ifndef UTILITY_H
#define UTILITY_H
#include <time.h>
#include <ctime>
#include <cassert>
#include <string>

using namespace std;

static const bool debug = false;

string get_current_date();

string get_current_time();

class timer{
    private:
        clock_t _start;
        clock_t _stop;

    public:
        timer():_start(clock()), _stop(clock()){}

        void start(){ _start = clock(); }

        void stop(){ _stop = clock(); }

        double duration(){
            double duration = (_stop - _start) / (double)CLOCKS_PER_SEC;
            return duration;
        }
};

class averager
{
    public:
        averager();                         //Constructor
        void next_number(double value);     //Add another number to the object
        size_t how_many_numbers() const {   //Return how many numbers were
            return count;                   //given to the averager
        }
        double average() const;             //Return the average
                                            //from all the numbers.
    private:
        size_t count;                       //The numbers given to the averager
        double sum;                         //Sum of all the numbers given
};

class random
{
    public:
        random();
        int getNext(int lo, int hi);

    private:

    //----------------------------------------------------------------------
    //static member variable keeps the random number generator from being
    //seeded everytime a new Random object is instantiated.

    //Note the initialization of the static variable in the.cpp file.static

        static int objectCount;

    //----------------------------------------------------------------------
};
#endif // UTILITY_H
