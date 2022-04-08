#ifndef _RBTREE_
#define _RBTREE_

#include "../huidef.h"
#include "../iterators_tags.h"
#include <type_traits>
#include <utility>

struct rb_tree_node_base{
    rb_tree_node_base* right;
    rb_tree_node_base* left;
    rb_tree_node_base* parent;
    char color;//would be cool to store just a bit
};

template<typename T>
struct rb_tree_node : rb_tree_node_base{
    T val;
};

//TODO:FIX ODR

rb_tree_node_base* rb_tree_increment(rb_tree_node_base* node){
    if(node -> right != nullptr){
        node = node -> right;
        while(node -> left != nullptr)
            node = node -> left;
    }
    else{
        rb_tree_node_base* parent = node -> parent;
        while(parent == node -> right){
            node = parent;
            parent = node -> parent;
        }
        
        if(node -> right != parent)
            node = parent;
    }
    return node;
}

//TODO:finish it
rb_tree_node_base* rb_tree_decrement(rb_tree_node_base* node){
    return nullptr;
}

//TODO:finish it
rb_tree_node_base* rb_tree_rotate_left(rb_tree_node_base* node){
    return nullptr;
}

//TODO:finish it
rb_tree_node_base* rb_tree_rotate_right(rb_tree_node_base* node){
    return nullptr;
}

//TODO:finish it
void rb_tree_insert(rb_tree_node_base* node, rb_tree_node_base* parent, rb_tree_node_base* anchor, char side){
    return;
}

//TODO:finish it
void rb_tree_erase(rb_tree_node_base* node, rb_tree_node_base* anchor){
    return;
}

size_t get_black_count(rb_tree_node_base* top, rb_tree_node_base* bottom){
    size_t count = 0;
    for(; bottom; bottom = bottom -> parent){
        if(bottom -> color == 'b')
            ++count;
        if(bottom == top)
            break;
    }
    return count;
}

rb_tree_node_base* get_min(rb_tree_node_base* node){
    while(node -> left)
        node = node -> left;
    return node;
}

rb_tree_node_base* get_max(rb_tree_node_base* node){
    while(node -> right)
        node = node -> right;
    return node;
}

template<typename T, typename Ptr, typename Ref>
struct rb_tree_iterator{
    using this_type = rb_tree_iterator<T, Ptr, Ref>;
    using iterator = rb_tree_iterator<T, T*, T&>;
    using const_iterator = rb_tree_iterator<T, const T*, const T&>;
    using difference_type = hui::ptrdiff_t;
    using value_type = T;
    using pointer = Ptr;
    using reference = Ref;
    using node_type = rb_tree_node<value_type>;
    using iterator_category = hui::bidirectional_iterator_tag; 
public:
    node_type* node;
public:
    rb_tree_iterator() : node(nullptr) {}

    explicit rb_tree_iterator(node_type* node_in) : node(node_in) {}
    
    rb_tree_iterator(const iterator& iter) : node(iter.node) {}

    rb_tree_iterator& operator =(const iterator* iter){
        node = iter.node;
        return *this;
    }
    
    reference operator *(){
        return *(node);
    }

    pointer operator ->(){
        return node;
    }

    this_type& operator ++();
    this_type& operator --();
    this_type operator ++(int);
    this_type operator --(int);
};


template<typename Comparer, bool is_empty = std::is_empty<Comparer>()>
struct rb_tree_compare{
protected:
    rb_tree_compare(){}

    rb_tree_compare(const Comparer& comp) : Comparer(comp) {}

    const Comparer& get_compare() const{
        return *this;
    }

    Comparer& get_compare(){
        return *this;
    }
    
    template<typename T>
    bool compare(const T& l, const T& r){
        return m_comparer;
    }
    
    template<typename T>
    bool compare(const T& l, const T& r) const{
        return m_comparer;
    }
private:
    Comparer m_comparer;
};


template<typename Comparer>
struct rb_tree_compare<Comparer, false> : public Comparer{
protected:
    rb_tree_compare(){}

    rb_tree_compare(const Comparer& comp) : Comparer(comp) {}

    const Comparer& get_compare() const{
        return *this;
    }

    Comparer& get_compare(){
        return *this;
    }
    
    template<typename T>
    bool compare(const T& l, const T& r){
        return Comparer::operator()(l, r);
    }
    
    template<typename T>
    bool compare(const T& l, const T& r) const{
        return Comparer::operator()(l, r);
    }
};

template<typename Key, typename Pair, typename Comparer>
struct rb_tree_base : public rb_tree_compare<Comparer> {
    using rb_tree_compare<Comparer>::get_compare;
    using rb_tree_compare<Comparer>::compare;

    rb_tree_base(){}
    rb_tree_base(const Comparer& comp) : rb_tree_compare<Comparer>(comp) {}
};

template<typename Key, typename Value, typename Comparer, typename Allocator>
struct rb_tree : rb_tree_base<Key, Value, Comparer>{
    
};

#endif

