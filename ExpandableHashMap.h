// ExpandableHashMap.h
#include "provided.h"
#include <iostream>

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct Node {
        Node(){
        }
        
        Node(KeyType nKey, ValueType nVal){
            key = nKey;
            val = nVal;
        }
        


        KeyType key;
        ValueType val;
    };


    std::vector<std::list<Node*>> nodes;
    int numBuckets;
    int m_size;
    double loadFactor;
    double maxLoadFactor;
};

//constructor
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
: nodes(8)
{
    numBuckets = 8;
    m_size = 0;
    maxLoadFactor = maximumLoadFactor;
    loadFactor = m_size/numBuckets;
}

//destructor
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    for(int i = 0; i < nodes.size(); i++){
        for(typename std::list<Node*>::iterator j = nodes[i].begin(); j != nodes[i].end(); j++){
            delete *j;
        }
    }
    nodes.clear();
}

//reset
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    numBuckets = 8;
    m_size = 0;
    for(int i = 0; i < nodes.size(); i++){
        for(typename std::list<Node*>::iterator j = nodes[i].begin(); j != nodes[i].end(); j++){
            delete *j;
        }
    }
    nodes = new std::vector<std::list<Node*>>(8);
}

//size
template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_size;
}

//associate
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hasher(const KeyType& k);
    int index = hasher(key) % numBuckets;

    bool found = false;
    
    //searching for the key, and updating value if found
    for(typename std::list<Node*>::iterator i = nodes[index].begin(); i != nodes[index].end(); i++){
        if( (*i)->key == key){
            found = true;
            (*i)->val = value;
        }
    }
    
    //if the key is not found
    if(!found){
        //add the new node into the map
        m_size++;
        loadFactor = m_size/numBuckets;
        nodes[index].push_back(new Node(key, value));
        //if load factor exceeds max load factor
        if(loadFactor >= maxLoadFactor){
            //creating a new hash map
            std::vector<std::list<Node*>> newMap(numBuckets*2);
            //iterating through the buckets
            for(int i = 0; i < nodes.size(); i++){
                //iterating through each list in the bucket
                for(typename std::list<Node*>::iterator j = nodes[i].begin(); j != nodes[i].end(); j++){
                    //hashing and inserting the node into the new map
                    int index = hasher((*j)->key) % (numBuckets*2);
                    
                    newMap[index].push_back(*j);
                }
            }
            nodes = newMap;
            numBuckets *= 2;
        }
        
    }
    
    
}

//find
template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hasher(const KeyType& k);
    
    int index = hasher(key) % numBuckets;

    for(typename std::list<Node*>::const_iterator i = nodes[index].begin(); i != nodes[index].end(); i++){
        if( (*i)->key == key){
            
            return &((*i)->val);
        }
    }
    return nullptr;
}



