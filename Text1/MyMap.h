#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

#include <map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN
#include "support.h"

// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
// since you are not allowed to use any STL associative containers, and
// this implementation uses std::map.

// This code is deliberately obfuscated.

// If you can not get your own MyMap class template working, you may use
// this one during development in order to let you proceed with implementing
// the other classes for this project; you can then go back to working on
// fixing your own MyMap class template.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap(); ///done!
    ~MyMap();
    void clear();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node*    m_right = nullptr;
        Node*    m_left = nullptr;
    };
    
    int m_size;
    Node* m_root;
    
    void freeTree(Node* cur);
   
   // const ValueType* helpfind(Node* cur, const KeyType& key) const;
    
    
};


template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
    m_root= nullptr;
    m_size = 0;
    
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
    clear();
    
}


template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
    return m_size;
}


template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::freeTree(Node* cur)
{
    if (cur==nullptr)
        return;
    freeTree(cur->m_left);
    freeTree(cur->m_right);
    delete cur;
    
    
}

/*template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::helpfind(Node* cur, const KeyType& key) const
{
    if (cur==nullptr)
        return nullptr;
    else if (key ==cur->m_key)
        return &(cur->m_value);
    else if (key < cur->m_key)
        return helpfind(cur->m_left, key);
    else
        return helpfind(cur->m_right, key);
}
 */




template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
    freeTree(m_root);
    
}





template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
   // return helpfind(m_root, key);
    
     Node* current = m_root;
    while (current != nullptr) ///okayyyyy i guess here's the problem?
    {
        if (key == current->m_key)
            break;
        if (key > current->m_key)
            current = current->m_right;
        else if (key < current->m_key )
            current = current->m_left;
    }
    if (current == nullptr)
        return nullptr;
    const ValueType* pointer = &(current->m_value);
    return pointer;
    
}



template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    
    if (m_root == nullptr)
    {
        m_root= new Node;
        m_root->m_key = key;
        m_root->m_value= value;
        m_size++;
        return;
    }
    Node* current = m_root;
    
    for(;;)
    {
        if (key == current->m_key)
        {
            current->m_value = value;
            return;
        }
        if (key < current->m_key)
        {
            if (current->m_left!=nullptr)
                current = current->m_left;
            else
            {
                current->m_left = new Node;
                current->m_left->m_key = key;
                current->m_left->m_value = value;
                m_size++;
                return;
            }
        }
        else if (key > current->m_key)
        {
            if (current->m_right!=nullptr)
                current = current->m_right;
            else
            {
                current->m_right = new Node;
                current->m_right->m_key = key;
                current->m_right->m_value = value;
                m_size++;
                return;
            }
        }
        
    }
    
}




/*template <typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap(const MyMap &other) = delete;
    MyMap &operator=(const MyMap &other) = delete;
    typedef KeyType O;using O1=int;void clear(){l01.clear();}
    using l0=ValueType;MyMap(){}using l10=O const;O1 size()
    const{return l01.size();}using ll0=l0 const;using Const=
    MyMap<O,l0>;void associate(l10&Using,ll0&first){l01[
                                                        Using]=first;}using l1=std::map<O,l0>;using l00=Const
    const;ll0*find(l10&l11)const{auto first=l01.find(l11);
        return(first!=l01.end()?&first->second:0);}l0*find(l10&
                                                           l01){return(l0*)(*(l00*)(this)).find(l01);}private:l1
    l01;
};
 */

#endif // MYMAP_INCLUDED
