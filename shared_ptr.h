#ifndef _SHARED_PTR_
#define _SHARED_PTR_
#include <memory>
#include <new>
/*IN PROGRESS!!*/
#include "type_info.h"
#include "assert.h"
#include "exception.h"
#include "memory.h"

//std::shared_ptr<int> v;

namespace hui{
    //struct that holds counters of shred_ptr. in gnu version there is also 
    //template<Lock_policy lp = default_lock_policy> 
    //not sure waht it means. somehow connected to atomic_shared_ptr? 
    //BUT concurrency safety is good feature to add in a future
    struct ref_counter{
    public:
        ref_counter(int ref_count, int weak_count) noexcept 
            : m_ref_count(ref_count), m_weak_ref_count(weak_count) { }
        
        void destroy(){
            delete this;
        }

        virtual void free_value() = 0;
        virtual void free_ref_counter() = 0;

        int count() const noexcept {
            return m_ref_count;
        }

        void addref() noexcept{
            m_ref_count++;//do it atomicly
            m_weak_ref_count++;
        }

        void release(){
            HUIASSERT(m_ref_count > 0 && m_weak_ref_count > 0);
            if(--m_ref_count == 0)
                free_value();
            if(--m_weak_ref_count == 0)
                free_ref_counter();
        }

        virtual void* get_deleter(const hui::type_info&) noexcept = 0;//not sure why

        virtual ~ref_counter(){}

    private:
        int m_ref_count;
        int m_weak_ref_count;
    private:
        ref_counter(const ref_counter& other) = delete;
        ref_counter& operator=(ref_counter& other) = delete;
    };

    template<typename T, typename Allocator, typename Deleter>
    struct ref_counter_t : public ref_counter {
        using this_type = ref_counter_t<T, Allocator, Deleter>;
        using value_type = T;
        using allocator_type = Allocator;
        using deleter_type = Deleter;
         
        value_type m_ptr;
        deleter_type m_deleter;
        allocator_type m_allocator;

        ref_counter_t(value_type v, Allocator&& al, Deleter&& deler) : ref_counter(1, 0), m_ptr(v), m_allocator(al), m_deleter(deler) { }
    
        void free_value() override {
            m_deleter(m_ptr);
            m_ptr = nullptr;
        }

        void free_ref_counter() override {
            auto alloc_saveure = m_allocator;
            this -> ~ref_counter_t();
            //alloc_saveure.deallocate(this, sizeof(*this));
        }

        void* get_deleter(const hui::type_info&) noexcept override {
            return (void*)&m_deleter;
        }
    };

    template<typename T> struct shared_ptr_traits{
        using reference_type = T&;
    };

    template<> struct shared_ptr_traits<void>{
        using reference_type = void;
    };
    //other specializations!
    
    template<typename T>
    class shared_ptr{
    public:
        using this_type = shared_ptr<T>;
        using value_type = T;
        using reference_type = typename shared_ptr_traits<T>::reference_type;

        template<typename V, typename B, typename D>
        using ref_counter_type_t = hui::ref_counter_t<V, B, D>; 

    public:
        template<typename U, typename Allocator, typename Deleter>
        shared_ptr(U* pval, Allocator&& allocer, Deleter deler) {
            alloc_myself(pval, allocer, deler);
        }
    private:
        template<typename U, typename Allocator_, typename Deleter_>
        void alloc_myself(U val, Allocator_&& alloc, Deleter_ deler) noexcept { 
            try{
                U mem_p = alloc.allocate(sizeof(ref_counter_type_t<U, Allocator_, Deleter_>));
                if(!mem_p)
                    throw hui::bad_alloc();
                m_counter = new ref_counter_t<U, Allocator_, Deleter_>(val, alloc, hui::move(deler));//(ref_counter*)hui::construct<ref_counter_type_t<U, Allocator_, Deleter_>>(mem_p, hui::move(val), hui::move(alloc), hui::move(deler));
                m_p = reinterpret_cast<value_type*>(val); 
                
            }catch(...){
                deler(m_p);
                throw;
            }
        }
    private:
        value_type* m_p;
        ref_counter* m_counter;
    };

};

#endif

