#ifndef MAP_H
#define MAP_H
#include <iostream>
#include "bplustree.h"

/* Map is based on the B+ Tree */

using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()){ key = k, value = v;}

    Pair<K, V>& operator =(const Pair<K, V> &rhs){
        if (this != &rhs){
            this->key = rhs.key;
            this->value = rhs.value;
            return *this;
        }
        return *this;
    }

    Pair<K, V> operator +=(const Pair<K, V>& rhs){
        value = rhs.value;
        return *this;
    }

    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        outs << print_me.key << " | " << print_me.value;
        return outs;
    }
    friend bool operator !=(const Pair<K,V>& lhs, const Pair<K,V>& rhs){
        return (lhs.key != rhs.key);
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        bool isEqual = (lhs.key == rhs.key) ? true : false;
        return isEqual;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        bool isLess = (lhs.key < rhs.key) ? true : false;
        return isLess;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        bool isLess = (lhs.key > rhs.key) ? true : false;
        return isLess;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        bool isLess = (lhs.key <= rhs.key) ? true : false;
        return isLess;
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        bool isLess = (lhs.key >= rhs.key) ? true : false;
        return isLess;
    }
};

template <typename K, typename V>
class Map
{
    public:

    typedef BPlusTree<Pair<K, V> > map_base;

    class Iterator{
        public:
            friend class Map;

            Iterator(typename map_base::Iterator it = nullptr){
                _it = it;
            }
            Iterator operator ++(int unused){
                return _it++;
            }
            Iterator operator ++(){
                return ++_it;
            }

            Pair<K, V> operator *(){
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

            bool is_null() { return _it.is_null(); }

        private:
            typename map_base::Iterator _it;

        };

        Map();                              //constructor

    //  Iterators
        Iterator begin();
        Iterator end();
        Iterator lower_bound(const K key);
        Iterator upper_bound(const K key);
        V equal_range(const K key);

    //  Capacity
        int size() const;
        bool empty() const;

    //  Element Access
        V& operator[](const K& key);
        V& operator[](const K& key) const;
        V& at(const K& key);
        const V& at(const K& key) const;

    //  Modifiers
        void insert(const K& k, const V& v);
        void erase(const K& key);
        void clear();
        V get(const K& key);

    //  Operations:
        Iterator find(const K& key);
        bool contains(const Pair<K, V>& target);

        bool is_valid();

        friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
            outs<<print_me.map;
            return outs;
        }

    private:
        int key_count;
        map_base map;
};

template<typename K, typename V>
Map<K,V>::Map(){
    key_count = 0;
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin(){
    return map.begin();
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end(){
    return map.end();
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::lower_bound(const K key){
    if (!map.contains(key)){
        for (Iterator it = map.begin(); it != map.end(); it++){
            Pair<K,V> pair(key, V());

            if (*it >= pair)
                return it;
        }
    }
    else
        return find(key);
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::upper_bound(const K key){
    if (!map.contains(key)){
        for (Iterator it = map.begin(); it != map.end(); ++it){
            Pair<K,V> pair(key);

            if (*it > pair){
                return it;
            }
        }
    }
    else{
        return ++find(key);
    }
}

template<typename K, typename V>
V Map<K, V>::equal_range(const K key){
    if (!map.contains(key)){
        return V();
    }
    else{
        return get(key);
    }
}

template<typename K, typename V>
int Map<K,V>::size() const{
    return map.size();
}

template<typename K, typename V>
bool Map<K,V>::empty() const{
    return map.empty();
}

//Element Access
template<typename K, typename V>
V& Map<K,V>::operator[](const K& key){
//    Pair<K,V> find_me(key);
    return map.get(Pair<K,V>(key)).value;
}

template <typename K, typename V>
V& Map<K,V>::operator[](const K& key)const{
    Pair<K,V> find_me(key, V());
    return map.get_existing(find_me).value;
}
template<typename K, typename V>
V& Map<K,V>::at(const K& key){
    Pair<K,V> pair(key);
    return map.get(pair);
}

template<typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    Pair<K,V> pair(key);
    return map.get_existing(pair);
}

template<typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v){
    Pair<K,V> pair(k,v);
    map.insert(pair);
    key_count++;
}

template<typename K, typename V>
void Map<K,V>::erase(const K& key){
    Pair<K,V> pair(key);
    map.remove(pair);
    key_count--;
}

template<typename K, typename V>
void Map<K,V>::clear(){
    map.clear_tree();
}

template<typename K, typename V>
V Map<K,V>::get(const K& key){
    Pair<K,V> pair(key);
    return map.get(pair).value;
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key){
    Pair<K,V> pair(key);
    return map.find(pair);
}

template<typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target){
    return map.contains(target);
}

template<typename K, typename V>
bool Map<K,V>::is_valid(){
    return map.is_valid();
}
#endif // MAP_H
