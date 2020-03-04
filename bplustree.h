#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <iomanip>
#include "fnc_array.h"

using namespace std;

//debug

//static const bool debug = false;

template <class T>
class BPlusTree
{
    public:

    class Iterator{
        public:
            friend class BPlusTree;
            Iterator(BPlusTree<T>* _it = NULL, int _key_ptr = 0)
                :it(_it), key_ptr(_key_ptr){}

            T operator *(){
                assert(!this->is_null() && "Accessing non-existent key\n");
                assert(key_ptr < it->data_count);

                return it->data[key_ptr];
            }

            Iterator operator++(int unused){
                Iterator hold(this->it, this->key_ptr);
                ++*this;

                return hold;
            }

            Iterator& operator++(){
                if (key_ptr + 1 < it->data_count){
                    key_ptr++;
                }
                else{
                    it = it->next;
                    key_ptr = 0;
                }
                return *this;
            }

            friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
                return ((lhs.it == rhs.it) && (lhs.key_ptr == rhs.key_ptr));
            }

            friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
                return ((lhs.it != rhs.it) || (lhs.key_ptr != rhs.key_ptr));
            }

            void print_Iterator(){
                if (!is_null())
                    cout << *it << endl;
            }

            bool is_null(){ return !it; }

        private:
            BPlusTree<T>* it;
            int key_ptr;
        };

        //Constructor
        BPlusTree(bool dups = false);

        // Copy, Assignment, Destructor
        BPlusTree(const BPlusTree<T>& other);
        ~BPlusTree();
        BPlusTree<T>& operator =(const BPlusTree<T>& RHS);
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        void insert(const T& entry);
                                    //insert entry into the tree
        void remove(const T& entry);
                                    //remove entry from the tree

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void clear_tree();
                                    //clear this object (delete all nodes etc.)
        void copy_tree(const BPlusTree<T>& other);
                                    //copy other into this object
        void copy_tree(const BPlusTree<T>& other,
                       BPlusTree<T>* &last_leaf);
        bool contains(const T& entry);
                                    //true if entry can be found in the tree

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        T& get(const T& entry);
                                    //return a reference to entry in the tree
        const T& get(const T& entry)const;
                                    //return a reference to entry in the tree
        T& get_existing(const T& entry);
                                    //returns an existing entry ref in the tree

        Iterator find(const T& entry);
                                    //return an iterator to this key.
                                    //NULL if not there.

        int size() const;
                                    //count the number of elements in the tree

        bool empty() const;         //true if the tree is empty

                                        //print a readable version of the tree
        void print_tree(int level = 0, ostream &outs=cout) const;
        friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me){
            print_me.print_tree(0, outs);
            return outs;
        }

        bool is_valid();

        void print_leaves();            //print the linked list depth

        Iterator begin();
        Iterator end();

    private:
        //data count and subset size parameters:
        static const int MINIMUM = 1;
        static const int MAXIMUM = 2 * MINIMUM;

        //Member Variables :
        bool dups_ok;                   //true if duplicate keys allowed
        int data_count;                 //number of data elements
        T data[MAXIMUM + 1];            //holds the keys
        int child_count;                //number of children
        BPlusTree* subset[MAXIMUM + 2]; //subtrees
        BPlusTree* next;                //link to next
        int entries;                    //how many entries in the entire tree

        bool is_leaf() const;           //true if this is a leaf node

        T* find_ptr(const T& entry);        //return a pointer to this key.
                                            //NULL if not there.

        //insertion helper functions:

        void loose_insert(const T& entry);
                                                //insertion that
                                                //allows MAXIMUM+1 data
                                                //elements in the root

        void fix_excess(int i);
                                                //fix excess of data elements
                                                //in child i

        //remove helper functions:

        void loose_remove(const T& entry);
                                                //allows MINIMUM-1 data
                                                //elements in the root

        BPlusTree<T>* fix_shortage(int i);
                                                //fix shortage of data elements
                                                //in child i
        BPlusTree<T>* get_smallest_node();
                                                //get the smallest node in tree
        BPlusTree<T>* get_biggest_node();
                                                //get biggest node in tree

        void get_smallest(T& entry);            //entry := leftmost leaf

        void get_second_smallest(T& entry);
                                                //get second smallest

        void get_biggest(T& entry);             //entry := rightmost leaf

        void rotate_left(int i);
                                                //transfer one element LEFT
                                                //from child i
        void rotate_right(int i);
                                                //transfer one element RIGHT
                                                //from child i

        BPlusTree<T> *merge_with_next_subset(int i);
                                                //merge subset i
                                                //with subset i +1

};

