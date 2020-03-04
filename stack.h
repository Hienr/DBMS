#ifndef STACK_H
#define STACK_H
#include "list.h"

template <class T>
class Stack{
    public:
        Stack(){}               //Constructor
        ~Stack(){}              //Deconstructor
        Stack(const Stack<T>& other);
                                //Copy Constructor
        Stack<T>& operator=(const Stack<T>& rhs);

        void push(T item);      //Function to push into stack.
        T pop();                //Function to pop the first item in the stack.
                                //Returns the data that was popped.
        T top();                //Returns the top data in the stack.
        bool empty();
        friend                  //Overload << operator
        ostream& operator<<(ostream& outs, const Stack& s){
            printList(s.stack, outs);
            return outs;
        }
        friend                      //Overload >> operator
        istream& operator>>(istream& in, Stack& s){
            T item;
            in >> item;
            s.push(item);
            return in;
        }

    private:
        List<T> stack;
};

template<class T>
Stack<T>::Stack(const Stack<T>& other){
    copyList(other.stack, this->stack);
}

template<class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& rhs){
    copyList(rhs.stack, this->stack);
    return *this;
}

template<class T>
void Stack<T>::push(T item){
    stack.insert(item);
}

template<class T>
T Stack<T>::pop(){
    return stack.removeHead();
}

template<class T>
T Stack<T>::top(){
    return stack.firstNode_item();
}

template<class T>
bool Stack<T>::empty(){
    return stack.isEmpty();
}
#endif // STACK_H
