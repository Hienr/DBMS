#ifndef ARRAY_FUNCTIONS_H
#define ARRAY_FUNCTIONS_H
#include <iostream>
#include "utility.h"
#include <vector>

using namespace std;

//ARRAY UTILITY FUNCTIONS ------------------------------------------------//

template <class T>                      //returns larger of the two items
T maximal(const T& a, const T& b);

template <class T>                      //swaps the two items
void SWAP(T& a, T& b);

template <class T>                      //returns index of largest item in data
int index_of_maximal(T data[ ], int n);

template <class T>                      //insert entry into sorted array
void ordered_insert(T data[ ], int& n, T entry);

template <class T>                      //returns first element greater than e
int first_ge(const T data[ ], int n, const T& entry);

template <class T>                      //append entry to the right of data
void attach_item(T data[ ], int& n, const T& entry);

template <class T>                      //insert entry at index i in data
void insert_item(T data[ ], int i, int& n, T entry);

template <class T>                      //remove last element and place in enry
void detach_item(T data[ ], int& n, T& entry);

template <class T>                      //remove entry at i, and place in entry
void delete_item(T data[ ], int i, int& n, T& entry);

template <class T>                      //append data2 to the right of data1
void merge(T data1[ ], int& n1, T data2[ ], int& n2);

template <class T>                      //move n/2 elements from data1todata 2
void split(T data1[ ], int& n1, T data2[ ], int& n2);

template <class T>                      //copy src[] into dest[]
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size);

template <class T>                      //Prints Array
void print_array(const T data[], int n, int pos = -1);

template <class T>                      //item > all data[i]
bool is_gt(const T data[], int n, const T& item);

template <class T>                      //item <= all data[i]
bool is_le(const T data[], int n, const T& item);

template<typename T>
void insertion_sort(T a[], unsigned int size);

template<typename T>
bool search(T a[], unsigned int size, T entry);

template<typename T>
void fill(T a[], int n, T entry);

template <typename T>                   //verify array is sorted in ascending o
bool verify(const T a[], unsigned int  size);

template<typename T>
void shuffle(T a[], int size, int how_many);

//-------------- Vector Extra operators: ---------------------

template <typename T>                   //print vector list
ostream& operator <<(ostream& outs, const vector<T>& list){
    for (int i = 0; i < list.size(); i++)
        outs << "[" << list[i] << "]";
    return outs;
}

template <typename T>                   //list.push_back addme
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back(addme);
    return list;
}

template<typename T>
bool contains(vector<T>& list, T entry);

//Implementations -------------------------------------------------------//

template <class T>                      //Returns larger of the two items
T maximal(const T& a, const T& b){
    return (a > b) ? a : b;
}

template <class T>                      //Swaps the two items
void SWAP(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}

template <class T>                      //returns index of largest item in data
int index_of_maximal(T data[], int n){
    assert(n > 0 && "index_of_maximal: size must be >0");
    if (n < 1)                      //makes sure size is at least 1
        return -1;

    unsigned int greatest_index = data[0];
    for (unsigned int i = 1; i < n; i++)
        if (data[i] > data[greatest_index])
            greatest_index = i;
    return (int)greatest_index;
}

template <class T>                      //insert entry into sorted array
void ordered_insert(T data[ ], int& n, T entry){
    data[n] = entry;
    n++;
    insertion_sort(data, n);
}

template <class T>                      //returns first element greater than e
int first_ge(const T data[], int n, const T& entry){
    int greatest_index = n;
    T greatest;// = NULL;
    assert(n >= 0 && "first_ge: size was negative");

    for (unsigned int i = 0; i < n; i++)
        if (data[i] >= entry){
            greatest_index = i;
            break;
        }
    return greatest_index;
}

template <class T>                      //append entry to the right of data
void attach_item(T data[], int& n, const T& entry){
    assert(n > -1 && "attatch_item: negative size");
                                        //ensures non-neg size
//    n++;
    data[n] = entry;
    n++;
}

template <class T>                      //insert entry at index i in data
void insert_item(T data[ ], int i, int& n, T entry){
                                        //checks
    //assert(i < n && "insert_item: trying to insert out of bounds");
    assert(i > - 1 && n > - 1 && "insert_item: either negative size/index");

    n++;
    for (unsigned int j = n - 1; j != i; --j)
        SWAP(data[j - 1],data[j]);
    data[i] = entry;
}