template<typename T>
BPlusTree<T>::BPlusTree(bool dups){
    data_count = 0;
    child_count = 0;
    entries = 0;

    dups_ok = dups;

    next = nullptr;
}

template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):
                        data_count(0), child_count(0), next(nullptr){
    if (debug) cout << "Copy Constructor Fired.\n";

    copy_tree(other);
}

template<typename T>
BPlusTree<T>::~BPlusTree(){
    if (debug) cout << "Destructor Fired. \n";

    clear_tree();
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
    if (debug) cout << "Assignment Operator was called. \n";

    if (this != &RHS)
        copy_tree(RHS);
    return *this;
}

template<typename T>                //insert entry into the tree
void BPlusTree<T>::insert(const T& entry){
    /* check if the tree contains entry to be inserted */
    bool contained_entry = (contains(entry));

    /* if tree contained the entry, and dups not allowed - don't insert */
    if (!dups_ok && contained_entry)
        return;

    /* perform a loose insert of entry */
    loose_insert(entry);

    /* keep track of the entires we inserted, but make sure it didn't
     * exist beforehand */

    if (!contained_entry)
        ++entries;

    if (data_count > MAXIMUM){
        BPlusTree<T>* node = new BPlusTree<T>();

        copy_array(node->data, data, node->data_count, data_count);
        copy_array(node->subset, subset, node->child_count, child_count);

        data_count = 0;
        child_count = 1;

        subset[0] = node;

        fix_excess(0);
    }
}

template<typename T>                //remove entry from the tree
void BPlusTree<T>::remove(const T& entry){
    /* prepare for removal, if tree contains entry. preemptively reduce size */
    if (contains(entry))
        --entries;

    loose_remove(entry);

    /* After removal of entry, there is a possibility our tree at ROOT will
     * have nothing inside. Since there is no parent of ROOT to fix, we must
     * fix this manually */

    if ((data_count == 0) && (child_count == 1)){
        BPlusTree<T>* deleter = subset[0];

        copy_array(data,subset[0]->data, data_count, subset[0]->data_count);
        copy_array(subset, subset[0]->subset,
                child_count, subset[0]->child_count);

        deleter->child_count = 0;
        delete deleter;
    }
}

template<typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    /* Clear this tree before copying over other */
    clear_tree();

    BPlusTree<T>* previous_leaf = nullptr;

    /* No previous leaf, so pass in null */
    copy_tree(other, previous_leaf);
}

template<typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other,
                             BPlusTree<T>* &last_leaf){
//    assert(empty());                        //ensure tree is EMPTY

    /* When we reach the leaf mid copying, copy the leaf's data & child count.
     * After copying over all data & child count, link the previous leaf
     * with this object */
    if (other.is_leaf()){
        copy_array(data, other.data, data_count, other.data_count);
        child_count = other.child_count;

        /* The last leaf may not have existed, so don't link */
        if (last_leaf != nullptr)
            last_leaf->next = this;
        last_leaf = this;

    }
    else{
        copy_array(data, other.data, data_count, other.data_count);
        for (int i = 0; i < other.child_count; i++){
            subset[i] = new BPlusTree<T>();
            subset[i]->copy_tree(*other.subset[i], last_leaf);
            child_count = other.child_count;
        }
    }

}

template <typename T>
void BPlusTree<T>::clear_tree(){
    if (is_leaf()){
        return;
    }
    else{
        for (int i = 0; i < child_count; i++){
            subset[i]->clear_tree();
            delete subset[i];
        }
        data_count = 0;
        child_count = 0;
        entries = 0;
    }
}

template<typename T>                //true if entry can be found in the tree
bool BPlusTree<T>::contains(const T& entry){
    Iterator found = find(entry);
    bool doesContain = (found.is_null()) ? false : true;
    return doesContain;
}


template<typename T>                //return a reference to entry in the tree
T& BPlusTree<T>::get(const T& entry){
    if (!contains(entry))
        insert(entry);
    return get_existing(entry);
}

