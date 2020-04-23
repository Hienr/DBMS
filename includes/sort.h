#ifndef SORT_H
#define SORT_H
#include <iostream>
#include "heap.h"

using namespace std;

template<typename T>
void _swap(T& a, T& b);

//Sorting Functions :

template<typename T>
void insertion_sort(T a[], unsigned int size);

template<typename T>
void bubble_sort(T a[], unsigned int size);

template<typename T>
void quick_sort(T a[], unsigned int size);

template <typename T>
void merge_sort(T a[], unsigned int size);

template<typename T>
void heap_sort(T a[], unsigned int size);

// quick sort helper function :

template<typename T>
void partition(T a[], unsigned int n, unsigned int& pivot_index);

//template<typename T>
//void partition(T a[], size_t n, size_t& pivot_index);

// merge sort helper function :

template <typename T>
void merge(T data[], size_t n1, size_t n2);

//heap helper functions :

template<typename T>
void make_heap(T a[], unsigned int size);

unsigned int parent_index(unsigned int i);

//Implementations -----------------------------------------------------------//

template<typename T>
void insertion_sort(T a[], unsigned int size){
    for (unsigned int i = 1; i < size; i++){
        T value = a[i];
        unsigned int hole = i;

        while ((hole > 0) && (a[hole - 1] > value)){
            swap(a[hole - 1], a[hole]);
            hole--;
        }
//        a[hole] = value;
    }
}

template<typename T>
void bubble_sort(T a[], unsigned int size){
    for (unsigned int i = 0; i < size - 1; i++){
        for (unsigned int j = 0; j < size - i - 1; j++){
            if (a[j] > a[j + 1]){
                T value = a[j + 1];
                a[j + 1] = a[j];
                a[j] = value;
            }
        }
    }
}

template<typename T>
void quick_sort(T a[], unsigned int size){
    unsigned int pivot_index = 0;
    unsigned int n1 = 0;
    unsigned int n2 = 0;

    if (size > 100){
        //partition array and set pivot
        partition(a, size, pivot_index);

        //get size of subarrays
        n1 = pivot_index;
        n2 = size - n1 - 1;

        //recursive call to sort subarrays
        quick_sort(a, n1);
        quick_sort((a + pivot_index + 1), n2);
    }
    else {
        //if the parition reaches less than 100 elements
        //use insertion sort instead
        insertion_sort(a, size);
    }
}

//template<typename T>
//void partition(T a[], size_t n, size_t& pivot_index){
//    T pivot = a[pivot_index];
//    size_t too_big_index = 1;
//    size_t too_small_index = n - 1;

//    while (too_big_index <= too_small_index){
//        while ((too_big_index < n) && (a[too_big_index] <= pivot))
//            too_big_index++;
//        while (a[too_small_index] > pivot)
//            too_small_index--;
//        if (too_big_index < too_small_index)
//            _swap(a[too_big_index], a[too_small_index]);
//    }

//    pivot_index = too_small_index;
//    _swap(a[pivot_index], a[0]);
//    a[pivot_index] = pivot;
//}

template<typename T>
void _swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
void partition(T a[], unsigned int n, unsigned int& pivot_index){
    T pivot = a[pivot_index];
    unsigned int j = 0;                              //boundary for small/big

    for (unsigned int i = 1; i < n; i++){
        if (a[i] <= pivot){
            j++;
            _swap(a[j], a[i]);
        }
    }
    pivot_index = j;
    _swap(a[j], a[0]);
}

template <typename T>
void merge_sort(T a[], unsigned int size){
    size_t n1;                                  //size of first array
    size_t n2;                                  //size of second array

    if (size > 1){
        //compute size of subarrays
        n1 = size/2;
        n2 = size - n1;

        //recursive call to split the array continuously
        merge_sort(a, n1);
        merge_sort((a + n1), n2);

        //merge the sorted halves
        merge(a, n1, n2);
    }

    //If less than 1, return
}

template <typename T>
void merge(T data[], size_t n1, size_t n2){
    T *temp;                                    //points to dynamic array
    size_t copied = 0;
    size_t copied1 = 0;
    size_t copied2 = 0;
    size_t i;

    temp = new int[n1 + n2];

    //merge elements from two halves of data into temp
    while ((copied1 < n1) && (copied2 < n2)){
        if (data[copied1] < (data + n1)[copied2])
            temp[copied++] = data[copied1++];   //copy from first half
        else {
            temp[copied++] = (data + n1)[copied2++];
                                                //copy from second half
        }
    }

    //copy remaning elements from either left or right over
    while (copied1 < n1){
        temp[copied++] = data[copied1++];
    }
    while (copied2 < n2){
        temp[copied++] = (data + n1)[copied2++];
    }

    //copy from temp back into data and release temp
    for (i = 0; i < n1 + n2; i++)
        data[i] = temp[i];
    delete [] temp;
}

template<typename T>
void heap_sort(T a[], unsigned int size){
    Heap<T> heap;
    for (unsigned int i = 0; i < size; i++)
        heap.insert(a[i]);
    for (int j = size - 1; j >= 0; j--){
        a[j] = heap.Pop();
    }
}
#endif // SORT_FNC_H
