#ifndef LIST_H
#define LIST_H
#include <iostream>

using namespace std;

const int INITIAL_V = 0;

template<class T>
struct node{
    T _item;
    node<T>* _next;
    node(const T& item = T(), node<T>* next = nullptr):_item(item), _next(next){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
          outs<<"["<<print_me._item<<"]->";
          return outs;
      }
};

//initializes head to NULL
template <class T>
node<T>* init_head(node<T>* &head);

//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head);

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head);

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy);

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item);

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item);

//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head);

//print the list and return outs
template<class T>
ostream& print_list(const node<T>* head, ostream& outs=cout);

//get the count of a linked list
template<class T>
int get_count(node<T>* head);

/*---------------------------------------------------------------------------------*/
template<class T>
node<T>* init_head(node<T>* &head){
    head = NULL;    //Assign head to NULL
    return head;    //Return pointer
}

template <class T>
T delete_head(node<T>* &head){
    T item = T();

    //Checking if list is empty.
    if (empty(head) == true)
    {
        //cout << "Empty list" << endl;
        return T();
    }

    //A new pointer to point at the head.
    node<T>* discard = head;

    //Move the head to the next item in the list.
    head = head->_next;

    //Set the removed node's data to item to return it.
    item = discard->_item;

    //Free memory at location of the discard pointer, and return.
    delete discard;

    return item;
}

template<class T>
void delete_all(node<T>* &head){
    //Delete the head until the list is empty.
    while (empty(head) == false)
        delete_head(head);
}

template<class T>
bool empty(const node<T>* head){
    if (head == NULL)
        return true;   //Head is NULL, list is empty.
    else
        return false;    //Head isn't NULL, list is not empty.
}

template<class T>
ostream& print_list(const node<T>* head, ostream& outs){
    const string END = "|||";   //End of a List.
    //Checking list if it's empty.
    if (empty(head) == true)
    {
        outs << END;
        return outs;
    }

    //Create a pointer to traverse through the list.
    const node<T>* walker = head;

    //While the pointer is not on a NULL.
    while (walker != NULL)
    {
        outs << *walker;            //Print out the contents of the node.
        walker = walker->_next;     //Advance the walker to the next node.
    }
    outs << END;
    return outs;
}

template <class T>
node<T>* insert_head(node<T>* &head, T item){
    //Declare a temporary node and allocate it space
    node<T>* temp = new node<T>;

    //Assign temporary node with passed item, and point it to head.
    temp->_item = item;
    temp->_next = head;

    //Assign the address of head to the temporary node.
    head = temp;

    return temp;
}

template<class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item){
    //Declare a walker and a temporary node.
    node<T>* walker = after;
    node<T>* temp = new node<T>;

    //Check if given address was NULL
    if (head == NULL)
        return insert_head(head, item);

    //Fill he new node with item.
    temp->_item = item;

    //Set temporary to  the next node.
    temp->_next = walker->_next;
    //Set walker's link to temp.
    walker->_next = temp;

    return temp;
}

template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy){
    //Declare walker to go node to node.
    const node<T>* walker = head;

    //Clear whatever is in cpy.
    init_head(cpy);

    //Declare two trackers, one to track where to insert.
    node<T>* tracker = cpy;

    //Checking if list is empty.
    if (empty(head) == true)
        return tracker;

    //Assign tracker to the address of the new list's head.
    tracker = insert_head(cpy, walker->_item);

    //Advance walker to the next item.
    walker = walker->_next;

    //While walker does not hit a NULL, insert a node after
    //the head, then move up both tracker and walker to
    //continually insert a node after the tracker.
    while(walker != NULL)
    {
        insert_after(cpy, tracker, walker->_item);
        tracker = tracker->_next;
        walker = walker->_next;
    }
    return tracker;
}

template<class T>
int get_count(node<T>* head){
    if (head == NULL)   //If list is empty, return 0.
        return INITIAL_V;
    T count = INITIAL_V;
    node<T>* counter = head;
    while (counter != NULL){
        count++;
        counter = counter->_next;
    }
    return count;
}

#endif // LIST_H