template<typename T>                //return a reference to entry in the tree
T& BPlusTree<T>::get_existing(const T& entry){
    /* Entry MUST exist or else assert */
    assert(contains(entry));

    /* begin search */
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index]==entry);

    if (found){
        if (is_leaf())              //only return data @ leaf
            return data[index];
        else                        //finding it at inner node - follow to leaf
            return subset[index + 1]->get_existing(entry);
    }
    else
        if (!found)  //yet, traverse more
            return subset[index]->get_existing(entry);
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry){
    /* begin to search for entry in this node */
    int index = first_ge(data, data_count, entry);

    bool found = (index < data_count && data[index] == entry);

    if (found){
        if (is_leaf()){             //Case 1A: found @ leaf
            return Iterator(this, index);
        }
        else                        //Case 1B: found !leaf so follow it down
            subset[index + 1]->find(entry);
    }
    else{
        if (is_leaf())              //Case 2A: !find but @ leaf so not here
            return Iterator();
        else                        //Case 2B: !find but not @ leaf so recurse
            subset[index]->find(entry);
    }
}

template<typename T>
int BPlusTree<T>::size() const{
    return entries;
}

template<typename T>
bool BPlusTree<T>::empty() const{
    bool isEmpty = (data_count < 1) ? true : false;
    return isEmpty;
}

template<typename T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const{
    if (!is_leaf())                 //if not a leaf keep entering child
        subset[child_count - 1]->print_tree(level + 1, outs);

                                    //eventually will reach a leaf, or returned
                                    //from a previous child; print it
    for (int i = data_count - 1; i >= 0; i--){
        outs << setw(level * 6) << "[" << data[i] << "]" << endl;
        if (!is_leaf())             //to check if there are more children
            subset[i]->print_tree(level + 1, outs);
    }
}

template<typename T>
void BPlusTree<T>::print_leaves(){
    if (is_leaf()){
        int count = 0;

        BPlusTree<T>* walker = next;
        cout << "|| Leaves Link :\n";
        for (int i = 0; i < data_count;i++){
            cout << "[" << data[i] << "]->";
        }
        while (walker != nullptr){
            for (int i = 0; i < walker->data_count;i++){
                cout << "[" << walker->data[i] << "]->";
            }
            count++;
            walker = walker->next;
            if (count%5 == 0)
                cout << endl;
        }
        cout << "|||\n\n";
    }
    else {
        subset[0]->print_leaves();
    }
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    return Iterator(get_smallest_node());
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    BPlusTree<T>* node = get_biggest_node();
    return Iterator(node->next);
}

template<typename T>
bool BPlusTree<T>::is_leaf() const{
    return (child_count == 0);
}

template<typename T>
T* BPlusTree<T>::find_ptr(const T& entry){
    /* begin to search for entry */
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);

    if (found){
        if (is_leaf())              //Case 1a: found @ leaf so return it
            return &data[index];
        else                        //Case 1b: found @ tree so follow it
            subset[index + 1]->find_ptr(entry);
    }
    else{
        if (is_leaf())              //Case 2a: did not find @ leaf so nonexist
            return nullptr;
        else                        //Case 2b: did not find so recurse
            subset[index]->find_ptr(entry);
    }
}

template<typename T>
bool BPlusTree<T>::is_valid(){
    bool isValid = false;

    if(empty())             //Tree is valid, as it is empty
        return true;

    /* if leaf node, check its level. If the first leaf node was not found yet
     * record its level so every subsequent leaf is the same level. So this
     * function will check:
     * [1] if leaf is SORTED
     * [2] if leaf has the same level as the previous leaves */
    if (is_leaf()){
        if (!verify(data, data_count))
            return false;
        return true;
    }
    else{
        for (int i = 0; i < child_count; i++){
            isValid = subset[i]->is_valid();
        }

        if (isValid){
            for (int i = 0; i < data_count; i++){
                T biggest, smallest;
                subset[i]->get_biggest(biggest);
                subset[i + 1]->get_smallest(smallest);

                if (data[i] < biggest)
                    return false;
                if (data[i] != smallest)
                    return false;
            }
            T biggest;
            subset[child_count - 1]->get_biggest(biggest);

            if (data[data_count - 1] > biggest)
                return false;
        }
        return isValid;
    }
}

