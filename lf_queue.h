#ifndef _LOCK_FREE_QUEUE_
#define _LOCK_FREE_QUEUE_

#include "unique_ptr.h"
#include "allocator.h"
#include <pthread.h>
#include <iostream>

namespace hui{
    template<typename T>
    class lf_queue{
        template<typename U>
        struct node;
    public:
        using value_type = T;
        //using allocator_type = Allocator;
        static constexpr int node_size = sizeof(node<value_type>);
        //...
    
    public:
        lf_queue() : head(alloc.allocate(node_size)), tail(head) {}

        lf_queue(const value_type& val) :  head(alloc.allocate(node_size)), tail(head)  {
            alloc.construct(head, val);
        }

        lf_queue(value_type&& val) : head(alloc.allocate(node_size)), tail(head){   
           alloc.construct(head, val); 
        }

        void push(const value_type& v){
            //node<value_type>* new_node = alloc.allocate(node_size);//is this okay to use same allocator? 
            //alloc.construct(new_node, v); 
            //node<value_type>* n;
            //while(1){
            //    n = tail;  
            //    if(__sync_bool_compare_and_swap((void*)((n->next).get_ptr()), NULL, new_node)){
            //        break; 
                //}else{
                //    __sync_bool_compare_and_swap(&(tail), n, &(n -> next));
                //    break; 
            //    }
            //}
            //__sync_bool_compare_and_swap(&(tail), n, &new_node);
        }

        void get_nodes(){
            auto curr = head;

            while(1){
                if(curr){
                    std::cout << curr -> val;       
                    curr = curr -> next;
                }
            }
        }
        
        ~lf_queue(){
            //later
        }

    private:
        template<typename U>
        struct node {
            U val;
            hui::unique_ptr<node> next;
            node(const U& val_in) : val(val_in), next(nullptr) {}
        };
   
        node<value_type>* head = nullptr;    
        node<value_type>* tail = nullptr;

        hui::Allocator<node<T>> alloc;        
    };
};
#endif

