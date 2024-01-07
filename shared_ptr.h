#ifndef _SHARDE_PTR_
#define _SHARDE_PTR_

#include <memory>
#include <new>
/*IN PROGRESS!!*/
#include "type_info.h"
#include "assert.h"
#include "exception.h"
#include "memory.h"
#include <iostream>
#include "deleters.h"
#include "functional"
#include "allocator.h"
#include "utility.h"

//std::shared_ptr<int> v;

namespace hui{
    //struct that holds counters of shred_ptr. in gnu version there is also 
    //template<Lock_policy lp = default_lock_policy> 
    //not sure waht it means. somehow connected to atomic_shared_ptr? 
    //BUT concurrency safety is good feature to add in a future
    struct ref_counter{
    public:
        ref_counter(int ref_count)  noexcept 
            : m_ref_count(ref_count){ }
        
        void destroy(){
            delete this;
        }

        virtual void free_value(){
            m_ref_count = 0;
        }
        virtual void free_ref_counter(){
            
        }

        int count() const noexcept {
            return m_ref_count;
        }

        void addref() noexcept{
            m_ref_count++;//do it atomicly
            //m_weak_ref_count++;
        }

        virtual void release(){
            //HUIASSERT(m_ref_count > 0);
            if(--m_ref_count == 0){
                //this -> free_ref_counter();
                free_value();
            }
          //  if(--m_weak_ref_count == 0)
          //      free_ref_counter();
        }

        void print_status(){
            std::cout << m_ref_count << " | " << "\n";
        }

        virtual void* get_deleter(const hui::type_info&) noexcept = 0;//not sure why

        virtual ~ref_counter(){}

    protected:
        int m_ref_count;
        //int m_weak_ref_count;
    private:
        ref_counter(const ref_counter& other) = delete;
        ref_counter& operator=(ref_counter& other) = delete;
    };

    template<typename T, typename Allocator, typename Deleter>
    struct ref_counter_t : ref_counter {
        using this_type = ref_counter_t<T, Allocator, Deleter>;
        using value_type = T;
        using allocator_type = Allocator;
        using deleter_type = Deleter;
         
        value_type m_ptr;
        deleter_type m_deleter;
        allocator_type m_allocator;

        ref_counter_t(value_type v, Allocator&& al, Deleter&& deler) : ref_counter(1),  m_ptr(v), m_allocator(al), m_deleter(deler) { }
        
        void free_value() {
            m_ptr.~value_type();
            m_ptr = nullptr;
        }

        void free_ref_counter() {
            //auto alloc_saveure = m_allocator;
            //this -> ~ref_counter_t();
            //alloc_saveure.deallocate(this, sizeof(*this));
        }

        void* get_deleter(const hui::type_info&) noexcept {
            return (void*)&m_deleter;
        }
    protected:
        int m_ref_count;
        //int m_weak_ref_count;
    
    };

    template<typename T> struct shared_ptr_traits{
        using reference_type = T&;
    };

    template<> struct shared_ptr_traits<void>{
        using reference_type = void;
    };
    //other specializations!, like T[]
    
    template<typename T>
    class shared_ptr{
    public:
        using this_type = shared_ptr<T>;
        using value_type = T;
        using reference_type = typename shared_ptr_traits<T>::reference_type;

        template<typename V, typename B, typename D>
        using ref_counter_type_t = hui::ref_counter_t<V, B, D>; 

    public:
        template<typename U, typename Allocator, typename Deleter = std::function<void(void*)>>
        shared_ptr(U* pval, Allocator&& allocer, Deleter deler) {
            alloc_myself(pval, allocer, deler);
        }

        template<typename U>
        shared_ptr(U* pval){
            alloc_myself(pval, hui::Allocator<U>(), hui::default_deleter<U>()); 
        }

        shared_ptr(const shared_ptr& ptr) : m_p(ptr.m_p), m_counter(ptr.m_counter) {
            m_counter -> addref();
            //print_status();
        }

        shared_ptr(shared_ptr&& ptr) : m_p(ptr.m_p), m_counter(ptr.m_counter) {
            ptr.m_p = nullptr;
            ptr.m_counter = nullptr;
        }

        //add constructor from shredptr of other type using enable_if <is_convertible>
        
        //not sure about that
        shared_ptr& operator =(shared_ptr ptr){
            swap_me(ptr);
            return *this;
        }

        void swap_me(shared_ptr& ptr){
            hui::swap(m_p, ptr.m_p);
            hui::swap(m_counter, ptr.m_counter);
        }

        reference_type operator *(){
            return *m_p;
        }

        value_type* operator ->(){
            return m_p;
        }
        
        value_type* get(){
            return m_p;
        } 

        ~shared_ptr(){
            if(m_counter)
                m_counter -> release();
        }
        
        void print_status(){
            m_counter->print_status(); 
        }

    private:
        template<typename U, typename Allocator_, typename Deleter_>
        void alloc_myself(U val, Allocator_ alloc, Deleter_ deler) noexcept { 
            try{
                void* count_p = alloc.allocate(sizeof(ref_counter_type_t<U, Allocator_, Deleter_>));// TODO: alignment?
                if(!count_p)
                    throw hui::bad_alloc();
                m_counter = (ref_counter_type_t<U, Allocator_, Deleter_>*)hui::construct<ref_counter_type_t<U, Allocator_, Deleter_>>(count_p, val, hui::move(alloc), hui::move(deler));
                //static_cast<ref_counter_type_t<U, Allocator_, Deleter_>*>(m_counter);
                m_p = reinterpret_cast<value_type*>(val); 
                
            }catch(...){
                deler(val);
                //throw;
            }
        }
    private:
        value_type* m_p = nullptr;
        ref_counter* m_counter = nullptr;
    };

    //make_shared
    //the point is to avoid this situation(and simmilar)
    //Buffer* bp = new Buffer();
    //hui::shared_ptr<Buffer> = bp;
    //hui::shared_ptr<Buffer> = bp;
    // ...
    // double free -> UB -> most likely segfault
    // then:
    // auto sbp = make_shared<Buffer>
   
    template<typename T, typename... Args>
    hui::shared_ptr<T> make_shared(Args... args){
       //use alloc_myself here 
    }


    //weak_ptr
    // accurse when you have cyclic pointers like this:
    // for example sahred ptr in Node of a tree
    //  
    // So weak_ptr dosnt have any privilage to control object
    // for example you can make pointers in Node weak_ptr
    // methods of weak_ptr:
    //      expired -> checks if object under it is dead
    //      lock -> returns shared_ptr to underlien
    //      use_count -> checks how many points to
    //      you can create weak_ptr only from shared_ptr or weak_ptr
    //
    // internally looks same 
    //
    // there  is a problem in realization of expired(). See you need to check if counter is 0, but if it is 0 shared_ptr destructor destroys it(=> segFault). So one possible solution is to store 2 counters, for weaks and shared
    // go with this approach?
    //
    
    template<typename T>
    class weak_ptr{
    public:
        using this_type = shared_ptr<T>;
        using value_type = T;
    private:
        

    };

};

#endif