template<typename T>                //insertion that allows max+1 data e inroot
void BPlusTree<T>::loose_insert(const T& entry){
                                    //find initial index
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);

    if (found){                 /* FOUND CASES */
        if (is_leaf()){             //Case 1a: Found at leaf, so APPEND
//            data[index] += entry;
            data[index] = entry;
            if (debug) cout << "|| Duplicate was detected.\n";
            return;
        }
        else{                       //Case 1b: Found not at leaf, so insert
                                    //into linked list
            subset[index + 1]->loose_insert(entry);

            if (subset[index + 1]->data_count > MAXIMUM){
                fix_excess(index + 1);
            }
        }
    }
    else {                      /* NOT FOUND CASES */
        if (is_leaf())              //Case 2a: Not found but at leaf; INSERT
            ordered_insert(data, data_count, entry);
        else{                       //Case 2b: Not found -- Recursive call
            subset[index]->loose_insert(entry);

            if (subset[index]->data_count > MAXIMUM)
                fix_excess(index);
        }
    }
}

template<typename T>                //fix excess data elements in child i
void BPlusTree<T>::fix_excess(int i){
    /* Create a new BPlusTree */
    BPlusTree<T> *bplus = new BPlusTree<T>();

    /* Fill new BPlusTree with the split from the array with > MAX */
    split(subset[i]->data, subset[i]->data_count,
          bplus->data, bplus->data_count);

    /* Split both data and the children */
    split(subset[i]->subset, subset[i]->child_count,
            bplus->subset, bplus->child_count);

    /* Perform an insertion with the NEW BPlusTree at i + 1 */
    insert_item(subset, i + 1, child_count, bplus);

    /* Promote subset[i] to be the new parent item */
    T detatched_item = T();
    detach_item(subset[i]->data, subset[i]->data_count, detatched_item);
    ordered_insert(data, data_count, detatched_item);

    /* After the array's have been successfully split. If subset[i+1] is a leaf
     * and apart of the linked list - then insert entry into the LL as well */
    if (subset[i+1]->is_leaf()){
        /* Insert middle value into subset[i + 1]
         * eg           [20]  v
         *          [10]    [20][21]            */

        insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, data[i]);

        /* Link
         *      [50]
         *    [25] [75] [80]
         * i = 0; so link [75]->[80]
         * then [25]-> [75] */
        subset[i+1]->next = subset[i]->next;
        subset[i]->next = subset[i+1];
    }
}

