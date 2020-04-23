#ifndef MMAP_H
#define MMAP_H
#include <iostream>
#include <vector>
#include "bplustree.h"

using namespace std;

/* MMAP is based on the B+ Tree */

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()) { key = k; }
    MPair(const K& k, const V& v) {
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist) {
        key = k;
        value_list = vector<V>(vlist);
//        for (int i = 0; i < vlist.size(); i++)
//            value_list.push_back(vlist[i]);
    }
    //--------------------------------------------------------------------------------
    MPair<K,V>& operator +=(const MPair<K, V>& rhs){
        for (int i = 0; i < rhs.value_list.size(); i++)
            value_list += rhs.value_list[i];
        return *this;
    }

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs << print_me.key << " | " << print_me.value_list;
        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key == rhs.key);
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key != rhs.key);
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key < rhs.key);
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key <= rhs.key);
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key >= rhs.key);
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key > rhs.key);
    }
};

template <typename K, typename V>
class MMap
{
    public:

    typedef BPlusTree<MPair<K, V> > map_base;

    class Iterator{
        public:
            friend class MMap;
            Iterator(typename map_base::Iterator it = Iterator(nullptr)){
                _it = it;
            }
            Iterator operator ++(int unused){
                return _it++;
            }
            Iterator operator ++(){
                return ++_it;
            }
            MPair<K, V> operator *(){
                return *_it;
            }
            friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
                return (lhs._it == rhs._it);
            }
            friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
                return (lhs._it != rhs._it);
            }

            void print_Iterator(){
                _it.print_Iterator();
            }

            bool is_null(){ return _it.is_null(); }
        private:
            typename map_base::Iterator _it;
        };

        MMap();

    //  Iterators
        Iterator begin();
        Iterator end();
        Iterator lower_bound(const K key);
        Iterator upper_bound(const K key);
        vector<V> equal_range(const K key);

    //  Capacity
        int size() const;
        bool empty() const;

    //  Element Access
        vector<V>& operator[](const K& key);
        const vector<V>& operator[](const K& key) const;

    //  Modifiers
        void insert(const K& k, const V& v);
        void erase(const K& key);
        void clear();

    //  Operations:
        bool contains(const K& key);
        vector<V> &get(const K& key);

        Iterator find(const K& key);
        int count(const K& key);

        bool is_valid() { return mmap.is_valid(); }

        friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
            outs<<print_me.mmap<<endl;
            return outs;
        }

    private:
        map_base mmap;
};

template<typename K, typename V>
MMap<K,V>::MMap(){
    ;
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin(){
    return mmap.begin();
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end(){
    return mmap.end();
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K key){
    MMap<K,V>::Iterator finder = find(key);

    if (finder.is_null()){
        for (MMap<K,V>::Iterator it = begin(); it != end(); it++){
            MPair<K,V> pair(key);

            if (*it >= key)
                return it;
        }
    }
    else
        return finder;
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K key){
    MMap<K,V>::Iterator finder = find(key);
    if (finder.is_null()){
        for (MMap<K,V>::Iterator it = begin(); it != end(); it++){
            MPair<K,V> pair(key);

            if (*it > key)
                return it;
        }
    }
    else
        return ++finder;
}

template<typename K, typename V>
vector<V> MMap<K,V>::equal_range(const K key){
    if(!contains(key))
        return vector<V>();
    else
        return get(key);
}

template<typename K, typename V>
int MMap<K,V>::size() const{
    return mmap.size();
}

template<typename K, typename V>
bool MMap<K,V>::empty() const{
    return mmap.empty();
}

template<typename K, typename V>
vector<V>& MMap<K,V>::operator[](const K& key){
    MPair<K,V> mpair(key);
    return mmap.get(mpair).value_list;
}

template<typename K, typename V>
const vector<V>& MMap<K,V>::operator[](const K& key) const{
    return mmap.get_existing(MPair<K,V>(key)).value_list;
}

template<typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v){
   MPair<K,V> mpair(k, v);
   mmap.insert(mpair);
}

template<typename K, typename V>
void MMap<K,V>::erase(const K& key){
    MPair<K,V> mpair(key);
    mmap.remove(mpair);
}

template<typename K, typename V>
void MMap<K,V>::clear(){
    mmap.clear_tree();
}

template<typename K, typename V>
bool MMap<K,V>::contains(const K& key){
    MPair<K,V> mpair(key);
    return mmap.contains(mpair);
}

template<typename K, typename V>
vector<V>& MMap<K,V>::get(const K& key){
    MPair<K,V> mpair(key);
    return mmap.get(mpair);
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
    MPair<K,V> mpair(key);
    MMap<K,V>::Iterator returnIt = mmap.find(mpair);
    return returnIt;
}

template<typename K, typename V>
int MMap<K,V>::count(const K& key){
    if (!contains(key))
        return 0;

    Iterator it = find(key);
    MPair<K,V> temp(*it);
    return temp.value_list.size();
}
#endif // MMAP_H
