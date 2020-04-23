#ifndef QUEUE_H
#define QUEUE_H
#include "fnc_list.h"

template <class T>
class Queue{
    public:
        Queue():_head(NULL), _tail(NULL){}      //Constructor

        ~Queue(){}                              //Deconstructor
        Queue(const Queue<T>& other);           //Copy Constructor
        Queue<T> &operator=(const Queue& rhs);
                                                //Overloaded = Operator

        void push(T item);
        T pop();
        bool empty();
        int count();
        T front();

        friend ostream& operator<<(ostream& outs, const Queue& q){
            return print_list(q._head, outs);
        }

    private:
        node<T>* _head;
        node<T>* _tail;
};

template<class T>
Queue<T>::Queue(const Queue<T>& other){
    copy_list(other._head, this->_head);
    copy_list(other._tail, this->_tail);
}

template<class T>
void Queue<T>::push(T item){
    _tail = insert_after(_head, _tail, item);
}

template<class T>
T Queue<T>::pop(){
    return delete_head(_head);
}

template<class T>
bool Queue<T>::empty(){
    return ::empty(_head);
}

template<class T>
T Queue<T>::front(){
    if (_head == NULL)
        return T();
    return _head->_item;
}

template<class T>
Queue<T>& Queue<T>::operator=(const Queue<T>& rhs){
    copy_list(rhs._head, this->_head);
    copy_list(rhs._tail, this->_tail);
    return *this;
}

template<class T>
int Queue<T>::count(){
    return get_count(_head);
}
#endif // QUEUE_H