template<typename T>
void BPlusTree<T>::loose_remove(const T& entry){
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);

    if (found){             /* Case 1 FOUND */
        if (is_leaf()){         /* Case 1A Found, @leaf, so just delete */
            if (debug) cout << "|| Deleting @ leaf\n";

            T detachment = T();
            delete_item(data, index, data_count, detachment);
        }
        else{                   /* Case 1B Found, not @leaf so follow it */
            if (debug) cout << "|| Following entry to its data\n";

            /* Found before leaf, so we need to replace the marker with
             * another suitable marker */

            /* -------------------------------------------- */
            T second_smallest = data[index], detachment = T();
            subset[index + 1]->get_second_smallest(second_smallest);

            if (second_smallest == data[index]){
                if (data_count > 1){
                    data[index] = data[index + 1];
                }
            }
            else {
                data[index] = second_smallest;
            }

            /* -------------------------------------------- */

            subset[index + 1]->loose_remove(entry);


            /* Check the need to fix shortage */
            if (subset[index + 1]->data_count < MINIMUM){
                if (debug) cout << "|| Fixing Shortage\n";

                fix_shortage(index + 1);
            }
            else{
                if (debug) cout << "|| Replacing marker in tree.\n";

                /* There was no shortage so we need to replace the marker
                 * with a new marker. Replace data[i] with the smallest
                 * entry in the linked list (subset[i + 1]) */

                subset[index + 1]->get_smallest(data[index]);
            }

        }
    }
    else{                   /* Case 2 NOT FOUND */
        if (is_leaf()){         /* Case 2A Not found, @leaf, so it DNE. */
            if (debug) cout << "|| Entry did not exist.\n";

            /* Entry did not exist in the tree */
            return;
        }
        else{                   /* Case 2B Not Found, not @leaf so recurse */
            if (debug) cout << "|| Entry wasn't found yet, so recursing\n";

            subset[index]->loose_remove(entry);

            if (subset[index]->data_count < MINIMUM)
                fix_shortage(index);
        }
    }
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i){
    bool didFix = false;

    if ((i < child_count - 1)
            && (subset[i + 1]->data_count > MINIMUM)){
        if (debug) cout << "|| Rotating Left\n";

        didFix = true;
        rotate_left(i);
    }
    else if ((i > 0)
             && (subset[i - 1]->data_count > MINIMUM)){
        if (debug) cout << "|| Rotating Right\n";

        didFix = true;
        rotate_right(i);
    }
    else if ((i > 0) && (i <= child_count - 1)){
        if (debug) cout << "|| Merging i-1 to i\n";

        didFix = true;
        merge_with_next_subset(i - 1);
    }
    else {
        if (debug) cout << "|| Merging i-1 to i\n";

        didFix = true;
        merge_with_next_subset(i);
    }

    /* Ensure tree fixes any shortages */
    assert(didFix && "Fix Shortage did not fix a shortage.");

    /* return the pointer of the fixed subset */
    return subset[i];
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){
    if (is_leaf())
        return this;
    else {
        subset[0]->get_smallest_node();
    }
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::get_biggest_node(){
     if (is_leaf()){
         return this;
     }
     else {
         subset[child_count - 1]->get_biggest_node();
     }
}

template<typename T>
void BPlusTree<T>::get_smallest(T& entry){
    if (is_leaf()){
        entry = data[0];
        return;
    }
    else{
        subset[0]->get_smallest(entry);
    }
}

template<typename T>
void BPlusTree<T>::get_second_smallest(T& entry){
    const int ONE_ITEM = 1;
    if (is_leaf()){
        /* if the node is bigger than one node, guaranteed the second
         * smallest entry will have to be at index 1 */

        if (data_count > ONE_ITEM)
            entry = data[1];
        return;
    }
    else{
        /* if node is not a leaf, recurse to find leftmost leaf */
        subset[0]->get_second_smallest(entry);

        /* when returning from recursion if the node we just checked
         * was a leaf, and had only one item in it - then guaranteed
         * entry at current node data[0] will be the second smallest */

        if ((subset[0]->data_count == ONE_ITEM) && (subset[0]->is_leaf())){
            if (entry != data[0])
                entry = data[0];
            return;
        }
    }
}

template<typename T>
void BPlusTree<T>::get_biggest(T& entry){
    entry = data[data_count - 1];
}

template<typename T>
void BPlusTree<T>::rotate_left(int i){
    /* Transfer data[i] down to subset[i] */
    attach_item(subset[i]->data, subset[i]->data_count, data[i]);

    /* Transfer last item from subset[i + 1] to data */
    delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, data[i]);

    /* Transfer subset[i + 1] children to subset[i]'s children */
    if (!subset[i + 1]->is_leaf()){
        BPlusTree<T>* detached_ptr = nullptr;
        delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count,
                detached_ptr);
        attach_item(subset[i]->subset, subset[i]->child_count, detached_ptr);
    }

    /* If child is a leaf, then match the data with the entry point of the LL*/
    if (subset[i + 1]->is_leaf()){
        data[i] = subset[i + 1]->data[0];
    }
}

template<typename T>
void BPlusTree<T>::rotate_right(int i){
    /* Transfer data[i - 1] down to the front of subset[i] */
    insert_item(subset[i]->data, 0, subset[i]->data_count, data[i - 1]);

    /* Transfer last item from subset[i - 1] to data */
    detach_item(subset[i - 1]->data, subset[i - 1]->data_count, data[i - 1]);

    /* Transfer subset[i - 1] children to subset[i]'s children */
    if (!subset[i - 1]->is_leaf()){
        BPlusTree<T>* detached_ptr = nullptr;
        detach_item(subset[i - 1]->subset, subset[i - 1]->child_count,
                detached_ptr);
        insert_item(subset[i]->subset, 0, subset[i]->child_count, detached_ptr);
    }

    /* If child is a leaf, then match the data with the entry point of the LL*/
    if (subset[i]->is_leaf())
        subset[i]->data[0] = data[i - 1];
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){
    /* Find middle value */
    T detatchment;

    delete_item(data, i, data_count, detatchment);

    if (!subset[i]->is_leaf())
        attach_item(subset[i]->data, subset[i]->data_count, detatchment);

    /* Merge child at i + 1 with child i [i] <- [i + 1] */
    merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data,
            subset[i+1]->data_count);

    merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset,
            subset[i+1]->child_count);

    /* Delete the pointer pointing at nothing */
    BPlusTree<T>* deletePtr;

    delete_item(subset, i+1, child_count, deletePtr);

    deletePtr->child_count = 0;
    deletePtr->data_count = 0;

    if (subset[i]->is_leaf())
        subset[i]->next = deletePtr->next;

    delete deletePtr;
}
#endif // BTREE_H
