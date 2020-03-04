#ifndef LIST_CLASS_H
#define LIST_CLASS_H
#include "fnc_list.h"
#include<iostream>

template <class T>
class List
{
    public:
        List();                     //Constructor
        ~List();                    //Deconstructor
        List<T>& operator=(const List<T> &other);
        List(const List<T> &other); //Copy Constructor
                                    //Overload = operator
        friend                      //Overload << operator
        ostream& operator<<(ostream& out, const List<T> &list){
            print_list(list.head, out);
            return out;
        }
        friend                      //Overload >> operator
        istream& operator>>(istream& in, List<T> &list){
            T item;
            in >> item;
            insert_head(list.head, item);
            return in;
        }

        T firstNode_item();
        node<T>* insert(T item);    //Insert a single head.
        node<T>* insert_after(const node<T>* &after, T item);
                                    //Insert after a specific node.

        bool isEmpty();             //Determine if the list is empty.
        T removeHead();             //Remove a head from the list.
        List<T> clearList();        //Clear the entire list
        void copyList(List<T> src, List<T> cpy);
                                    //Copy a List;
        ostream& printList(const List<T> list, ostream& out = cout);
                                    //Prints a given List
    private:
        node<T>* head;
};

template<class T>
List<T>::List(){
    //Both pointers initialized to NULL to avoid garbage values.
    init_head(head);
}

template<class T>
List<T>::~List(){
    //Delete both head and , and all the memory it has reserved.
    delete_all(head);
}

template<class T>
List<T>::List(const List<T> &other){
    copy_list(other.head, head);
}

template<class T>
List<T>& List<T>::operator=(const List<T> &other){
    copy_list(other.head, this->head);
    return *this;
}

template<class T>
node<T>* List<T>::insert(T item){
    return insert_head(head,item);
}

template<class T>
node<T>* List<T>::insert_after(const node<T>* &after, T item){
    return insert_after(after, item);
}

template<class T>
T List<T>::removeHead(){
    return delete_head(head);
}

template<class T>
List<T> List<T>::clearList(){
    delete_all(this->head);
    return *this;
}

template<class T>
bool List<T>::isEmpty(){
    return empty(this->head);
}

template<class T>
void copyList(const List<T> src, List<T> &cpy){
    List<T> temp(src);
    cpy = temp;
}

template<class T>
ostream& printList(const List<T> list, ostream& out = cout){
    out << list;
    return out;
}

template<class T>
T List<T>::firstNode_item(){
    return head->_item;
}

#endif // LIST_CLASS_H