template <class T>                      //remove last element and place in enry
void detach_item(T data[ ], int& n, T& entry){
    assert(n > -1 && "detatch_item: index given was negative");
                                        //checks
                                        //fix size
    entry = data[n - 1];                //return value
    n--;
}

template <class T>                      //remove entry at i, and place in entry
void delete_item(T data[ ], int i, int& n, T& entry){
    assert(n > -1 && i > -1 && "delete_item: negative parameter given");
    assert(i <= n && "delete_item: index given was out of bounds");
                                        //checks
    int walker = i;
    while (walker < n - 1){
        SWAP(data[walker], data[walker+1]);
        walker++;
    }
    detach_item(data, n, entry);
}

template <class T>                      //append data2 to the right of data1
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    assert(n1 > -1 && n2 > -1 && "merge: size was negative");
                                        //checks

//    cout << "|| Entered merge function" << endl;
                                        //merge Arrays

    for (int i = n1, j = 0; i < n1 + n2; i++, j++)
        data1[i] = data2[j];
    n1 += n2;                           //change size

//    cout << "||Exiting merge function" << endl;
}

////1,2,3  -->   1 / 3
//template <class T>                      //move n/2 elements from data1todata 2
//void split(T data1[ ], int& n1, T data2[ ], int& n2){
//    assert(n1 > -1 && n2 > -1 && "split: size was negative");

//    int how_many = n1/2;
//    for (unsigned int i = 0; i < how_many; i++){
//        T item;// = NULL;
//        detach_item(data1, n1, item);
//        ordered_insert(data2, n2, item);
//    }
//}

//1,2,3  -->   1 / 3
template <class T>                      //move n/2 elements from data1todata 2
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    assert(n1 > -1 && n2 > -1 && "split: size was negative");

    int how_many = n1/2;
    for (int i = 0; i < how_many; i++){
        T item;// = NULL;
        detach_item(data1, n1, item);
        insert_item(data2, 0, n2, item);
    }
}

template <class T>                      //copy src[] into dest[]
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){
    dest_size = src_size;
    for (unsigned int i = 0; i < src_size; i++){
        dest[i] = src[i];
    }
}

template <class T>                      //Prints Array
void print_array(const T data[], int n, int pos){
    assert(pos < n && "print_array: pos was higher than size");
    const int START_INDEX = 0;
    int start = (pos == -1) ? START_INDEX : pos;
    for (int i = start; i < n; i++){
        if (i%10 == 0)
            cout << endl;
        cout << i <<"[" << data[i] << "] ";
    }
    cout << endl;
}

template <class T>                      //item > all data[i]
bool is_gt(const T data[], int n, const T& item){
                                        //assume all data is SORTED
    return item > data[n-1];
}

template <class T>                      //item <= all data[i]
bool is_le(const T data[], int n, const T& item){
                                        //assume data is SORTED
    return item <= data[0];
}

template<typename T>
void insertion_sort(T a[], unsigned int size){
    for (unsigned int i = 1; i < size; i++){
        T value = a[i];
        unsigned int hole = i;

        while ((hole > 0) && (a[hole - 1] > value)){
            a[hole] = a[hole - 1];
            hole--;
        }
        a[hole] = value;
    }
}

template<typename T>
int search(T a[], int size, T entry){
    for (int i = 0; i < size; i++){
        if (a[i] == entry)
            return i;
    }
    return size;
}

template<typename T>
void fill(T a[], int n, T entry){
    for (int i = 0; i < n; i++)
        a[i] = entry;
}

template <typename T>
bool verify(const T a[], int  size){
    bool sorted = true;
    int i = 0;
    while ((sorted) && (i < size - 1)){
        if (a[i] > a[i+1])
            sorted = false;
        i++;
    }
    return sorted;
}


/*//shuffle a[ ]: Put this in a function!
    for (int i = 0; i< how_many; i++){
        int from = Random(0, how_many-1);
        int to = Random(0, how_many -1);
        int temp = a[to];
        a[to] = a[from];
        a [from] = temp;
    }*/
template<typename T>
void shuffle(T a[], int size, int how_many){
    random r;
    for (int i = 0; i < how_many; i++){
        int from = r.getNext(0, how_many - 1);
        int to = r.getNext(0, how_many - 1);
        int temp = a[to];

        a[to] = a[from];
        a[from] = temp;
    }
}

template<typename T>
bool contains(vector<T>& list, T entry){
    for (int i = 0; i < list.size(); i++)
        if (list[i] == entry)
            return true;
    return false;
}

#endif // ARRAY_FUNCTIONS_H
